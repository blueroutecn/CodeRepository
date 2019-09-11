#pragma once
#include<string>
#include<vector>
#include<map>
#include<memory>
#include<windows.h>
#include<list>
#include<iostream>

using std::string;
using std::vector;
using std::map;
using std::auto_ptr;
using std::list;
using std::cout;
using std::endl;



// 适配器模式（adapter model）
class productExist     // adaptee, 适配者， 已存在的类，需要适配其接口为需求的类型。
{
public:
	productExist() {}
	void apiExist(string msg="existed") { cout << msg <<" api\n"; }  // 已存在的接口。
};

class productWant    // interface, 客户端需求。
{
public:
	virtual void apiWant(string msg) = 0;  // 需求的接口。
};

class productAdapter:private productExist, public productWant  // 类适配器，子类可以替换父类(productWant)
{
private:
	void adapterActivity(string &msg) { msg = "wanted"; }  // 适配器动作，实现接口适配。
public:
	productAdapter():productExist(), productWant(){}
	void apiWant(string msg) { adapterActivity(msg); apiExist(msg); } // 通过已存在的接口实现需求接口。
};

class productExist2  // 新的适配者，新的已存在类。
{
public:
	void apiExist(string msg) { cout << msg << " api version2\n"; }
};

class productAdapter2 :private productExist2, public productWant  // 类适配器，对于不同的已存类。
{
private:
	void adapterActivity(string &msg) { msg = "wanted"; }
public:
	productAdapter2():productExist2(), productWant(){}
	void apiWant(string msg) { adapterActivity(msg); apiExist(msg); }
};

class productAdapter3 :public productWant  // 对象适配器，待适配的对象作为成员传入。
{
private:
	productExist& __pe;  // 对象作为类的数据成员。
private:
	void adapterActivity(string &msg) { msg = "wanted"; }
public:
	productAdapter3(productExist &pe):__pe(pe), productWant(){}
	void apiWant(string msg) { adapterActivity(msg); __pe.apiExist(msg); }  // 调用已存对象的接口。
};

class productAdapter4 :public productWant  // 对象适配器，对于不同的已存对象。
{
private:
	productExist2 & __pe;  
	void adapterActivity(string &msg) { msg = "wanted"; }
public:
	productAdapter4(productExist2 &pe) :__pe(pe), productWant() {}
	void apiWant(string msg) { adapterActivity(msg); __pe.apiExist(msg); }  
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	productExist* pe = new productExist();
//	pe->apiExist("exist");
//
//	productWant* pw = new productAdapter();
//	pw->apiWant("exist");
//
//	productExist2* pe2 = new productExist2();
//	pe2->apiExist("exist");
//
//	productAdapter2* pw2 = new productAdapter2();
//	pw2->apiWant("exist");
//
//	productAdapter3* pw3 = new productAdapter3(*pe);
//	pw3->apiWant("exist");
//
//	productAdapter4* pw4 = new productAdapter4(*pe2);
//	pw4->apiWant("exist");
//
//	system("pause");
//	return 0;
//}



// 装饰器模式(decorator model)
class Car  // 抽象基类
{
public:
	virtual void features() = 0;  // 基本功能(接口)。
};

class superCar:public Car  // 具体对象类。
{
	void features() { cout << "This is superCar.\n"; }
};

class decoratorCar : public Car  // 抽象装饰类，继承抽象基类，以获得相应接口。
{
protected:
	Car* __car; 
public:
	decoratorCar(Car* car):__car(car), Car(){}
	void features() { __car->features(); addFeature(); }  // 在基类接口的基础上增加功能。
	virtual void addFeature() = 0;  // 装饰器的新增功能。
};

class runCar : public decoratorCar  // 实现装饰器（动态加载新功能）
{
public:
	runCar(Car* car):decoratorCar(car){}
	void addFeature() { cout << "can run now.\n"; }
};

class swimmCar :public decoratorCar
{
public:
	swimmCar(Car* car) : decoratorCar(car){}
	void addFeature() { cout << "can swimm now.\n"; }
};

class flyCar : public decoratorCar
{
public:
	flyCar(Car* car): decoratorCar(car){}
	void addFeature() { cout << "can fly now.\n"; }
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	Car* car = new superCar();
//
//	decoratorCar* rcar = new runCar(car);
//	decoratorCar* scar = new swimmCar(car);
//	decoratorCar* fcar = new flyCar(car);
//
//	car = rcar;
//	car->features();
//
//	car = scar;
//	car->features();
//
//	car = fcar;
//	car->features();
//
//
//	system("pause");
//	return 0;
//}



// 组合模式（compsite model）
class component  // 抽象统一构件， 提供一致的访问性。
{
private:
	string __name;
public:
	component(string name):__name(name){}
	virtual ~component(){}   // 虚析构函数。

	virtual void operation() = 0;
	string getName() { return __name; }
	virtual void add(component*) = 0;       // 添加构件。
	virtual void remove(component*) = 0;    // 删除构件。
	virtual component* getChild(int) = 0;   // 获取子构件。
};

class leaf :public component  // 叶子构件，无子构件。
{
public:
	leaf(string name):component(name){}
	virtual ~leaf(){}

	void operation(){ cout << "Leaf " << getName() << " Operation.\n"; }
	void add(component* pcom){}   //叶子构件，空操作。
	void remove(component* pcom){}
	component* getChild(int index) { return nullptr; }  // 无子构件。
};

class composite :public component  // 组合器， 树枝构件。
{
private:
	vector<component*> __vecComp;
public:
	composite(string name):component(name){}
	virtual ~composite()
	{
		vector<component*>::iterator it = __vecComp.begin();
		while (it != __vecComp.end())
		{
			if (*it != nullptr)
			{
				cout << "composite " << getName() << " delete:" << (*it)->getName() << endl;
				delete *it;    // 删除该节点构件指针。
				*it = nullptr; // 节点构件为空。
			}
			__vecComp.erase(it); // 删除该节点构件。
			it = __vecComp.begin(); // 更新迭代器。
		}
	}

	void operation()
	{
		cout << "composite " << getName() << " Operation.\n";
	}

	void add(component* pcom)
	{
		__vecComp.push_back(pcom);
	}

	void remove(component* pcom)
	{
		vector<component*>::iterator it = __vecComp.begin();
		for (; it != __vecComp.end(); ++it)
		{
			if ((*it)->getName() == pcom->getName())
			{
				if (*it != nullptr)
				{
					delete *it;
					*it = nullptr;
				}
				__vecComp.erase(it);
				break;
			}
		}
	}

	component* getChild(int index)
	{
		if (index > __vecComp.size())
		{
			return nullptr;
		}
		return __vecComp[index - 1];
	}
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	component *pcompositeA = new composite("compositeA");
//	component *pcompositeB = new composite("compositeB");
//
//	component *pLeafA = new composite("LeafA");
//	component *pLeafB = new composite("LeafB");
//
//	pcompositeA->add(pLeafA);
//	pcompositeB->add(pLeafB);
//	pcompositeA->add(pcompositeB);
//
//	pcompositeB->operation();
//	pcompositeA->remove(pLeafA);
//
//	system("pause");
//	return 0;
//}



// 外观模式（facade model）
class LightSystem  // 子系统类。
{
public:
	void on(){cout << "开灯..." << endl;}
	void off(){	cout << "关灯..." << endl;}
};

class MovieSystem
{
public:
	void on(){cout << "放电影..." << endl;}
	void off(){	cout << "关闭电影..." << endl;}
};

class SounderSystem
{
public:
	void on(){ cout << "打开音响..." << endl;}
	void off(){	cout << "关闭音响..." << endl;}
};

class MusicSystem
{
public:
	void on(){ cout << "放音乐..." << endl;	}
	void off(){	cout << "关闭音乐..." << endl;}
};

class HomeSystem  //外观模式类，智能生活，给用户提供简洁的界面
{
public:	
	void OpenKTV()  //KTV模式...
	{
		light.off();
		sounder.on();
		music.on();
		cout << "*********KTV模式********" << endl;
	}

	void CloseKTV()
	{
		light.on();
		sounder.off();
		music.off();
		cout << "**********KTV模式 已经关闭********" << endl;
	}
	
	void OpenMovie()  //观影模式...
	{
		light.off();
		sounder.on();
		sounder.on();
		cout << "*********家庭影院模式********" << endl;
	}
	void CloseMovie()
	{
		light.on();
		sounder.off();
		sounder.off();
		cout << "*********家庭影院模式 已经关闭********" << endl;
	}

private:
	LightSystem light;
	MovieSystem movie;
	MusicSystem music;
	SounderSystem sounder;
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	HomeSystem home;
//	home.OpenKTV();//开启KTV模式
//	home.CloseKTV();
//
//	home.OpenMovie();//开启电影模式
//	home.CloseMovie();
//
//
//	system("pause");
//	return 0;
//}



// 代理模式（proxy model）
class iFactory   // 统一抽象接口类。
{
public:
	virtual void makeProduct() = 0;
};

class phoneFactory : public iFactory  // 真实类。
{
public:
	void makeProduct() { cout << "make phone.\n"; }
};

class proxyFactory : public iFactory  // 代理类，在真实对象和调用对象间设置隔离。
{
private:
	iFactory * __realObj;
public:
	proxyFactory(iFactory* fac):__realObj(fac){}
	void operation() { cout << "some other opertion.\n"; }
	void makeProduct() { __realObj->makeProduct(); }
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	iFactory* factory = new phoneFactory();
//	proxyFactory* proxy = new proxyFactory(factory);
//	proxy->makeProduct();
//
//
//	system("pause");
//	return 0;
//}



// 享元模式（flyweight model）
class iPlayer  // 创建抽象享元类。
{
public:
	virtual ~iPlayer(){}
	virtual void assignWeapon(string weapon) = 0;
	virtual void mission() = 0;

protected:
	string __task;   // 内部状态。
	string __weapon; // 外部状态。
};

class terrorist : public iPlayer  // 具体享元。
{
public:
	terrorist() { __task = "plant a bomb"; }

	virtual void assignWeapon(string weapon) override 
	{ __weapon = weapon; }  // override,强制重载基类虚函数。
	virtual void mission() override 
	{ cout << "terrorist with " << __weapon << ", " << "task is " << __task << endl; }
};

class counterTerrorist : public iPlayer
{
public:
	counterTerrorist() { __task = "diffuse bomb"; }

	virtual void assignWeapon(string weapon) override
	{	__weapon = weapon;	}
	virtual void mission() override
	{ cout << "counter terrorist with " << __weapon << ", " << "task is " << __task << endl; }
};

class playerFactory  // 享元工厂。
{
public:
	static iPlayer* getPlayer(string type)  // 静态函数。
	{
		iPlayer* p = nullptr;

		// 如果 T / CT 对象存在，则直接从享元池获取；否则，创建一个新对象并添加到享元池中，然后返回。
		if (__map.find(type) != __map.end())
		{
			p = __map[type];
		}
		else
		{
			if (type == "T")
			{
				cout << "terrorist created.\n";
				p = new terrorist();
			}
			else if (type == "CT")
			{
				cout << "counter terrorist created.\n";
				p = new counterTerrorist();
			}
			else
			{
				cout << "unreachable code!\n";
			}

			__map.insert(std::make_pair(type, p)); // 一旦创建，将其插入map中；make_pair, 将两个数据合成一个元组。
		}
		return p;
	}

private:
	static map<string, iPlayer*>  __map;  // 静态数据成员，享元池。
public:
	static string __playerType[];  // 玩家类型。
	static string __weapons[];     // 武器类型。
};


// 使用流程。
//// 享元池初始化为空, 类静态对象单独初始化。
//map<std::string, iPlayer*> playerFactory::__map = map<string, iPlayer*>();
//// 玩家类型初始化。
//string playerFactory::__playerType[2] = { "T", "CT" };
//// 玩家武器初始化。
//string playerFactory::__weapons[4] = { "AK-47", "Maverick", "Gut Knife", "Desert Eagle" };
//
//int main(int argc, char* argv[])
//{
//	//int playerLen = playerFactory::__playerType->size()/sizeof(string); wrong.
//	//int weaponsLen = playerFactory::__weapons->size()/sizeof(string);
//
//	int playerLen = 2, weaponsLen = 4;
//
//	// 假设，游戏中有十位玩家
//	for (int i = 0; i < 10; i++)
//	{
//		// 获取随机玩家和武器
//		int typeIndex = rand() % playerLen;
//		int weaponIndex = rand() % weaponsLen;
//		string type = playerFactory::__playerType[typeIndex];
//		string weapon = playerFactory::__weapons[weaponIndex];
//
//		// 获取玩家
//		iPlayer *p = playerFactory::getPlayer(type);
//
//		// 从武器库中随机分配武器
//		p->assignWeapon(weapon);
//
//		// 派玩家去执行任务
//		p->mission();
//	}
//
//
//	system("pause");
//	return 0;
//}



// 桥接模式（bridge model）
class Abstraction           // 接口抽象基类，声明该类所需的接口(接口端抽象)。
{
public:
	virtual ~Abstraction() {};
	virtual void Operation() = 0;  // 声明接口，表示该类所支持的操作；抽象基类。

protected:
	Abstraction() {};              // 只允许在派生类中生成。
};

class AbstractionImplement  //实现抽象基类，定义了接口实现（实现端抽象）。
{
public:
	virtual ~AbstractionImplement() {};
	virtual void work() = 0;    // 定义实现接口（实现接口与Abstraction的接口不用相同）。

protected:
	AbstractionImplement() {};
};

class RefinedAbstractionA :public Abstraction   // 扩充Abstraction定义的接口。
{
public:
	RefinedAbstractionA(AbstractionImplement* imp):__imp(imp){}
	virtual ~RefinedAbstractionA() { delete __imp; __imp = nullptr;}
	virtual void Operation()  override  // 调用AbstractionImplement对象，并拓展Abstraction接口。
	{cout << "refined abstractionA operation: "; __imp->work();}  // 这里调用__imp的成员，所以__imp的类必须在此之前定义。   

private:
	AbstractionImplement* __imp; // 维护一个指向AbstractionImplement的指针。
};

class RefinedAbstractionB :public Abstraction
{
public:
	RefinedAbstractionB(AbstractionImplement* imp):__imp(imp){}
	virtual ~RefinedAbstractionB() { delete __imp; __imp = nullptr; }
	virtual void Operation() override
	{ cout << "refinded abstractionB operation: "; __imp->work(); }

private:
	AbstractionImplement* __imp;
};

class ConcreteAbstractionImplementA :public AbstractionImplement  // 具体实现类。
{
public:
	ConcreteAbstractionImplementA() {};
	~ConcreteAbstractionImplementA() {};
	void work()  override { cout << "concrete abstraction implementA work.\n"; }  
};

class ConcreteAbstractionImplementB :public AbstractionImplement
{
public:
	ConcreteAbstractionImplementB() {};
	~ConcreteAbstractionImplementB() {};
	void work() override { cout << "concrete abstraction implementB work.\n"; }
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	AbstractionImplement* imp = new ConcreteAbstractionImplementA();
//	Abstraction* abs = new RefinedAbstractionA(imp);
//	abs->Operation();
//
//	imp = new ConcreteAbstractionImplementB();  // 实现方式。
//	abs = new RefinedAbstractionA(imp);         // 接口类型。
//	abs->Operation();                           // 接口实现。
//
//	imp = new ConcreteAbstractionImplementA();
//	abs = new RefinedAbstractionB(imp);
//	abs->Operation();
//
//	imp = new ConcreteAbstractionImplementB();
//	abs = new RefinedAbstractionB(imp);
//	abs->Operation();
//
//	system("pause");
//	return 0;
//}



// 单例模式（singleton model）
class Singleton  //简单模式：懒汉模式，需要实例时才创建。
{
public:
	static Singleton * GetInstance()// 静态函数，全类唯一。
	{
		if (!__pInstance) __pInstance = new Singleton();
		__count++;
		return __pInstance;
	}

	void display() { cout << "singleton has been called " << __count << "times.\n"; }

	~Singleton() {}

private:
	static int __count;
	static Singleton* __pInstance;  // 维护一个唯一指向singleton对象的静态指针。

private:     // 封闭任何外部创建实例的方式。
	Singleton() {}
	Singleton(const Singleton&) {}
	Singleton& operator = (const Singleton&) {};
};


/*
-------------------------------------------------------
问题：
1、需要手动释放__pInstance(方案一、二、三)；
2、如果在多线程环境不安全（方案四、方案五)。
-------------------------------------------------------
*/


// 方案一：注册退出函数atexit(RealesSingleton)。
// int atexit(void(*function)(void)); 注册参数为无参数无返回的函数指针；调用和注册顺序相反. 
void RealesSingleton()
{
	delete Singleton::GetInstance();
	cout << "singleton has been deleted.\n";
}


// 方案二：使用智能指针auto_ptr<Singleton> m_pInstance。


// 方案三：使用内嵌类和一个静态成员。
// 程序结束时会析构所有全局变量，包括类的静态成员。以此，在类内设一个私有静态成员负责指针析构。
class Singleton2
{
public:
	static Singleton2 * GetInstance()
	{
		if (__pInstance.get() == nullptr) __pInstance.reset(new Singleton2);  // 智能指针的使用（类）。
		__count++;
		return __pInstance.get();
	}

	void display() { cout << "singleton2 has been called " << __count << " times.\n"; }

	~Singleton2() {}  // 这里的虚构函数没有操作。

private:
	static int __count;
	static auto_ptr<Singleton2> __pInstance;  // 采用智能指针保存singleton对象。

private:
	class Clear  // 内嵌类, 如果体外定义Singleton2::Clear::~Clear()。
	{
	public:
		~Clear()
		{
			__pInstance.release();   // clear对象析构时，释放__pInstance. 
			cout << "singleton2 has been deleted.\n";
		}
	};
	static Clear __clear;  // 静态成员，需要初始化（即总会存在一个clear对象，析构时调用~clear）。

private:
	Singleton2() {}
	Singleton2(const Singleton2&) {}
	Singleton2& operator = (const Singleton2&) {};
};


// 方案四：双监测锁机制。
class LockGuard
{
public:
	LockGuard() { InitializeCriticalSection(&__cs); }  // 初始化临界区变量。
	~LockGuard() { DeleteCriticalSection(&__cs); }     // 释放临界区变量。

private:
	void Guard() { EnterCriticalSection(&__cs); }    // 判断是否能加入当前临界区。
	void unGuard() { LeaveCriticalSection(&__cs); }  // 释放资源函数，离开临界区。

private:
	CRITICAL_SECTION __cs;  // 多线程同时访问的资源（临界区）

public:
	class CGuard  // 干嘛的？
	{
	public:
		CGuard(LockGuard &lg) :__lg(lg) { __lg.Guard(); }
		~CGuard() { __lg.unGuard(); }
	private:
		LockGuard & __lg;
	};
};

class Singleton3
{
public:
	~Singleton3() {}
	void display() 
	{ cout << "singleton3 has been called " << __count << " times.\n"; }
	static Singleton3* GetInstance()
	{
		if (__pInstance == nullptr)
		{
			LockGuard::CGuard gd(__lg); // 创建一个临时CGuard对象，判断是否可以进入，离开时析构释放。
			if (__pInstance == nullptr) __pInstance = new Singleton3();
		}
		__count++;
		return __pInstance;
	}

private:
	static int __count;
	static LockGuard __lg;  // 锁， 用于判断是否可以进入临界区（唯一singleton）。
	static Singleton3 * __pInstance;

private:
	class Clear
	{
	public:
		~Clear()
		{
			delete __pInstance;   
			cout << "singleton3 has been deleted.\n";
		}
	};
	static Clear __cl;

private:
	Singleton3() {}
	Singleton3(const Singleton3&) {}
	Singleton3& operator = (const Singleton3&) {};
};


// 方案五：饿汉模式，不管是否会用到，程序开始即创建实例,即采用静态初始化保证线程安全。
class Singleton4
{
public:
	static Singleton4 * GetInstance() 
	{
		__count++;
		return __pInstance;
	}
	void display() 
	{ cout << "Singleton4 has been called " << __count << " times.\n"; }

	~Singleton4() {}

private:
	static int __count;
	static Singleton4 * __pInstance;

private:
	class Clear
	{
	public:
		~Clear()
		{
			delete __pInstance;
			__pInstance = nullptr;
			cout << "singleton4 has been deleted.\n";
		}
	};
	static Clear __clear;

private:
	Singleton4() {}
	Singleton4(const Singleton4&) {}
	Singleton4& operator = (const Singleton4&) {};
};


// 使用流程。
//int Singleton::__count = 0;
//Singleton* Singleton::__pInstance = nullptr;
//
//int Singleton2::__count = 0;
//auto_ptr<Singleton2> Singleton2::__pInstance; // 智能指针不需要nullptr初始化。
//Singleton2::Clear Singleton2::__clear;   // 静态内嵌类对象初始化(总会有一个对象)。
//
//int Singleton3::__count = 0;
//LockGuard Singleton3::__lg;
//Singleton3* Singleton3::__pInstance = nullptr;
//Singleton3::Clear Singleton3::__cl;
//
//int Singleton4::__count = 0;
//Singleton4* Singleton4::__pInstance = new Singleton4(); // 程序开始即产生一个实例。
//Singleton4::Clear Singleton4::__clear;
//
//int main(int argc, char* argv[])
//{
//	atexit(RealesSingleton);         // 方案一，注册退出时执行函数。
//	Singleton* p = nullptr;
//	p = Singleton::GetInstance();
//	p->display();
//	p = Singleton::GetInstance();
//	p = Singleton::GetInstance();
//	p->display();
//
//
//	Singleton2* p2 = nullptr;         // 方案三，使用内嵌类和其静态成员。
//	p2 = Singleton2::GetInstance();   // 方案二，使用智能指针。
//	p2->display();
//	p2 = Singleton2::GetInstance();
//	p2 = Singleton2::GetInstance();
//	p2->display();
//
//
//	Singleton3* p3 = nullptr;         // 方案四，使用双监测锁机制。
//	p3 = Singleton3::GetInstance();
//	p3->display();
//	p3 = Singleton3::GetInstance();
//	p3 = Singleton3::GetInstance();
//	p3->display();
//
//
//	Singleton4* p4 = nullptr;         // 方案五，使用饿汉模式。
//	p4 = Singleton4::GetInstance();
//	p4->display();
//	p4 = Singleton4::GetInstance();
//	p4 = Singleton4::GetInstance();
//	p4->display();
//
//  这里的析构顺序是singleton1，4，3，2。
//	//system("pause");  
//	return 0;
//}



// 简单工程模式（simple factory model）
class IceCream // 抽象出一个公共接口，作为纯虚函数。
{
public:
	virtual void taste() { cout << "can't find your teste icecream.\n"; }
};

class AppleIceCream :public IceCream // 抽象类实例化，一个类负责一个单一功能。
{
public:
	void taste() { cout << "this is apple taste icecream.\n"; }
};

class BananaIceCream :public IceCream
{
public:
	void taste() { cout << "this is banana taste icecream.\n"; }
};

class OrangeIceCream :public IceCream
{
public:
	void taste() { cout << "this is orange taste icecream.\n"; }
};

enum TASTE { Apple, Banana, Orange, Peach };  // 各种口味。

class IceCreamFactory // 工厂类,由工厂生产产品，产品种类的变化会导致工厂的变化。
{
public:
	IceCream * create_ice_cream(TASTE taste)
	{
		IceCream* _iceCream = nullptr;

		switch (taste)  // 口味（实现）的改变，工厂类也要改变。
		{
		case Apple:_iceCream = new AppleIceCream(); break;
		case Banana:_iceCream = new BananaIceCream(); break;
		case Orange:_iceCream = new OrangeIceCream(); break;
		default: _iceCream = new IceCream();
		}

		return _iceCream;
	}
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	TASTE taste;
//	IceCream* ice;
//	IceCreamFactory iceFac;
//
//	taste = Apple;
//	ice = iceFac.create_ice_cream(taste);
//	ice->taste();
//	taste = Banana;
//	ice = iceFac.create_ice_cream(taste);
//	ice->taste();
//	taste = Orange;
//	ice = iceFac.create_ice_cream(taste);
//	ice->taste();
//
//
//	return 0;
//}



// 工厂模式（factory model）
namespace fm    // 设定命名空间，避免函数、变量类型的重名冲突。
{
	class IceCream  // 抽象产品类。
	{
	public:
		virtual void taste() { cout << "can't find your taste.\n"; }
	};

	class AppleIceCream :public IceCream
	{
	public:
		void taste() { cout << "apple taste.\n"; }
	};

	class BananaIceCream :public IceCream
	{
	public:
		void taste() { cout << "banana taste.\n"; }
	};

	class OrangeIceCream :public IceCream
	{
	public:
		void taste() { cout << "orange taste.\n"; }
	};	

	class IceCreamFactory    // 抽象工厂类，选择具体产品的生产工厂。
	{
	public:
		virtual IceCream * createIceCream() = 0;  
	};

	class AppleIceCreamFactory :public IceCreamFactory  // 具体工厂类，生产一类产品。
	{
	public:
		IceCream * createIceCream() { return new AppleIceCream(); } // 生成具体产品，返回抽象产品类型。
	};

	class OrangeIceCreamFactory :public IceCreamFactory
	{
	public:
		IceCream * createIceCream() { return new OrangeIceCream(); }
	};

	class BananaIceCreamFactory :public IceCreamFactory
	{
	public:
		IceCream * createIceCream() { return new BananaIceCream(); }
	};
}


// 使用流程。
//int main(int argc, char* argv[])
//{
//	fm::IceCream* ice;
//	fm::IceCreamFactory* iceFac;
//
//	iceFac = new fm::AppleIceCreamFactory();
//	ice = iceFac->createIceCream();
//	ice->taste();
//
//	delete iceFac;  // 删除指针所指对象。
//	iceFac = new fm::BananaIceCreamFactory();
//	ice = iceFac->createIceCream();
//	ice->taste();
//
//	return 0;
//}



// 抽象工厂模式（abstract factory model）
class phone  // 产品抽象类。
{
public:
	virtual ~phone() {};
	virtual void call(string num) = 0;
};

class androidPhone : public phone  // 产品类。
{
public:
	void call(string num) { cout << "android phone is calling "<<num<<endl; }
};

class iosPhone : public phone
{
public:
	void call(string num) { cout << "ios phone is calling "<<num<<endl; }
};

class pad
{
public:
	virtual ~pad(){}
	virtual void play() = 0;
};

class androidPad : public pad
{
public:
	void play() { cout << "android pad is playing.\n"; }
};

class iosPad : public pad
{
public:
	void play() { cout << "ios pad is playing.\n"; }
};

class mobileFactory  // 抽象工厂类，抽象工厂生产一个产品族（多类产品），生产没有顺序要求。
{
public:
	virtual ~mobileFactory(){}
	virtual phone* createPhone() = 0;  // 生产手机产品(产品族)
	virtual pad* createPad() = 0;      // 生产平板产品(产品族)
};

class androidFactory : public mobileFactory  // 具体工厂。
{
public:
	phone * createPhone() { return new androidPhone(); }
	pad* createPad() { return new androidPad(); }
};

class iosFactory :public mobileFactory
{
public:
	phone * createPhone() { return new iosPhone(); }
	pad* createPad() { return new iosPad(); }
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	phone* ph;
//	pad* pa;
//	mobileFactory* mf;
//
//	mf = new androidFactory();
//	ph = mf->createPhone();
//	pa = mf->createPad();
//	ph->call("110");
//	pa->play();
//
//	delete mf;
//	delete ph;
//	delete pa;
//	mf = new iosFactory();
//	ph = mf->createPhone();
//	pa = mf->createPad();
//	ph->call("911");
//	pa->play();
//
//	delete mf;
//	delete ph;
//	delete pa;
//
//	return 0;
//}



// 建造者模式（builder model）
class Builder  // 抽象建造者类，给出产品各组分建造步骤的接口。
{
public:
	virtual ~Builder() {}
	virtual void BuildPartA() = 0;
	virtual void BuildPartB() = 0;
};

class ConcreteBuilder1 :public Builder  // 具体建造者。
{
public:
	virtual ~ConcreteBuilder1() {}
	virtual void BuildPartA() { cout << "concrete builder1: step1, build partA.\n"; }
	virtual void BuildPartB() { cout << "concrete builder1: step2, build partB.\n"; }
};

class ConcreteBuilder2 :public Builder
{
public:
	virtual ~ConcreteBuilder2() {}
	virtual void BuildPartA() { cout << "concrete builder2: step1, build partA.\n"; }
	virtual void BuildPartB() { cout << "concrete builder2: step2, build partB.\n"; }

private:
};

class Director  // 指挥者，调用建造者接口组装产品。
{
public:
	Director(Builder* pBuilder):__pBuilder(pBuilder){}
	~Director() { delete __pBuilder; __pBuilder = nullptr; }
	void construct() { __pBuilder->BuildPartA(); __pBuilder->BuildPartB(); } // 按顺序组装产品。

private:
	Builder * __pBuilder;  // 维护一个建造者对象指针。
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	Builder* b;
//	Director* d;
//
//	b = new ConcreteBuilder1();
//	d = new Director(b);
//	d->construct();
//
//	delete b;  // 这里不需要delete d；因为d指向的Director对象析构会销毁指针。
//	b = new ConcreteBuilder2();
//	d = new Director(b);
//	d->construct();
//
//	delete b;
//
//	return 0;
//}



// 原型模式（prototype model）
class Prototype  // 抽象原型类。
{
public:
	virtual ~Prototype(){}
	virtual Prototype* Clone() const { return nullptr; }

protected:    
	Prototype(){}
};

class ConcretePrototype : public Prototype  // 具体原型。
{
public:
	ConcretePrototype() { cout << "prototype is created.\n"; }
	ConcretePrototype(const Prototype& cp) { cout << "prototype is created.\n"; }
	~ConcretePrototype() { cout << "prototype is deleted.\n"; }

	Prototype* Clone() const override
	{ cout << "prototype is cloned.\n"; return new ConcretePrototype(*this); } // 复制自己。
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	Prototype* p = new ConcretePrototype();
//	Prototype* p2 = p->Clone();
//	Prototype* p3 = p->Clone();
//
//	delete p;
//	p = nullptr;
//
//	if (p2)  // 删除p2指向的对象。
//	{
//		delete p2;
//		p2 = nullptr;
//	}
//
//	if (p3)  // 删除p3指向的对象。
//	{
//		delete p3;
//		p3 = nullptr;
//	}
//
//	return 0;
//}



// 模版方法模式（template method）
class testPaper  // 算法模版抽象类，定义了算法的整体框架和流程。
{
public:
	void doTest() { name(); oneAsk(); twoAsk(); }  // 整体算法框架和流程。
	void oneAsk() { cout << "question one：  "; answerOne(); }  // 主体算法步骤。
	void twoAsk() { cout << "question two:   "; answerTwo(); }

	virtual void answerOne() = 0;  // 部分算法实现, 留待子类实现。
	virtual void answerTwo() = 0;
	virtual void name() = 0;
};

class studentA : public testPaper
{
public:
	void name() override { cout << "student A.\n"; }
	void answerOne() override { cout << "i can't work it out.\n"; }
	void answerTwo() override { cout << "pass, i give up.\n\n"; }
};

class studentB : public testPaper
{
public:
	void name() override { cout << "student B.\n"; }
	void answerOne() override { cout << "it's easy.\n"; }
	void answerTwo() override { cout << "i can answer this.\n"; }
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	testPaper* p;
//
//	p = new studentA();
//	p->doTest();
//
//	delete p;
//	p = new studentB();
//	p->doTest();
//
//	if (p)
//	{
//		delete p;
//		p = nullptr;
//	}
//
//	return 0;
//}



// 中介者模式（mediator model）
extern class colleague;

class mediator   // 抽象中介者。
{
public:
	virtual void registerColleague(colleague* coll)   // 将参与者注册到中介者。
	{ __colleagues.emplace_back(coll); }  // emplace_back 替代push_back, 插入时无临时变量，效率更高。
	
	const list<colleague*>& getColleagues() const     
	{ return __colleagues; }  // 常函数不修改成员变量，返回参与者指针常列表的引用（不允许修改）。

	virtual void sendMessage(const colleague* sender, const string& msg) const = 0; // 中介者抽象接口，将发送者消息发送给所有参与者。

private:
	list<colleague*>  __colleagues;  // 维持一个参与者的指针列表。
};

class colleague  // 抽象参与者。
{
public:
	colleague(const string& name):__name(name){}

	string getName() const { return __name; }

	virtual void sendMessage(const mediator& md, const string& msg) const = 0; // 通过中介者发送信息。

	virtual void getMessage(const colleague* co, const string& msg) const = 0;  // 接收发送者的信息。

protected: 
	string __name;
};

class concreteMediator : public mediator     // 具体中介者。
{
public:
	void sendMessage(const colleague* sender, const string& msg) const override  // 通信方式。
	{
		for (const colleague* c : getColleagues())  // 新循环方式。
		{
			if (c != sender)  // colleague可以提前声明，但是当调用成员时其必须已定义。
				c->getMessage(sender, msg); // 父类公共、子类私有，父类指针指向子类实例，结果调用子类私有成员。
		}
	}
};

class concreteColleague : public colleague   // 具体参与者。
{
public:
	concreteColleague(const string& name):colleague(name) {}

	void sendMessage(const mediator& md, const string& msg) const override
	{
		md.sendMessage(this, msg);  // this, 当前类的自指指针。
	}

private:
	void getMessage(const colleague* sender, const string& msg) const override   // 通过父类指针多态调用。
	{
		cout << __name << " get the message from " << sender->getName() << ": " << msg << endl;
	}
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	colleague *landlord = new concreteColleague("Tom");	// 房东
//
//	colleague *jerry = new concreteColleague("Jerry");	// 租客
//	colleague *tuffy = new concreteColleague("Tuffy");
//
//	concreteMediator mediator; // 中介者 - 添加租客
//	mediator.registerColleague(jerry);
//	mediator.registerColleague(tuffy);
//	mediator.registerColleague(landlord);
//
//	// 通过中介将消息发送出去
//	landlord->sendMessage(mediator, "Xi'erqi, two bedroom house, 6000/month.");
//	jerry->sendMessage(mediator, "want one bedroom house.");
//	tuffy->getMessage(jerry, "want to rent house with you.");  // 变态c++。
//
//	return 0;
//}




// 策略模式（strategy model）
#define free_ptr(p) if(p) delete p; p = nullptr  

class wind  // 抽象策略类。
{
public:
	virtual ~wind() {}
	virtual void blow() = 0;  // 统一接口。
};

class coldWind : public wind  // 具体算法类。
{
public:
	void blow() override { cout << "blow cold wind.\n"; }
};

class warmWind :public wind
{
public:
	void blow() override { cout << "blow warm wind.\n"; }
};

class noWind :public wind
{
public:
	void blow() override { cout << "blow no wind.\n"; }
};

class windMode  // 客户端接口类。
{
public:
	windMode(wind* wind):__wind(wind){}
	~windMode() { free_ptr(__wind); }
	void blowWind() { __wind->blow(); }  // 客户端接口，访问策略。

private:
	wind * __wind;  // 维护一个抽象策略类的指针。
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	wind* w;
//	windMode* wm;
//
//	w = new coldWind();
//	wm = new windMode(w);
//	wm->blowWind();
//
//	free_ptr(w);
//	w = new warmWind();
//	wm = new windMode(w);
//	wm->blowWind();
//
//	free_ptr(w);
//	w = new noWind();
//	wm = new windMode(w);
//	wm->blowWind();
//
//	free_ptr(w);
//
//	return 0;
//}




// 访问者模式（visitor model）
extern class parkA;
extern class parkB;

class visitor  // 抽象访问者。
{
public:
	virtual ~visitor(){}
	virtual void visit(parkA*) = 0;  // 访问对象接口。
	virtual void visit(parkB*) = 0;
};

class tourist : public visitor  // 具体的访问者，访问对象不变，具有特定的访问操作。
{
public:
	void visit(parkA*) override { cout << "tourist visit parkA.\n"; }  // 访问操作。
	void visit(parkB*) override { cout << "tourist visit parkB.\n"; }
};

class cleaner : public visitor
{
public:
	void visit(parkA*) override { cout << "cleaner clean parkA.\n"; }
	void visit(parkB*) override { cout << "cleaner clean parkB.\n"; }
};

class place  // 抽象元素类，为元素统一访问者接口。
{
public:
	virtual ~place(){}
	virtual void accept(visitor*) = 0;  // 元素接受哪些访问者。
};

class parkA :public place
{
public:
	void accept(visitor* v) override 
	{ cout << "parkA accept visitor.\n"; v->visit(this); }  // 通过在元素中调用相应访问者实现访问。
};

class parkB : public place
{
public:
	void accept(visitor* v) override
	{ cout << "parkB accept visitor.\n"; v->visit(this); }
};

class city  // 客户端。
{
public:
	~city() { for (auto* p : __places) { free_ptr(p); } } // 释放指针指向的空间，之后由list释放指针元素。

	void login(place* p) { __places.emplace_back(p); }  // 注入元素。

	void logout(place* p) { __places.remove(p); }       // 移除元素。

	void operation(visitor* v) { for (place* p : __places) p->accept(v); }  // 客户端封装。

private:
	list<place*>  __places;  // 维护一个元素指针列表，后期需要释放指向的内存（之前的模式未处理）。
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	place* p;
//	visitor* v;
//	city c;
//
//	p = new parkA();
//	c.login(p);
//	p = new parkB();
//	c.login(p);
//
//	v = new tourist();
//	c.operation(v);
//
//	free_ptr(v);
//	v = new cleaner();
//	c.operation(v);
//
//	free_ptr(v);
//
//	return 0;
//}




// 观察者模式（observer model）
extern class listenner;

class notifier  // 抽象通知者，通知者的变化会被监听者监听到（从而触发后者的动作）。
{
public:
	virtual ~notifier(){}  // 父类虚析构函数，防止子类不能正常析构（父类指针指向子类实例，先析构子类后父类）。
	virtual void addListenner(listenner* l) = 0;    // 注入监听者。
	virtual void removeListenner(listenner* l) = 0; // 移除监听者。
	virtual void notify() = 0;                      // 通知所有监听者。
};

class listenner  // 抽象监听者，监听通知者的消息触发相关动作。
{
public:
	virtual void teacherComming() = 0;  // 监听者对通知的动作。
};

class techerNotifier :public notifier  // 具体通知者。
{
public:
	~techerNotifier() { for (auto* l : __listenners) { free_ptr(l); } }

	void addListenner(listenner* l) override { __listenners.emplace_back(l); }

	void removeListenner(listenner* l) override { __listenners.remove(l); delete l; }

	void notify() override { for (auto* l : __listenners) { l->teacherComming(); } }

private:
	list<listenner*>  __listenners;  // 维护一个监听者指针列表。
};

class listennerA :public listenner     // 具体监听者。
{
public:
	void teacherComming() override { stopCopy(); }
	void stopCopy() { cout << "teacher is comming, stop copy homework!\n"; }
};

class listennerB : public listenner
{
public:
	void teacherComming() override { stopPlay(); }
	void stopPlay() { cout << "teacher is comming, stop playing games!\n"; }
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	listenner* l;
//	notifier* n;
//	n = new techerNotifier();
//
//	l = new listennerA();
//	n->addListenner(l);
//	l = new listennerB();
//	n->addListenner(l);
//	n->notify();
//
//	n->removeListenner(l);
//	n->notify();
//
//	return 0;
//}



// 状态模式（state model）
enum tlState { Red, Green, Yellow };

class state  // 抽象状态类。
{
public:
	virtual void handle() = 0;  // 状态的动作。
};

class trafficLight  // 上下文类，根据状态决定行为。
{
public:
	trafficLight(); // 类的循环调用，声明与定义分离。
	~trafficLight() { free_ptr(__state); }

	void setState(state* s) { __state = s; }  // 更改状态。
	void operation() { __state->handle(); }   // 依据状态产生操作，并修改状态。

private:
	state * __state;  // 维护一个状态指针。
};

class redLight :public state   // 具体状态类。
{
public:
	redLight(trafficLight* tl):__traffic(tl){}
	void handle() override;    // 涉及类的循环调用，无法在一个文件中实现。

private:
	trafficLight * __traffic;  // 维护一个上下文环境。
};

class greenLight : public state
{
public:
	greenLight(trafficLight* tl):__traffic(tl){}
	void handle() override;

private:
	trafficLight * __traffic;
};

class yellowLight : public state
{
public:
	yellowLight(trafficLight* tl):__traffic(tl){}
	void handle() override;

private:
	trafficLight * __traffic;
};


// 使用流程。
//trafficLight::trafficLight()
//{
//	__state = new redLight(this);  // 初始状态。
//}
//
//void redLight::handle()
//{
//	cout << "red light now.\n";
//	__traffic->setState(new greenLight(__traffic));  // 自动状态迁移。
//	delete this; // 删除当前状态对象。
//}
//
//void greenLight::handle()
//{
//	cout << "green light now.\n";
//	__traffic->setState(new yellowLight(__traffic));
//	delete this;
//}
//
//void yellowLight::handle()
//{
//	cout << "yellow light now.\n";
//	__traffic->setState(new redLight(__traffic));
//	delete this;
//}
//
//
//int main(int argc, char* argv[])
//{
//	trafficLight tl;
//
//	tlState state = Red;  // 初始状态为红灯
//	int i = 0;            // 总次数
//	int seconds;          // 秒数
//
//	while (true && i<10)
//	{
//		// 表示一个完整的状态流（红灯->绿灯->黄灯）已经完成
//		if (i % 3 == 0)
//			cout << "*****" << "Session " << ((i + 1) / 3) + 1 << "*****\n";
//
//		// 根据当前状态来设置持续时间，红灯（6秒）、绿灯（4秒）、黄灯（2秒）
//		if (state == Red)
//		{
//			seconds = 6;
//			state = Green;
//		}
//		else if (state == Green)
//		{
//			seconds = 4;
//			state = Yellow;
//		}
//		else if (state == Yellow)
//		{
//			seconds = 2;
//			state = Red;
//		}
//
//		// 休眠
//		Sleep(seconds * 1000);
//
//		tl.operation();
//		i++;
//	}
//
//
//	return 0;
//}




// 备忘录模式（memento model）
class memento  // 备忘录, 一个备忘录存储一个状态。
{
public:
	memento(string s):__state(s){}
	//void setState(string s) { __state = s; }
	string getState() { return __state; }

private:
	string __state;  // 状态。
};

class life   // 原发器， 创建备忘录并产生记录。
{
public:
	void setState(string s) { cout << "set state to " << s << endl; __state = s; }
	void display() { cout << "current state is " << __state << endl;; }

	memento* createMemento() { return new memento(__state); }  // 创建备忘录。
	void getState(memento* m) { __state = m->getState(); }     // 获取记录。

private:
	string __state;
};

class pandoraBox  // 管理者，负责保存备忘录和原发器。
{
public:
	pandoraBox(life* l):__life(l){}
	~pandoraBox() 
	{
		for (int i = 0; i < __history.size();i++) delete __history.at(i);
		__history.clear();  // 容器的clear()只做浅删除，指针指向的内存需要手动释放。
	}

	void save() { cout << "save ...\n"; __history.emplace_back(__life->createMemento()); } // 保存当前状态(备忘录)。
	void undo() { cout << "undo...\n"; __life->getState(__history.back()); }  // 恢复之前的状态（备忘录）。

private:
	life * __life;    // 维护一个原发器对象。
	vector<memento*>  __history;  // 维护原发器的备忘录列表。
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	life l;
//	memento* m;
//	pandoraBox p(&l);
//
//	l.setState("2019/9/9");
//	p.save();
//	l.setState("2019/9/10");
//	p.save();
//	l.setState("2019/9/11");
//	p.save();
//
//	l.setState("2019/9/11 9:11");
//	l.display();
//	p.undo();
//	l.display();
//
//
//	return 0;
//}



// 职责链模式（chain of responsibility model）
class handler  // 抽象处理者。
{
public:
	handler() { __nextHandler = nullptr; }
	virtual ~handler(){}

	void setNextHandler(handler* h) { __nextHandler = h; }  // 设置下一个处理者。
	virtual void handleRequst(int days) = 0;  // 处理请求。

protected:
	handler * __nextHandler;    // 后继者，下一个处理者。
};

class director :public handler  // 具体处理者。
{
public:
	void handleRequst(int days) override 
	{
		if(days <= 1) cout << "i'm director, have authority to agree 1 day off.\n";
		else __nextHandler->handleRequst(days);  // 如果当前处理者不能处理，向后传递处理请求。
	}
};

class manager :public handler
{
public:
	void handleRequst(int days) override
	{
		if (days <= 3) cout << "i'm manager, have authority to agree 3 days off.\n";
		else __nextHandler->handleRequst(days);
	}
};

class boss :public handler
{
public:
	void handleRequst(int days) override
	{
		if (days < 7) cout << "i'm boss, agree at most 7 days off.\n";
		else cout << "i'm boss, you ask too many days, don't agree.\n";
	}
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	director d;
//	manager m;
//	boss b;
//
//	d.setNextHandler(&m);
//	m.setNextHandler(&b);
//
//	d.handleRequst(1);
//	d.handleRequst(2);
//	m.handleRequst(3);
//	m.handleRequst(5);
//	b.handleRequst(6);
//	b.handleRequst(7);
//
//
//	return 0;
//}



// 迭代器模式（iterator model）
template<class Item>   // 模版,定义类对象。
class Iterator         // 抽象迭代器。
{
public:
	Iterator(){}
	virtual ~Iterator(){}

	virtual void first() = 0;
	virtual void next() = 0;
	virtual Item* curItem() = 0;  // 返回当前元素。
	virtual bool isDone() = 0;
};

template<class Item>
class Aggregate   // 抽象聚合类，存有迭代对象。
{
public:
	Aggregate(){}
	virtual~Aggregate(){}

	virtual void pushData(Item item) = 0;
	virtual Iterator<Item>* createIterator() = 0;  // 返回一个Item类型的迭代器指针。
	virtual Item& operator[](int index) = 0;  // 重载[]操作符，返回一个聚合元素。
	virtual int getSize() = 0;
};

template<class Item>
class ConcreteIterator :public Iterator<Item>  // 具体迭代器。
{
public:
	ConcreteIterator(Aggregate<Item>* a):__aggr(a), __cur(0){}
	~ConcreteIterator(){}

	void first() override { __cur = 0; }
	void next() override { if (__cur < __aggr->getSize()) __cur++; }
	bool isDone() override { return __cur >= __aggr->getSize(); }
	Item* curItem() override
	{
		if (__cur < __aggr->getSize()) return &(*__aggr)[__cur];
		else return nullptr;
	}

private:
	int  __cur;  // 当前元素索引。
	Aggregate<Item>* __aggr;  // 维护一个聚合类指针。
};

template<class Item>
class ConcreteAggregate :public Aggregate<Item>  // 模版父类的继承。
{
public:
	ConcreteAggregate(){}
	~ConcreteAggregate(){}

	void pushData(Item i) { __data.emplace_back(i); }
	Iterator<Item>* createIterator() // 指向当前聚合的迭代器。
	{ return new ConcreteIterator<Item>(this); } // 注意模版类的使用。
	Item& operator[](int index) { return __data[index]; }  
	int getSize() { return __data.size(); }

private:
	vector<Item>  __data;  // 存有所有被迭代对象。
};


// 使用流程。
//int main(int argc, char* argv[])
//{
//	Aggregate<int> * aggr = new ConcreteAggregate<int>();
//
//	aggr->pushData(3);
//	aggr->pushData(2);
//	aggr->pushData(1);
//
//	Iterator<int> * it = aggr->createIterator();
//
//	for (it->first(); !it->isDone(); it->next())
//	{
//		std::cout << *it->curItem() << std::endl;
//	}
//
//	delete it;
//	delete aggr;
//
//
//	return 0;
//}






















