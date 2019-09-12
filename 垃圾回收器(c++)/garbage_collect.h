#pragma once
//------------------------------------------------------------------
//һ�����߳��������������ڴ���գ�
//����:
//���ü�������reference counting����
//ÿһ����̬���õ��ڴ棨��̬���ڴ棩����һ����֮��ص����ü�������
//ÿ����һ���Ը��ڴ�����ã���������1��ÿ����һ�����ã���������1��
//��������Ϊ0������ڴ�Ϊʹ�ã�����ɾ���ͷš�
//ȱ�ݣ�
//��״���ã�circle reference����
//------------------------------------------------------------------



#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>
using namespace std;



//����DISPLAY��չʾ�������ջ��
#define DISPLAY



//��Iter����ָ�����ķ�Χʱ���׳����쳣��
//�Զ��塣
class OutOfRangeExc
{
public:
private:
};



//------------------------------------------------------------------
//class Iter�У�
//�����������������ڵ�����ָ�����������STL��
//��Ҫ���ڱ���class GCPtr��ָ�����飬���ṩ�߽��飻
//Iterָ�벻�μӻ�Ӱ���������չ��̣�
//����ͨ��GCPtr�����begin()��end()�������Iter����
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
    //����Iter��ָ������г��ȡ�
	{
		return length;
	}


	T & operator *()
	//���������*��
	//���ص�ǰָ��ptr��ָ���ֵ����reference����
	//������Խ���ȡ��
	{
		if ((ptr >= end) || (ptr < begin))
			throw OutOfRangeExc();
		return *ptr;
	}


	T  *operator ->()
	//���������->��
	//���ص�ǰָ��ptr��ָ��ĵ�ַ��copy����
	//������Խ���ȡ��
	{
		if ((ptr >= end) || (ptr < begin))
			throw OutOfRangeExc();
		return ptr;
	}


	Iter operator ++()
	//���ص�Ŀ�����++��ǰ��ʽ����
	//�����޸ĺ��Iter����copy����ptr��1��end��begin���䡣
	//*this ����ǰclass������ͬ��
	//�ú����������ָ��Խ�磬��ͬ��
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
	//���ص�Ŀ�����++������ʽ����
	//���첢����һ���޸�ǰ��Iter���󣨸�Ч������ͬ��
	//��ǰclass����ptr��1��end��begin���䡣
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
	//���������[]��
	//����ָ��������Ԫ�أ�reference����������Խ���ȡ��
	{
		if ((i < 0) || (i >= (end - begin)))
			throw OutOfRangeExc();
		return ptr[i];
	}

	//����һϵ����Թ�ϵ˫Ŀ�������
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
	//����˫Ŀ�����-��
	//���ط����޸ĺ��Iter����copy����ptr��n��end��begin���䡣
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
	//����˫Ŀ�����-��
	//���ص�ǰclass����������class��������ptrָ���Ԫ�ظ�����
	{
		return ptr - itr2.ptr;
	}


private:
	T * ptr;           //��ǰָ��
	T *end;            //ָ���ն˵���һλ��
	T *begin;          //ָ����������
	unsigned length;   //���г���
};



//------------------------------------------------------------------
//class GCInfo�У�
//memPtr     ��ָ��̬���ڴ��ָ�루��ַ����
//refcount   �Ƕ�̬���ڴ��ʹ�ü�������
//isArray��  ��memPtrָ���������ʱisArrayΪtrue��
//arraySize����isArrayΪtrueʱarraySizeΪ���鳤�ȡ�
//���ԣ�GCInfo����ͽ���̬���ڴ���ʹ�ü�������ϵ������
//------------------------------------------------------------------

template<class T>
class GCInfo
{
public:
	GCInfo(T *mPtr, unsigned size = 0)
	//mPtrִ�з�����ڴ棬�����ڴ���һ�����飬��sizeΪ�����С��
	{
		refcount = 1;
		memPtr = mPtr;

		if (size != 0)
			isArray = true;
		else
			isArray = false;

		arraySize = size;
	}


	unsigned refcount;   //��ǰ�ڴ�����ø�����
	T *memPtr;           //ָ�������ڴ档
	bool isArray;        //��memPtrָ��һ�������ڴ棬Ϊtrue������Ϊfalse��
	unsigned arraySize;  //��isArrayΪtrue����arraySizeΪָ������Ĵ�С��
};



//���������==��
//ʵ��GCInfo����ıȽϡ�
template<class T>
bool operator ==(const GCInfo<T> &ob1,const GCInfo<T> &ob2)
{
	//������GCInfo����ָ����ڴ���ͬʱ����Ϊ������ͬ��
	return (ob1.memPtr == ob2.memPtr);
}



//GCPtrʵ��һ��ָ�����ͣ����������ջ����ͷ�δʹ�õ��ڴ档
//GCPtr����Ĭ��ָ��һ����NEW������ڴ档
//����GCPtr����ָ��һ�η�������飬�����ָ�������С���Ӷ���ȷ�ͷ��ڴ棩��
template<class T,int size = 0>
class GCPtr
{
public:
	//ΪGCPtr<T>����һ�����������͡�
	//����ͨ��GCiterator ����GCPtr�ĵ�������
	typedef  Iter<T>   GCiterator;

	GCPtr(T *t = nullptr)
	//Ĭ��Ϊ��ָ�롣
	{
		//���ǲ����ĵ�һ��GCPtr������shutdown()ע��Ϊ���뺯����
		//���Ѿ���ǳ���һ��GCPtr����������first������Ϊfalse��
		if (first)  
			atexit(shutdown);
		first = false;

		list<GCInfo<T>>::iterator p;
		p = findPtrInfo(t);

		//��t��gclist�ڴ��ڣ����ۼ������ü�������
		//���򣬹���GCInfo���󲢽�����gclist�ڡ�
		if (p != gclist.end())
			p->refcount++;
		else
		{
			GCInfo<T> gcObj(t, size);
			gclist.push_front(gcObj);
		}

		//������ڴ��ַ��ָ�룩��
		addr = t;
		arraySize = size;

		if (size > 0) isArray = true;
		else isArray = false;

#ifdef DISPLAY
		cout << "constructing GCPtr. ";
		if (isArray) 
			cout << "size is�� " << arraySize << "\n";
		else cout << "\n";
#endif
	}

	GCPtr(const GCPtr &ob)
	//����ʽ���졣
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
			count << "size is�� " << arraySize << endl;
		else cout << endl;
#endif
	}

	~GCPtr();

	//GCPtr���о�̬����������������
	//�������ͷ���һ�����󣬷���true��
	static bool collect();

	//���ظ�ֵ�����=��
	//ͨ��ָ���GCPtr��ֵ��
	T *operator = (T *t);

	//���ظ�ֵ�����=��
	//ͨ��GCPtr�����GCPtr��ֵ��
	GCPtr &operator = (GCPtr &rv);

	T &operator *()
	//���������*��
	//����GCPtr����ָ���Ԫ�أ�reference����
	{
		return *addr;
	}

	T *operator ->()
	//���������->��
	//����GCPtr����ָ��ĵ�ַ��copy����
	{
		return addr;
	}

	T &operator [](int i)
	//���������[]��
	//����ָ��������Ԫ�أ�reference����
	{
		return addr[i];
	}

	operator T *()
	//���������*��
	//����GCPtr����ָ��ĵ�ַ��
    //Ҳ���ǽ�GCPtr����ת��Ϊһ����ָͨ�루typename T����
	//������ת����Ӱ���������գ������һ����ָͨ��p�����addr����addrָ����ڴ�ļ������������ӡ�
	//ͬ����ʹһ����ָͨ��ָ���addr���ڴ棬collect������Ȼ�����ͷŸ��ڴ棬��p�ͳ���һ��Ұָ�롣
	{
		return addr;
	}

	Iter<T> begin()
	//����һ��Iter����copy����
	//�ù���ʱΪ�˴���һ��Iter���������Ӷ����Բ���addrָ����ڴ档
	{
		int size;
		if (isArray) size = arraySize;
		else size = 1;

		return Iter<T>(addr, addr, addr + size);
	}

	Iter<T> end()
	//����һ��Iter����copy����
	//�ö���ǰָ��GCPtr��ָ�ڴ���ն˵���һ��ַ�����鳤�Ȳ��䡣
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

	//GCPtr��ľ�̬������������ʾgclist��
	static void showlist();
	
	//GCPtr�ྲ̬�������������ʱ���gclist��
	//�ͷ�������״���ö��޷������ͷŵ��ڴ档
	static void shutdown();


private:
	static list<GCInfo<T>> gclist;    //��̬������һ��GCInfo<T>���͵�list���������̬�ڴ��ָ�롣
	T *addr;                          //GCPtr�����ǰָ����ڴ档
	bool isArray;                     //��GCPtr�����ָ�����飬Ϊtrue������Ϊfalse��
	unsigned arraySize;                    //��isArrayΪtrue����arraySizeΪ�����С��
	static bool first;                //��̬����������һ��GCPtr��������ʱΪtrue��
	                                  //first������ע��һ�����뺯�����������ʱ���������ر�������������
	typename  list<GCInfo<T>>::iterator  findPtrInfo(T *ptr); //����һ��iterator��ָ��gclist��ָ����Ϣ��
};



//ΪGCPtr��ľ�̬����gclist�����ռ䡣
//����template��size�Ͳ���д��Ĭ��ֵ����ͬ��
template<class T, int size>
list<GCInfo<T>> GCPtr<T, size>::gclist;



//ΪGCPtr��ľ�̬����first��ֵ��
template<class T, int size>
bool GCPtr<T, size>::first = true;



//����GCPtr�������������
//ÿ����ָ��Խ��ʱ��ִ������������
template<class T, int size>
GCPtr<T, size>::~GCPtr()
{
	list<GCInfo<T>>::iterator p;

	//�ҵ�����ɾ�����뿪�������GCPtr�����ָ�룬���������ڴ�����ü�������1��
	p = findPtrInfo(addr);
	if (p->refcount) 	p->refcount--;

#ifdef DISPLAY
	cout << "GCPtr going out of scope.\n";
#endif

	//�����������ա�
	collect();
}



//------------------------------------------------------------------
//ʵ�ʵ��У�ֻ����һ������²Ž����������գ�
//��gclist�ﵽһ����С��
//��һ��������GCPtr���󳬳�������
//���ڴ治��ʱ���Ž����������ա�
//------------------------------------------------------------------



//������һ�������ͷţ�����true��
//��Ϊcollect()��GCPtr�ľ�̬���������Կ�����ʱֱ�ӵ����Լ����ͷ�й©���ڴ档
//��GCPtr<int>::collect();�����ͷ�gclist<int>����
template<class T, int size>
bool GCPtr<T, size>::collect()
{
	//������δʹ��ָ�루�ڴ棩��memfreedΪtrue��
	bool memfreed = false;

#ifdef DISPLAY
	cout << "before garbage collection for ";
	showlist();
#endif

	list<GCInfo<T>>::iterator p;

	//do
	//{
		//ɨ��gclist��Ѱ��δʹ�õ�ָ�롣
		for (p = gclist.begin(); p != gclist.end(); p++)
		{
			//������ʹ�ã�������
			if (p->refcount > 0) continue;

			//����δʹ��ָ��.
			memfreed = true; 
			
			//����δʹ��ָ��ǿ�ָ�룬���ͷ���ָ����ڴ档
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



//���ظ�ֵ�����=��
//����GCPtr���캯��GCPtr��T *t����
//���µ�ַ��GCPtr����ֵ��
template<class T,int size>
T *GCPtr<T, size>::operator =(T *t)
//�����ض�Ӧ�����龰��
//GCPtr<T,size> gc;  gc = new T();
//����class GCPtr����һ���¶���gc��ͬʱ�µ�ַ(new T())���ݸ�*t��
//��ʱoperator =��T *t���������Լ���µ�ַt���޸�gclist��
{
	list<GCInfo<T>>::iterator p;

	//�������µ�ַʱ����GCPtr����ԭ��ָ����ڴ�ļ�������1��
	p = findPtrInfo(addr);
	p->refcount--;

	p = findPtrInfo(t);

	//����ָ����ڴ��Ѿ�����gclist�У��򽫸��ڴ�ļ�������1��
	//������gclist��Ϊ���ڴ濪��һ��Ŀ¼���¼�һ���
	if (p != gclist.end())
		p->refcount++;
	else
	{
		GCInfo<T> gcObj(t, size);
		gclist.push_front(gcObj);
	}

	//ΪGCPtr���󱣴����ڴ��ַ��
	//Ҳ����˵�ڸ����µ�ַ�Ĳ���ʱͨ���������������ɵġ�
	addr = t;

	return t;
}



//���ظ�ֵ�����=��
//����GCPtr���캯��GCPtr��GCPtr &rv����
//ͨ��GCPtr�����GCPtr�����gclist��ֵ��
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



//չʾgclist�ĺ�����
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



//��gclist���ҵ�һ��ָ�롣
template<class T, int size>
typename list<GCInfo<T>>::iterator  GCPtr<T, size>::findPtrInfo(T *ptr)
{
	list<GCInfo<T>>::iterator p;

	for (p = gclist.begin(); p != gclist.end(); p++)
	{
		if (p->memPtr == ptr)
			return p;
	}

	//δ�ҵ�ָ�룬����һ��ָ�������ն���һλ�õĵ�����gclist.end()��
	return p;
}



//�������ǰ����gclist��
//�ͷ�������״���ö��޷����ͷŵ��ڴ档
template<class T, int size>
void GCPtr<T, size>::shutdown()
{
	//������gclistΪ�գ����ء�
	if (gclistSize() == 0) return;

	list<GCInfo<T>>::iterator p;

	//��gclist���������ü�������Ϊ0��
	for (p = gclist.begin(); p != gclist.end(); p++)
	{
		p->refcount = 0;
	}

#ifdef DISPLAY
	cout << "before collection for shutdown for "
		<< typeid(T).name() << endl;
#endif

	//�������ա�
	collect();

#ifdef DISPLAY
	cout << "after collection for shutdown for "
		<< typeid(T).name() << endl;
#endif
}




