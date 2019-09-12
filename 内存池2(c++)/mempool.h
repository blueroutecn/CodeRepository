#pragma once
#include<string>
#include<iostream>
using std::string;
using std::cout;
using std::endl;
using std::cin;


/*--- 链表栈模版 --------------------------------------------*/
#include<memory>

template<typename T>
struct StackNode_
{
	T data;
	StackNode_ *prev; // 链表，指向栈前一个元素。
};


// T为存储对象的类型，Alloc为内存分配器（默认使用allocator）
template<typename T,class Alloc = std::allocator<T>>
class StackAlloc
{
public:
	// 以T为变量类型的数据结构StackNode_，重命名为Node
	typedef StackNode_<T> Node;
	// 以Node为变量类型的内存分配器allocator
	typedef typename Alloc::template rebind<Node>::other allocator; 

	StackAlloc() { head_ = 0; }
	~StackAlloc() { clear(); }

	// 判断栈是否为空,为空返回true。
	bool empty() { return head_ == 0; }

	// 清理内存,释放栈元素所有内存。
	void clear();

	// 入栈。
	void push(T elem);

	// 出栈。
	T pop();

	// 返回栈顶元素。
	T top() { return head_->data; }


private:
	allocator allocator_;     // 栈内存分配器
	Node*     head_;          // 栈顶节点

};


// 模版的定义必须和声明在同一个文件。
// 注意类模版中，类外定义函数的方式（template<>，以及StackAlloc<T,Alloc>）。
template<typename T, class Alloc>
void StackAlloc<T,Alloc>::clear()
{
	Node* curr = head_;

	while (curr != 0)
	{
		Node* tmp = curr->prev;

		// 先析构，再回收内存。

		// destroy()调用类型的析构函数，销毁相应内存上的内容，但地址依然被占用状态。
		allocator_.destroy(curr);
		// deallocate()释放内存，将内存地址返回系统，此时该内存地址使用引用-1。
		allocator_.deallocate(curr, 1);

		curr = tmp;
	}
	head_ = 0;
}


template<typename T, class Alloc>
void StackAlloc<T, Alloc>::push(T elem)
{
	// 为节点分配内存。
	Node* newNode = allocator_.allocate(1);
	// 调用节点构造函数。
	allocator_.construct(newNode, Node());

	// 入栈操作。
	newNode->data = elem;
	newNode->prev = head_;
	head_ = newNode;
}


template<typename T, class Alloc>
T StackAlloc<T, Alloc>::pop()
{
	// 出栈操作，返回出栈元素。
	T result = head_->data;
	Node* tmp = head_->prev;

	allocator_.destroy(head_);
	allocator_.deallocate(head_, 1);

	head_ = tmp;
	return result;
}




/*--- 内存池模版 --------------------------------------------*/
#include<climits>
#include<cstddef>

/*
在这个内存池中，其实是使用链表来管理整个内存池的内存区块的。
内存池首先会定义固定大小的基本内存区块(Block)，
然后在其中定义了一个可以实例化为存放对象内存槽的对象槽（Slot_）和对象槽指针的联合体。
然后在区块中，定义了四个关键性质的指针，它们的作用分别是：

currentBlock_: 指向当前内存区块的指针
currentSlot_: 指向当前内存区块中的对象槽
lastSlot_: 指向当前内存区块中的最后一个对象槽
freeSlots_: 指向当前内存区块中所有空闲的对象槽
梳理好整个内存池的设计结构之后，我们就可以开始实现关键性的逻辑了。
*/


#define BlockSize  4096

template<typename T>
class MemoryPool
{
public:
	typedef T* pointer;

	// 定义rebind<U>::other接口。
	template<typename U> struct rebind
	{
		typedef MemoryPool<U> other;
	};

	// 默认构造函数，初始化槽指针。
	// noexcept表示该函数不会抛出异常。
	MemoryPool() noexcept
	{
		currentBlock_ = nullptr;
		currentSlot_ = nullptr;
		lastSlot_ = nullptr;
		freeSlots_ = nullptr;
	}
	// 销毁内存池。
	~MemoryPool()noexcept;

	// 同一时间只能分配一个对象，n和hint会被忽略。
	pointer allocate(size_t n = 1, const T* hint = 0)
	{
		// 如果有空闲的对象槽，则直接将空闲区域交付出去。
		if (freeSlots_ != nullptr)
		{
			pointer result = reinterpret_cast<pointer>(freeSlots_);
			freeSlots_ = freeSlots_->next;
			return result;
		}
		// 否则，分配一个新的内存区块。
		else
		{
			if (currentSlot_ >= lastSlot_)
			{
				// 分配一个新的内存区块，并指向前一个内存区块。
				data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
				reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
				currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);

				// 填补整个区块来满足元素内存区域的对齐要求。
				data_pointer_ body = newBlock + sizeof(slot_pointer_);
				uintptr_t result = reinterpret_cast<uintptr_t>(body);
				size_t bodyPadding = (alignof(slot_type_)-result) % alignof(slot_type_);
				currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
				lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_) + 1);
			}

			return reinterpret_cast<pointer>(currentSlot_++);
		}
	}

	// 销毁指针p指向的内存区块。
	void deallocate(pointer p, size_t n = 1);

	// 调用对象构造函数，初始化。
	template<typename U,typename... Args>
	void construct(U* p, Args&&... args)
	{
		new (p) U(std::forward<Args>(args)...);
	}

	// 调用对象析构函数，销毁内存池中的对象。
	template<typename U>
	void destroy(U* p)
	{
		p->~U();
	}


private:


	// 共用体结构类型，用于存储内存池中的对象槽，
	// 要么被实例化为一个存放对象的槽，
	// 要么被实例化为一个指向存放对象槽的槽指针。
	union Slot_
	{
		T elem;
		Slot_ *next;
	};

	// 数据指针。
	typedef char* data_pointer_;
	// 对象槽。
	typedef Slot_ slot_type_;
	// 对象槽指针。
	typedef Slot_* slot_pointer_;

	// 指向当前内存区块。
	slot_pointer_ currentBlock_;
	// 指向当前内存区块的一个对象槽。
	slot_pointer_ currentSlot_;
	// 指向当前内存区块的最后一个对象槽。
	slot_pointer_ lastSlot_;
	// 指向当前内存区块的空闲对象槽。
	slot_pointer_ freeSlots_;

	// 静态断言，检查定义的内存池大小是否过小。
	static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small");
};


template<typename T>
MemoryPool<T>::~MemoryPool()noexcept
{
	slot_pointer_ curr = currentBlock_;

	// 循环销毁内存池中分配的内存区块。
	while (curr != nullptr)
	{
		slot_pointer_ prev = curr->next;
		// reinterpret_cast<type-id>(expression),将curr转换为void*指针。
		// operator,???（operator 删除似乎没有影响）。
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
}


// 似乎由于pointer的问题，无法类外定义。
//template<typename T>
//pointer MemoryPool<T>::allocate(size_t n, const T* hint)
//{
//	// 如果有空闲的对象槽，则直接将空闲区域交付出去。
//	if (freeSlots_ != nullptr)
//	{
//		pointer result = reinterpret_cast<pointer>(freeSlots_);
//		freeSlots_ = freeSlots_->next;
//		return result;
//	}
//	// 否则，分配一个新的内存区块。
//	else
//	{
//		if (currentSlot_ >= lastSlot_)
//		{
//			// 分配一个新的内存区块，并指向前一个内存区块。
//			data_pointer_ newBlock = reinterpret_cast<data_pointer>(operator new(BlockSize));
//			reinterpret_cast<slot_pointer_>(newBlock_)->next = currentBlock_;
//			currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
//
//			// 填补整个区块来满足元素内存区域的对齐要求。
//			data_pointer_ body = newBlock + sizeof(slot_pointer_);
//			uintptr_t result = reinterpret_cast<uintptr>(body);
//			size_t bodyPadding = (alignof(slot_type_)-result) % alignof(slot_type_);
//			currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
//			lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_) + 1);
//		}
//
//		return reinterpret_cast<pointer>(currentSlot_++);
//	}
//}


template<typename T>
void MemoryPool<T>::deallocate(pointer p, size_t n)
{
	if (p != nullptr)
	{
		// 为访问next，必须强制将p转成slot_pointer_.
		reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
		freeSlots_ = reinterpret_cast<slot_pointer_>(p);
	}
}








