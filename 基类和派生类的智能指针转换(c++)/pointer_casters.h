#include <iostream>
#include <memory>


//struct Base
//{
//	int a;                                     //结构体的默认权限是public.
//	virtual void f() const { std::cout << "I am base!\n"; }
//	virtual ~Base() {}
//};
//
//struct Derived : Base
//{
//	void d()
//	{
//		std::cout << "Derived:d()\n";
//	}
//	void f() const override
//	{
//		std::cout << "I am derived!\n";
//	}
//	~Derived() {}
//};


class Base
{
public:                                      // 类的默认权限是private.
	virtual ~Base() {}
	virtual void f() const { cout << "I am base!\n"; }
};

class Derived : public Base
{
public:
	~Derived() {}
	void d() {	std::cout << "Derived:d()\n";	}
	void f() const override	{  std::cout << "I am derived!\n";	}
};


int main(int argc, char* argv[]) 
{
	auto basePtr = std::make_shared<Base>();         // 万能auto, 获取共享智能指针.
	cout << "Base pointer says: ";
	basePtr->f();

	auto derivedPtr = std::make_shared<Derived>();
	cout << "Derived pointer says: ";
	derivedPtr->f();

	/*
	static_pointer_cast、dynamic_pointer_cast:
	用于智能指针在基类和派生类间的转换。
	*/


	// 上行的转换（派生类到基类的转换）
	cout << "Derived pointer static_pointer_cast to Base says: ";
	std::static_pointer_cast<Base>(derivedPtr)->f();


	// 下行的转换（基类到派生类的转换)(构造临时 shared_ptr)
	cout << "Base pointer static_pointer_cast to derived says: ";
	std::static_pointer_cast<Derived>(basePtr)->d();
	


	/*
	static_cast和dynamic_cast的区别如下：

	1.基本类型
	1.1 基本类型对象间转换：
	static_cast可以在基本类型间做转换，与隐式转换规则相同，不符合的不能转换。
	dynamic_cast不能再基本类型之间做转换。

	1.2 基本类型指针或引用间转换：
	static_cast不能在基本类型指针或者引用间做转换。
	dynamic_cast不能在基本类型指针或引用间做转换。

	1.3 基本类型指针与void *之间做转换：
	static_cast可以在基本类型指针与void *之间做转换。
	dynamic_cast不能在基本类型指针与void*之间做转换。


	2.类继承之间转换
	2.1 无关类之间互相转换：
	static_cast不能在没有继承关系的类指针或引用之间做转换。
	dynamic_cast也不能够在没有继承关系的类指针或引用之间做转换，否则转换后的指针为空。
	*/


	// 下行的转换（基类到派生类的转换)(直接转型，不构造临时 shared_ptr)
	cout << "Base pointer static_cast to derived says: ";
	static_cast<Derived*>(basePtr.get())->d();
	

	// 下行的转换（基类到派生类的转换)(转换成对象)
	Derived& derived = static_cast<Derived&>(*basePtr);
	cout << "Base pointer static_cast to derived Obeject says: ";
	derived.d();


	// 下行的转换（基类到派生类的转换)
	auto downcastedPtr = std::dynamic_pointer_cast<Derived>(basePtr);
	if (downcastedPtr)							     // 要判断是否转换成功。
	{
		cout << "Base pointer dynamic_pointer_cast to derived says: ";
		downcastedPtr->d();
	}

	// All pointers to derived share ownership
	cout << "Pointers to underlying derived: " << derivedPtr.use_count() << "\n";
}