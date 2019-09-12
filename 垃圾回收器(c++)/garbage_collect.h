#pragma once
//------------------------------------------------------------------
//一个单线程垃圾回收器（内存回收）
//方案:
//引用计数法（reference counting），
//每一个动态配置的内存（动态堆内存）都有一个与之相关的引用计数器。
//每增加一个对该内存的引用，计数器加1；每减少一个引用，计数器减1；
//当计数器为0，则该内存为使用，可以删除释放。
//缺陷：
//环状引用（circle reference）。
//------------------------------------------------------------------



#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>
using namespace std;



//定义DISPLAY以展示垃圾回收活动。
#define DISPLAY



//当Iter超出指向对象的范围时，抛出该异常。
//自定义。
class OutOfRangeExc
{
public:
private:
};



//------------------------------------------------------------------
//class Iter中，
//重载了算数运算在内的所有指针操作，类似STL；
//主要用于遍历class GCPtr所指的数组，并提供边界检查；
//Iter指针不参加或影响垃圾回收过程；
//可以通过GCPtr对象的begin()、end()函数获得Iter对象。
//------------------------------------------------------------------

template <class T>
class Iter
{
public:
	Iter()
	{
		ptr = end = begin = nullptr;
		length = 0;
	}

	Iter(T *p, T *first, T *last)
	{
		ptr = p;
		begin = first;
		end = last;
		length = last - first;
	}

	
	unsigned size()
    //返回Iter所指向的数列长度。
	{
		return length;
	}


	T & operator *()
	//重载运算符*。
	//返回当前指针ptr所指向的值本身（reference）。
	//不允许越界存取。
	{
		if ((ptr >= end) || (ptr < begin))
			throw OutOfRangeExc();
		return *ptr;
	}


	T  *operator ->()
	//重载运算符->。
	//返回当前指针ptr所指向的地址（copy）。
	//不允许越界存取。
	{
		if ((ptr >= end) || (ptr < begin))
			throw OutOfRangeExc();
		return ptr;
	}


	Iter operator ++()
	//重载单目运算符++（前置式）。
	//返回修改后的Iter对象（copy），ptr加1，end、begin不变。
	//*this 即当前class对象，下同。
	//该函数可能造成指针越界，下同。
	{
		ptr++;
		return *this;
	}

	Iter operator --()
	{
		ptr--;
		return *this;
	}


	Iter operator ++(int notused)
	//重载单目运算符++（后置式）。
	//构造并返回一个修改前的Iter对象（高效），下同。
	//当前class对象，ptr加1，end、begin不变。
	{
		T *temp = ptr;
		ptr++;
		return Iter<T>(temp, begin, end);
	}

	Iter operator --(int notused)
	{
		T *temp = ptr;
		ptr--;
		return Iter<T>(temp, begin, end);
	}


	T & operator [](int i)
	//重载运算符[]。
	//返回指定的数组元素（reference），不允许越界读取。
	{
		if ((i < 0) || (i >= (end - begin)))
			throw OutOfRangeExc();
		return ptr[i];
	}

	//定义一系列相对关系双目运算符。
	bool operator ==(Iter op2)
	{
		return ptr == op2.ptr;
	}

	bool operator != (Iter op2)
	{
		return ptr != op2.ptr;
	}

	bool operator > (Iter op2)
	{
		return ptr > op2.ptr;
	}

	bool operator >= (Iter op2)
	{
		return ptr >= op2.ptr;
	}

	bool operator < (Iter op2)
	{
		return ptr < op2.ptr;
	}

	bool operator <= (Iter op2)
	{
		return ptr <= op2.ptr;
	}

	
	Iter operator - (int n)
	//重载双目运算符-。
	//返回返回修改后的Iter对象（copy），ptr减n，end、begin不变。
	{
		ptr -= n;
		return *this;
	}

	Iter operator + (int n)
	{
		ptr += n;
		return *this;
	}

	int operator - (Iter<T> &itr2)
	//重载双目运算符-。
	//返回当前class对象与输入class对象中两ptr指针间元素个数。
	{
		return ptr - itr2.ptr;
	}


private:
	T * ptr;           //当前指针
	T *end;            //指向终端的下一位置
	T *begin;          //指向数组的起点
	unsigned length;   //数列长度
};



//------------------------------------------------------------------
//class GCInfo中，
//memPtr     是指向动态堆内存的指针（地址）；
//refcount   是动态堆内存的使用计数器；
//isArray，  当memPtr指向的是数组时isArray为true；
//arraySize，当isArray为true时arraySize为数组长度。
//所以，GCInfo对象就将动态堆内存与使用计数器联系起来。
//------------------------------------------------------------------

template<class T>
class GCInfo
{
public:
	GCInfo(T *mPtr, unsigned size = 0)
	//mPtr执行分配的内存，若该内存是一个数组，则size为数组大小。
	{
		refcount = 1;
		memPtr = mPtr;

		if (size != 0)
			isArray = true;
		else
			isArray = false;

		arraySize = size;
	}


	unsigned refcount;   //当前内存的引用个数。
	T *memPtr;           //指向分配的内存。
	bool isArray;        //若memPtr指向一个数组内存，为true；否则，为false。
	unsigned arraySize;  //若isArray为true，则arraySize为指向数组的大小。
};



//重载运算符==。
//实现GCInfo对象的比较。
template<class T>
bool operator ==(const GCInfo<T> &ob1,const GCInfo<T> &ob2)
{
	//当两个GCInfo对象指向的内存相同时，认为二者相同。
	return (ob1.memPtr == ob2.memPtr);
}



//GCPtr实现一个指针类型，以垃圾回收机制释放未使用的内存。
//GCPtr对象默认指向一段以NEW分配的内存。
//若令GCPtr对象指向一段分配的数组，则必须指定数组大小（从而正确释放内存）。
template<class T,int size = 0>
class GCPtr
{
public:
	//为GCPtr<T>定义一个迭代器类型。
	//可以通过GCiterator 定义GCPtr的迭代器。
	typedef  Iter<T>   GCiterator;

	GCPtr(T *t = nullptr)
	//默认为空指针。
	{
		//若是产生的第一个GCPtr对象，则将shutdown()注册为退离函数。
		//因已经标记出第一个GCPtr对象，立即将first变量设为false。
		if (first)  
			atexit(shutdown);
		first = false;

		list<GCInfo<T>>::iterator p;
		p = findPtrInfo(t);

		//若t在gclist内存在，则累加其引用计数器。
		//否则，构造GCInfo对象并将加入gclist内。
		if (p != gclist.end())
			p->refcount++;
		else
		{
			GCInfo<T> gcObj(t, size);
			gclist.push_front(gcObj);
		}

		//储存该内存地址（指针）。
		addr = t;
		arraySize = size;

		if (size > 0) isArray = true;
		else isArray = false;

#ifdef DISPLAY
		cout << "constructing GCPtr. ";
		if (isArray) 
			cout << "size is： " << arraySize << "\n";
		else cout << "\n";
#endif
	}

	GCPtr(const GCPtr &ob)
	//复制式构造。
	{
		list<GCInfo<T>>::iterator p;
		p = findPtrInfo(ob.addr);
		p->refcount++;

		addr = ob.addr;
		arraySize = ob.arraySize;
		if (size > 0) isArray = true;
		else isArray = false;

#ifdef DISPLAY
		cout << "constructing GCPtr. ";
		if (isArray) 
			count << "size is： " << arraySize << endl;
		else cout << endl;
#endif
	}

	~GCPtr();

	//GCPtr类中静态函数，回收垃圾。
	//若至少释放了一个对象，返回true。
	static bool collect();

	//重载赋值运算符=。
	//通过指针对GCPtr赋值。
	T *operator = (T *t);

	//重载赋值运算符=。
	//通过GCPtr对象对GCPtr赋值。
	GCPtr &operator = (GCPtr &rv);

	T &operator *()
	//重载运算符*。
	//返回GCPtr对象指向的元素（reference）。
	{
		return *addr;
	}

	T *operator ->()
	//重载运算符->。
	//返回GCPtr对象指向的地址（copy）。
	{
		return addr;
	}

	T &operator [](int i)
	//重载运算符[]。
	//返回指定的数组元素（reference）。
	{
		return addr[i];
	}

	operator T *()
	//重载运算符*。
	//返回GCPtr对象指向的地址。
    //也就是将GCPtr对象转化为一个普通指针（typename T）。
	//该类型转化不影响垃圾回收，即如果一个普通指针p获得了addr，该addr指向的内存的计数器不会增加。
	//同理，即使一个普通指针指向该addr的内存，collect（）依然可以释放该内存，则p就成了一个野指针。
	{
		return addr;
	}

	Iter<T> begin()
	//返回一个Iter对象（copy）。
	//该功能时为了创建一个Iter迭代器，从而可以操作addr指向的内存。
	{
		int size;
		if (isArray) size = arraySize;
		else size = 1;

		return Iter<T>(addr, addr, addr + size);
	}

	Iter<T> end()
	//返回一个Iter对象（copy）。
	//该对象当前指向GCPtr所指内存的终端的下一地址，数组长度不变。
	{
		int size;
		if (isArray) size = arraySize;
		else size = 1;

		return Iter<T>(addr + size, addr, addr + size);
	}

	static int gclistSize()
	{
		return gclist.size();
	}

	//GCPtr类的静态函数，用来显示gclist。
	static void showlist();
	
	//GCPtr类静态函数，程序结束时清空gclist。
	//释放所有因环状引用而无法正常释放的内存。
	static void shutdown();


private:
	static list<GCInfo<T>> gclist;    //静态变量，一个GCInfo<T>类型的list，储存各动态内存的指针。
	T *addr;                          //GCPtr类对象当前指向的内存。
	bool isArray;                     //若GCPtr类对象指向数组，为true；否则，为false。
	unsigned arraySize;                    //若isArray为true，则arraySize为数组大小。
	static bool first;                //静态变量，当第一个GCPtr类对象产生时为true。
	                                  //first被用来注册一个退离函数，程序结束时被调用来关闭垃圾回收器。
	typename  list<GCInfo<T>>::iterator  findPtrInfo(T *ptr); //定义一个iterator，指向gclist中指针信息。
};



//为GCPtr类的静态变量gclist保留空间。
//这里template中size就不在写出默认值，下同。
template<class T, int size>
list<GCInfo<T>> GCPtr<T, size>::gclist;



//为GCPtr类的静态变量first赋值。
template<class T, int size>
bool GCPtr<T, size>::first = true;



//定义GCPtr类的析构函数。
//每当有指针越界时，执行析构函数。
template<class T, int size>
GCPtr<T, size>::~GCPtr()
{
	list<GCInfo<T>>::iterator p;

	//找到即将删除或离开作用域的GCPtr对象的指针，将关联的内存的引用计数器减1。
	p = findPtrInfo(addr);
	if (p->refcount) 	p->refcount--;

#ifdef DISPLAY
	cout << "GCPtr going out of scope.\n";
#endif

	//进行垃圾回收。
	collect();
}



//------------------------------------------------------------------
//实际当中，只有在一定情况下才进行垃圾回收：
//如gclist达到一定大小；
//如一定数量的GCPtr对象超出作用域；
//如内存不足时，才进行垃圾回收。
//------------------------------------------------------------------



//若至少一个对象被释放，返回true。
//因为collect()是GCPtr的静态函数，所以可以随时直接调用以检查和释放泄漏的内存。
//如GCPtr<int>::collect();就是释放gclist<int>对象。
template<class T, int size>
bool GCPtr<T, size>::collect()
{
	//若存在未使用指针（内存），memfreed为true。
	bool memfreed = false;

#ifdef DISPLAY
	cout << "before garbage collection for ";
	showlist();
#endif

	list<GCInfo<T>>::iterator p;

	//do
	//{
		//扫描gclist，寻找未使用的指针。
		for (p = gclist.begin(); p != gclist.end(); p++)
		{
			//若正在使用，跳过。
			if (p->refcount > 0) continue;

			//发现未使用指针.
			memfreed = true; 
			
			//若该未使用指针非空指针，则释放其指向的内存。
			if (p->memPtr)
			{
				if (p->isArray)
				{
#ifdef DISPLAY
					cout << "deleting array of size: " << p->arraySize << endl;
#endif
					delete[] p->memPtr; 
				}
				else
				{
#ifdef DISPLAY
					cout << "Deleting: " << *(T *)p->memPtr << endl;
#endif
					delete p->memPtr;
				}
			}
			gclist.remove(*p); 
			
			//gclist.clear();
			break;
		}
	//} while (p != gclist.end());


#ifdef DISPLAY
	cout << "after garbage collection for ";
	showlist();
#endif
	
	return memfreed;
}



//重载赋值运算符=。
//就是GCPtr构造函数GCPtr（T *t）。
//以新地址对GCPtr对象赋值。
template<class T,int size>
T *GCPtr<T, size>::operator =(T *t)
//该重载对应以下情景，
//GCPtr<T,size> gc;  gc = new T();
//即以class GCPtr生成一个新对象gc，同时新地址(new T())传递给*t。
//此时operator =（T *t）被调用以检查新地址t和修改gclist。
{
	list<GCInfo<T>>::iterator p;

	//当赋予新地址时，将GCPtr对象原来指向的内存的计数器减1。
	p = findPtrInfo(addr);
	p->refcount--;

	p = findPtrInfo(t);

	//若新指针的内存已经存在gclist中，则将该内存的计数器加1。
	//否则，在gclist中为新内存开辟一个目录（新加一项）。
	if (p != gclist.end())
		p->refcount++;
	else
	{
		GCInfo<T> gcObj(t, size);
		gclist.push_front(gcObj);
	}

	//为GCPtr对象保存新内存地址。
	//也就是说在赋予新地址的操作时通过该重载运算符完成的。
	addr = t;

	return t;
}



//重载赋值运算符=。
//就是GCPtr构造函数GCPtr（GCPtr &rv）。
//通过GCPtr对象对GCPtr对象的gclist赋值。
template<class T,int size>
GCPtr<T, size> & GCPtr<T, size>::operator = (GCPtr &rv)
{
	list<GCInfo<T>>::iterator p;

	p = findPtrInfo(addr);
	p->refcount--;

	p = findPtrInfo(rv.addr);
	p->refcount++;

	addr = rv.addr;

	return rv;
}



//展示gclist的函数。
template<class T, int size>
void GCPtr<T, size>::showlist()
{
	list<GCInfo<T>>::iterator p;

	cout << "gclist" << "<"<<typeid(T).name() << ", "<< size << ">: \n";

	cout << "memPtr      refcount       value\n";

	if (gclist.begin() == gclist.end())
	{
		cout << "----empty-----\n\n";
		return;
	}

	for (p = gclist.begin(); p != gclist.end(); p++)
	{
		cout << "[" << (void *)p->memPtr << "]"<< "     " << p->refcount << "       ";

		if (p->memPtr)  
			cout << "     " << *p->memPtr;
		else
			cout << "    ----";

		cout << endl;
	}
	cout << endl;
}



//在gclist中找到一个指针。
template<class T, int size>
typename list<GCInfo<T>>::iterator  GCPtr<T, size>::findPtrInfo(T *ptr)
{
	list<GCInfo<T>>::iterator p;

	for (p = gclist.begin(); p != gclist.end(); p++)
	{
		if (p->memPtr == ptr)
			return p;
	}

	//未找到指针，返回一个指向数组终端下一位置的迭代器gclist.end()。
	return p;
}



//程序结束前清理gclist。
//释放所有因环状引用而无法被释放的内存。
template<class T, int size>
void GCPtr<T, size>::shutdown()
{
	//若链表gclist为空，返回。
	if (gclistSize() == 0) return;

	list<GCInfo<T>>::iterator p;

	//将gclist中所有引用计数器设为0。
	for (p = gclist.begin(); p != gclist.end(); p++)
	{
		p->refcount = 0;
	}

#ifdef DISPLAY
	cout << "before collection for shutdown for "
		<< typeid(T).name() << endl;
#endif

	//垃圾回收。
	collect();

#ifdef DISPLAY
	cout << "after collection for shutdown for "
		<< typeid(T).name() << endl;
#endif
}




