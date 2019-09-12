#pragma once
#include<string>
#include<iostream>
using std::string;
using std::cout;
using std::endl;
using std::cin;


/*--- ����ջģ�� --------------------------------------------*/
#include<memory>

template<typename T>
struct StackNode_
{
	T data;
	StackNode_ *prev; // ����ָ��ջǰһ��Ԫ�ء�
};


// TΪ�洢��������ͣ�AllocΪ�ڴ��������Ĭ��ʹ��allocator��
template<typename T,class Alloc = std::allocator<T>>
class StackAlloc
{
public:
	// ��TΪ�������͵����ݽṹStackNode_��������ΪNode
	typedef StackNode_<T> Node;
	// ��NodeΪ�������͵��ڴ������allocator
	typedef typename Alloc::template rebind<Node>::other allocator; 

	StackAlloc() { head_ = 0; }
	~StackAlloc() { clear(); }

	// �ж�ջ�Ƿ�Ϊ��,Ϊ�շ���true��
	bool empty() { return head_ == 0; }

	// �����ڴ�,�ͷ�ջԪ�������ڴ档
	void clear();

	// ��ջ��
	void push(T elem);

	// ��ջ��
	T pop();

	// ����ջ��Ԫ�ء�
	T top() { return head_->data; }


private:
	allocator allocator_;     // ջ�ڴ������
	Node*     head_;          // ջ���ڵ�

};


// ģ��Ķ�������������ͬһ���ļ���
// ע����ģ���У����ⶨ�庯���ķ�ʽ��template<>���Լ�StackAlloc<T,Alloc>����
template<typename T, class Alloc>
void StackAlloc<T,Alloc>::clear()
{
	Node* curr = head_;

	while (curr != 0)
	{
		Node* tmp = curr->prev;

		// ���������ٻ����ڴ档

		// destroy()�������͵�����������������Ӧ�ڴ��ϵ����ݣ�����ַ��Ȼ��ռ��״̬��
		allocator_.destroy(curr);
		// deallocate()�ͷ��ڴ棬���ڴ��ַ����ϵͳ����ʱ���ڴ��ַʹ������-1��
		allocator_.deallocate(curr, 1);

		curr = tmp;
	}
	head_ = 0;
}


template<typename T, class Alloc>
void StackAlloc<T, Alloc>::push(T elem)
{
	// Ϊ�ڵ�����ڴ档
	Node* newNode = allocator_.allocate(1);
	// ���ýڵ㹹�캯����
	allocator_.construct(newNode, Node());

	// ��ջ������
	newNode->data = elem;
	newNode->prev = head_;
	head_ = newNode;
}


template<typename T, class Alloc>
T StackAlloc<T, Alloc>::pop()
{
	// ��ջ���������س�ջԪ�ء�
	T result = head_->data;
	Node* tmp = head_->prev;

	allocator_.destroy(head_);
	allocator_.deallocate(head_, 1);

	head_ = tmp;
	return result;
}




/*--- �ڴ��ģ�� --------------------------------------------*/
#include<climits>
#include<cstddef>

/*
������ڴ���У���ʵ��ʹ�����������������ڴ�ص��ڴ�����ġ�
�ڴ�����Ȼᶨ��̶���С�Ļ����ڴ�����(Block)��
Ȼ�������ж�����һ������ʵ����Ϊ��Ŷ����ڴ�۵Ķ���ۣ�Slot_���Ͷ����ָ��������塣
Ȼ���������У��������ĸ��ؼ����ʵ�ָ�룬���ǵ����÷ֱ��ǣ�

currentBlock_: ָ��ǰ�ڴ������ָ��
currentSlot_: ָ��ǰ�ڴ������еĶ����
lastSlot_: ָ��ǰ�ڴ������е����һ�������
freeSlots_: ָ��ǰ�ڴ����������п��еĶ����
����������ڴ�ص���ƽṹ֮�����ǾͿ��Կ�ʼʵ�ֹؼ��Ե��߼��ˡ�
*/


#define BlockSize  4096

template<typename T>
class MemoryPool
{
public:
	typedef T* pointer;

	// ����rebind<U>::other�ӿڡ�
	template<typename U> struct rebind
	{
		typedef MemoryPool<U> other;
	};

	// Ĭ�Ϲ��캯������ʼ����ָ�롣
	// noexcept��ʾ�ú��������׳��쳣��
	MemoryPool() noexcept
	{
		currentBlock_ = nullptr;
		currentSlot_ = nullptr;
		lastSlot_ = nullptr;
		freeSlots_ = nullptr;
	}
	// �����ڴ�ء�
	~MemoryPool()noexcept;

	// ͬһʱ��ֻ�ܷ���һ������n��hint�ᱻ���ԡ�
	pointer allocate(size_t n = 1, const T* hint = 0)
	{
		// ����п��еĶ���ۣ���ֱ�ӽ��������򽻸���ȥ��
		if (freeSlots_ != nullptr)
		{
			pointer result = reinterpret_cast<pointer>(freeSlots_);
			freeSlots_ = freeSlots_->next;
			return result;
		}
		// ���򣬷���һ���µ��ڴ����顣
		else
		{
			if (currentSlot_ >= lastSlot_)
			{
				// ����һ���µ��ڴ����飬��ָ��ǰһ���ڴ����顣
				data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
				reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
				currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);

				// ���������������Ԫ���ڴ�����Ķ���Ҫ��
				data_pointer_ body = newBlock + sizeof(slot_pointer_);
				uintptr_t result = reinterpret_cast<uintptr_t>(body);
				size_t bodyPadding = (alignof(slot_type_)-result) % alignof(slot_type_);
				currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
				lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_) + 1);
			}

			return reinterpret_cast<pointer>(currentSlot_++);
		}
	}

	// ����ָ��pָ����ڴ����顣
	void deallocate(pointer p, size_t n = 1);

	// ���ö����캯������ʼ����
	template<typename U,typename... Args>
	void construct(U* p, Args&&... args)
	{
		new (p) U(std::forward<Args>(args)...);
	}

	// ���ö������������������ڴ���еĶ���
	template<typename U>
	void destroy(U* p)
	{
		p->~U();
	}


private:


	// ������ṹ���ͣ����ڴ洢�ڴ���еĶ���ۣ�
	// Ҫô��ʵ����Ϊһ����Ŷ���Ĳۣ�
	// Ҫô��ʵ����Ϊһ��ָ���Ŷ���۵Ĳ�ָ�롣
	union Slot_
	{
		T elem;
		Slot_ *next;
	};

	// ����ָ�롣
	typedef char* data_pointer_;
	// ����ۡ�
	typedef Slot_ slot_type_;
	// �����ָ�롣
	typedef Slot_* slot_pointer_;

	// ָ��ǰ�ڴ����顣
	slot_pointer_ currentBlock_;
	// ָ��ǰ�ڴ������һ������ۡ�
	slot_pointer_ currentSlot_;
	// ָ��ǰ�ڴ���������һ������ۡ�
	slot_pointer_ lastSlot_;
	// ָ��ǰ�ڴ�����Ŀ��ж���ۡ�
	slot_pointer_ freeSlots_;

	// ��̬���ԣ���鶨����ڴ�ش�С�Ƿ��С��
	static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small");
};


template<typename T>
MemoryPool<T>::~MemoryPool()noexcept
{
	slot_pointer_ curr = currentBlock_;

	// ѭ�������ڴ���з�����ڴ����顣
	while (curr != nullptr)
	{
		slot_pointer_ prev = curr->next;
		// reinterpret_cast<type-id>(expression),��currת��Ϊvoid*ָ�롣
		// operator,???��operator ɾ���ƺ�û��Ӱ�죩��
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
}


// �ƺ�����pointer�����⣬�޷����ⶨ�塣
//template<typename T>
//pointer MemoryPool<T>::allocate(size_t n, const T* hint)
//{
//	// ����п��еĶ���ۣ���ֱ�ӽ��������򽻸���ȥ��
//	if (freeSlots_ != nullptr)
//	{
//		pointer result = reinterpret_cast<pointer>(freeSlots_);
//		freeSlots_ = freeSlots_->next;
//		return result;
//	}
//	// ���򣬷���һ���µ��ڴ����顣
//	else
//	{
//		if (currentSlot_ >= lastSlot_)
//		{
//			// ����һ���µ��ڴ����飬��ָ��ǰһ���ڴ����顣
//			data_pointer_ newBlock = reinterpret_cast<data_pointer>(operator new(BlockSize));
//			reinterpret_cast<slot_pointer_>(newBlock_)->next = currentBlock_;
//			currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
//
//			// ���������������Ԫ���ڴ�����Ķ���Ҫ��
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
		// Ϊ����next������ǿ�ƽ�pת��slot_pointer_.
		reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
		freeSlots_ = reinterpret_cast<slot_pointer_>(p);
	}
}








