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



// ������ģʽ��adapter model��
class productExist     // adaptee, �����ߣ� �Ѵ��ڵ��࣬��Ҫ������ӿ�Ϊ��������͡�
{
public:
	productExist() {}
	void apiExist(string msg="existed") { cout << msg <<" api\n"; }  // �Ѵ��ڵĽӿڡ�
};

class productWant    // interface, �ͻ�������
{
public:
	virtual void apiWant(string msg) = 0;  // ����Ľӿڡ�
};

class productAdapter:private productExist, public productWant  // ������������������滻����(productWant)
{
private:
	void adapterActivity(string &msg) { msg = "wanted"; }  // ������������ʵ�ֽӿ����䡣
public:
	productAdapter():productExist(), productWant(){}
	void apiWant(string msg) { adapterActivity(msg); apiExist(msg); } // ͨ���Ѵ��ڵĽӿ�ʵ������ӿڡ�
};

class productExist2  // �µ������ߣ��µ��Ѵ����ࡣ
{
public:
	void apiExist(string msg) { cout << msg << " api version2\n"; }
};

class productAdapter2 :private productExist2, public productWant  // �������������ڲ�ͬ���Ѵ��ࡣ
{
private:
	void adapterActivity(string &msg) { msg = "wanted"; }
public:
	productAdapter2():productExist2(), productWant(){}
	void apiWant(string msg) { adapterActivity(msg); apiExist(msg); }
};

class productAdapter3 :public productWant  // ������������������Ķ�����Ϊ��Ա���롣
{
private:
	productExist& __pe;  // ������Ϊ������ݳ�Ա��
private:
	void adapterActivity(string &msg) { msg = "wanted"; }
public:
	productAdapter3(productExist &pe):__pe(pe), productWant(){}
	void apiWant(string msg) { adapterActivity(msg); __pe.apiExist(msg); }  // �����Ѵ����Ľӿڡ�
};

class productAdapter4 :public productWant  // ���������������ڲ�ͬ���Ѵ����
{
private:
	productExist2 & __pe;  
	void adapterActivity(string &msg) { msg = "wanted"; }
public:
	productAdapter4(productExist2 &pe) :__pe(pe), productWant() {}
	void apiWant(string msg) { adapterActivity(msg); __pe.apiExist(msg); }  
};


// ʹ�����̡�
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



// װ����ģʽ(decorator model)
class Car  // �������
{
public:
	virtual void features() = 0;  // ��������(�ӿ�)��
};

class superCar:public Car  // ��������ࡣ
{
	void features() { cout << "This is superCar.\n"; }
};

class decoratorCar : public Car  // ����װ���࣬�̳г�����࣬�Ի����Ӧ�ӿڡ�
{
protected:
	Car* __car; 
public:
	decoratorCar(Car* car):__car(car), Car(){}
	void features() { __car->features(); addFeature(); }  // �ڻ���ӿڵĻ��������ӹ��ܡ�
	virtual void addFeature() = 0;  // װ�������������ܡ�
};

class runCar : public decoratorCar  // ʵ��װ��������̬�����¹��ܣ�
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


// ʹ�����̡�
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



// ���ģʽ��compsite model��
class component  // ����ͳһ������ �ṩһ�µķ����ԡ�
{
private:
	string __name;
public:
	component(string name):__name(name){}
	virtual ~component(){}   // ������������

	virtual void operation() = 0;
	string getName() { return __name; }
	virtual void add(component*) = 0;       // ��ӹ�����
	virtual void remove(component*) = 0;    // ɾ��������
	virtual component* getChild(int) = 0;   // ��ȡ�ӹ�����
};

class leaf :public component  // Ҷ�ӹ��������ӹ�����
{
public:
	leaf(string name):component(name){}
	virtual ~leaf(){}

	void operation(){ cout << "Leaf " << getName() << " Operation.\n"; }
	void add(component* pcom){}   //Ҷ�ӹ������ղ�����
	void remove(component* pcom){}
	component* getChild(int index) { return nullptr; }  // ���ӹ�����
};

class composite :public component  // ������� ��֦������
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
				delete *it;    // ɾ���ýڵ㹹��ָ�롣
				*it = nullptr; // �ڵ㹹��Ϊ�ա�
			}
			__vecComp.erase(it); // ɾ���ýڵ㹹����
			it = __vecComp.begin(); // ���µ�������
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


// ʹ�����̡�
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



// ���ģʽ��facade model��
class LightSystem  // ��ϵͳ�ࡣ
{
public:
	void on(){cout << "����..." << endl;}
	void off(){	cout << "�ص�..." << endl;}
};

class MovieSystem
{
public:
	void on(){cout << "�ŵ�Ӱ..." << endl;}
	void off(){	cout << "�رյ�Ӱ..." << endl;}
};

class SounderSystem
{
public:
	void on(){ cout << "������..." << endl;}
	void off(){	cout << "�ر�����..." << endl;}
};

class MusicSystem
{
public:
	void on(){ cout << "������..." << endl;	}
	void off(){	cout << "�ر�����..." << endl;}
};

class HomeSystem  //���ģʽ�࣬����������û��ṩ���Ľ���
{
public:	
	void OpenKTV()  //KTVģʽ...
	{
		light.off();
		sounder.on();
		music.on();
		cout << "*********KTVģʽ********" << endl;
	}

	void CloseKTV()
	{
		light.on();
		sounder.off();
		music.off();
		cout << "**********KTVģʽ �Ѿ��ر�********" << endl;
	}
	
	void OpenMovie()  //��Ӱģʽ...
	{
		light.off();
		sounder.on();
		sounder.on();
		cout << "*********��ͥӰԺģʽ********" << endl;
	}
	void CloseMovie()
	{
		light.on();
		sounder.off();
		sounder.off();
		cout << "*********��ͥӰԺģʽ �Ѿ��ر�********" << endl;
	}

private:
	LightSystem light;
	MovieSystem movie;
	MusicSystem music;
	SounderSystem sounder;
};


// ʹ�����̡�
//int main(int argc, char* argv[])
//{
//	HomeSystem home;
//	home.OpenKTV();//����KTVģʽ
//	home.CloseKTV();
//
//	home.OpenMovie();//������Ӱģʽ
//	home.CloseMovie();
//
//
//	system("pause");
//	return 0;
//}



// ����ģʽ��proxy model��
class iFactory   // ͳһ����ӿ��ࡣ
{
public:
	virtual void makeProduct() = 0;
};

class phoneFactory : public iFactory  // ��ʵ�ࡣ
{
public:
	void makeProduct() { cout << "make phone.\n"; }
};

class proxyFactory : public iFactory  // �����࣬����ʵ����͵��ö�������ø��롣
{
private:
	iFactory * __realObj;
public:
	proxyFactory(iFactory* fac):__realObj(fac){}
	void operation() { cout << "some other opertion.\n"; }
	void makeProduct() { __realObj->makeProduct(); }
};


// ʹ�����̡�
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



// ��Ԫģʽ��flyweight model��
class iPlayer  // ����������Ԫ�ࡣ
{
public:
	virtual ~iPlayer(){}
	virtual void assignWeapon(string weapon) = 0;
	virtual void mission() = 0;

protected:
	string __task;   // �ڲ�״̬��
	string __weapon; // �ⲿ״̬��
};

class terrorist : public iPlayer  // ������Ԫ��
{
public:
	terrorist() { __task = "plant a bomb"; }

	virtual void assignWeapon(string weapon) override 
	{ __weapon = weapon; }  // override,ǿ�����ػ����麯����
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

class playerFactory  // ��Ԫ������
{
public:
	static iPlayer* getPlayer(string type)  // ��̬������
	{
		iPlayer* p = nullptr;

		// ��� T / CT ������ڣ���ֱ�Ӵ���Ԫ�ػ�ȡ�����򣬴���һ���¶�����ӵ���Ԫ���У�Ȼ�󷵻ء�
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

			__map.insert(std::make_pair(type, p)); // һ���������������map�У�make_pair, ���������ݺϳ�һ��Ԫ�顣
		}
		return p;
	}

private:
	static map<string, iPlayer*>  __map;  // ��̬���ݳ�Ա����Ԫ�ء�
public:
	static string __playerType[];  // ������͡�
	static string __weapons[];     // �������͡�
};


// ʹ�����̡�
//// ��Ԫ�س�ʼ��Ϊ��, �ྲ̬���󵥶���ʼ����
//map<std::string, iPlayer*> playerFactory::__map = map<string, iPlayer*>();
//// ������ͳ�ʼ����
//string playerFactory::__playerType[2] = { "T", "CT" };
//// ���������ʼ����
//string playerFactory::__weapons[4] = { "AK-47", "Maverick", "Gut Knife", "Desert Eagle" };
//
//int main(int argc, char* argv[])
//{
//	//int playerLen = playerFactory::__playerType->size()/sizeof(string); wrong.
//	//int weaponsLen = playerFactory::__weapons->size()/sizeof(string);
//
//	int playerLen = 2, weaponsLen = 4;
//
//	// ���裬��Ϸ����ʮλ���
//	for (int i = 0; i < 10; i++)
//	{
//		// ��ȡ�����Һ�����
//		int typeIndex = rand() % playerLen;
//		int weaponIndex = rand() % weaponsLen;
//		string type = playerFactory::__playerType[typeIndex];
//		string weapon = playerFactory::__weapons[weaponIndex];
//
//		// ��ȡ���
//		iPlayer *p = playerFactory::getPlayer(type);
//
//		// ���������������������
//		p->assignWeapon(weapon);
//
//		// �����ȥִ������
//		p->mission();
//	}
//
//
//	system("pause");
//	return 0;
//}



// �Ž�ģʽ��bridge model��
class Abstraction           // �ӿڳ�����࣬������������Ľӿ�(�ӿڶ˳���)��
{
public:
	virtual ~Abstraction() {};
	virtual void Operation() = 0;  // �����ӿڣ���ʾ������֧�ֵĲ�����������ࡣ

protected:
	Abstraction() {};              // ֻ�����������������ɡ�
};

class AbstractionImplement  //ʵ�ֳ�����࣬�����˽ӿ�ʵ�֣�ʵ�ֶ˳��󣩡�
{
public:
	virtual ~AbstractionImplement() {};
	virtual void work() = 0;    // ����ʵ�ֽӿڣ�ʵ�ֽӿ���Abstraction�Ľӿڲ�����ͬ����

protected:
	AbstractionImplement() {};
};

class RefinedAbstractionA :public Abstraction   // ����Abstraction����Ľӿڡ�
{
public:
	RefinedAbstractionA(AbstractionImplement* imp):__imp(imp){}
	virtual ~RefinedAbstractionA() { delete __imp; __imp = nullptr;}
	virtual void Operation()  override  // ����AbstractionImplement���󣬲���չAbstraction�ӿڡ�
	{cout << "refined abstractionA operation: "; __imp->work();}  // �������__imp�ĳ�Ա������__imp��������ڴ�֮ǰ���塣   

private:
	AbstractionImplement* __imp; // ά��һ��ָ��AbstractionImplement��ָ�롣
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

class ConcreteAbstractionImplementA :public AbstractionImplement  // ����ʵ���ࡣ
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


// ʹ�����̡�
//int main(int argc, char* argv[])
//{
//	AbstractionImplement* imp = new ConcreteAbstractionImplementA();
//	Abstraction* abs = new RefinedAbstractionA(imp);
//	abs->Operation();
//
//	imp = new ConcreteAbstractionImplementB();  // ʵ�ַ�ʽ��
//	abs = new RefinedAbstractionA(imp);         // �ӿ����͡�
//	abs->Operation();                           // �ӿ�ʵ�֡�
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



// ����ģʽ��singleton model��
class Singleton  //��ģʽ������ģʽ����Ҫʵ��ʱ�Ŵ�����
{
public:
	static Singleton * GetInstance()// ��̬������ȫ��Ψһ��
	{
		if (!__pInstance) __pInstance = new Singleton();
		__count++;
		return __pInstance;
	}

	void display() { cout << "singleton has been called " << __count << "times.\n"; }

	~Singleton() {}

private:
	static int __count;
	static Singleton* __pInstance;  // ά��һ��Ψһָ��singleton����ľ�ָ̬�롣

private:     // ����κ��ⲿ����ʵ���ķ�ʽ��
	Singleton() {}
	Singleton(const Singleton&) {}
	Singleton& operator = (const Singleton&) {};
};


/*
-------------------------------------------------------
���⣺
1����Ҫ�ֶ��ͷ�__pInstance(����һ��������)��
2������ڶ��̻߳�������ȫ�������ġ�������)��
-------------------------------------------------------
*/


// ����һ��ע���˳�����atexit(RealesSingleton)��
// int atexit(void(*function)(void)); ע�����Ϊ�޲����޷��صĺ���ָ�룻���ú�ע��˳���෴. 
void RealesSingleton()
{
	delete Singleton::GetInstance();
	cout << "singleton has been deleted.\n";
}


// ��������ʹ������ָ��auto_ptr<Singleton> m_pInstance��


// ��������ʹ����Ƕ���һ����̬��Ա��
// �������ʱ����������ȫ�ֱ�����������ľ�̬��Ա���Դˣ���������һ��˽�о�̬��Ա����ָ��������
class Singleton2
{
public:
	static Singleton2 * GetInstance()
	{
		if (__pInstance.get() == nullptr) __pInstance.reset(new Singleton2);  // ����ָ���ʹ�ã��ࣩ��
		__count++;
		return __pInstance.get();
	}

	void display() { cout << "singleton2 has been called " << __count << " times.\n"; }

	~Singleton2() {}  // ������鹹����û�в�����

private:
	static int __count;
	static auto_ptr<Singleton2> __pInstance;  // ��������ָ�뱣��singleton����

private:
	class Clear  // ��Ƕ��, ������ⶨ��Singleton2::Clear::~Clear()��
	{
	public:
		~Clear()
		{
			__pInstance.release();   // clear��������ʱ���ͷ�__pInstance. 
			cout << "singleton2 has been deleted.\n";
		}
	};
	static Clear __clear;  // ��̬��Ա����Ҫ��ʼ�������ܻ����һ��clear��������ʱ����~clear����

private:
	Singleton2() {}
	Singleton2(const Singleton2&) {}
	Singleton2& operator = (const Singleton2&) {};
};


// �����ģ�˫��������ơ�
class LockGuard
{
public:
	LockGuard() { InitializeCriticalSection(&__cs); }  // ��ʼ���ٽ���������
	~LockGuard() { DeleteCriticalSection(&__cs); }     // �ͷ��ٽ���������

private:
	void Guard() { EnterCriticalSection(&__cs); }    // �ж��Ƿ��ܼ��뵱ǰ�ٽ�����
	void unGuard() { LeaveCriticalSection(&__cs); }  // �ͷ���Դ�������뿪�ٽ�����

private:
	CRITICAL_SECTION __cs;  // ���߳�ͬʱ���ʵ���Դ���ٽ�����

public:
	class CGuard  // ����ģ�
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
			LockGuard::CGuard gd(__lg); // ����һ����ʱCGuard�����ж��Ƿ���Խ��룬�뿪ʱ�����ͷš�
			if (__pInstance == nullptr) __pInstance = new Singleton3();
		}
		__count++;
		return __pInstance;
	}

private:
	static int __count;
	static LockGuard __lg;  // ���� �����ж��Ƿ���Խ����ٽ�����Ψһsingleton����
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


// �����壺����ģʽ�������Ƿ���õ�������ʼ������ʵ��,�����þ�̬��ʼ����֤�̰߳�ȫ��
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


// ʹ�����̡�
//int Singleton::__count = 0;
//Singleton* Singleton::__pInstance = nullptr;
//
//int Singleton2::__count = 0;
//auto_ptr<Singleton2> Singleton2::__pInstance; // ����ָ�벻��Ҫnullptr��ʼ����
//Singleton2::Clear Singleton2::__clear;   // ��̬��Ƕ������ʼ��(�ܻ���һ������)��
//
//int Singleton3::__count = 0;
//LockGuard Singleton3::__lg;
//Singleton3* Singleton3::__pInstance = nullptr;
//Singleton3::Clear Singleton3::__cl;
//
//int Singleton4::__count = 0;
//Singleton4* Singleton4::__pInstance = new Singleton4(); // ����ʼ������һ��ʵ����
//Singleton4::Clear Singleton4::__clear;
//
//int main(int argc, char* argv[])
//{
//	atexit(RealesSingleton);         // ����һ��ע���˳�ʱִ�к�����
//	Singleton* p = nullptr;
//	p = Singleton::GetInstance();
//	p->display();
//	p = Singleton::GetInstance();
//	p = Singleton::GetInstance();
//	p->display();
//
//
//	Singleton2* p2 = nullptr;         // ��������ʹ����Ƕ����侲̬��Ա��
//	p2 = Singleton2::GetInstance();   // ��������ʹ������ָ�롣
//	p2->display();
//	p2 = Singleton2::GetInstance();
//	p2 = Singleton2::GetInstance();
//	p2->display();
//
//
//	Singleton3* p3 = nullptr;         // �����ģ�ʹ��˫��������ơ�
//	p3 = Singleton3::GetInstance();
//	p3->display();
//	p3 = Singleton3::GetInstance();
//	p3 = Singleton3::GetInstance();
//	p3->display();
//
//
//	Singleton4* p4 = nullptr;         // �����壬ʹ�ö���ģʽ��
//	p4 = Singleton4::GetInstance();
//	p4->display();
//	p4 = Singleton4::GetInstance();
//	p4 = Singleton4::GetInstance();
//	p4->display();
//
//  ���������˳����singleton1��4��3��2��
//	//system("pause");  
//	return 0;
//}



// �򵥹���ģʽ��simple factory model��
class IceCream // �����һ�������ӿڣ���Ϊ���麯����
{
public:
	virtual void taste() { cout << "can't find your teste icecream.\n"; }
};

class AppleIceCream :public IceCream // ������ʵ������һ���ฺ��һ����һ���ܡ�
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

enum TASTE { Apple, Banana, Orange, Peach };  // ���ֿ�ζ��

class IceCreamFactory // ������,�ɹ���������Ʒ����Ʒ����ı仯�ᵼ�¹����ı仯��
{
public:
	IceCream * create_ice_cream(TASTE taste)
	{
		IceCream* _iceCream = nullptr;

		switch (taste)  // ��ζ��ʵ�֣��ĸı䣬������ҲҪ�ı䡣
		{
		case Apple:_iceCream = new AppleIceCream(); break;
		case Banana:_iceCream = new BananaIceCream(); break;
		case Orange:_iceCream = new OrangeIceCream(); break;
		default: _iceCream = new IceCream();
		}

		return _iceCream;
	}
};


// ʹ�����̡�
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



// ����ģʽ��factory model��
namespace fm    // �趨�����ռ䣬���⺯�����������͵�������ͻ��
{
	class IceCream  // �����Ʒ�ࡣ
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

	class IceCreamFactory    // ���󹤳��࣬ѡ������Ʒ������������
	{
	public:
		virtual IceCream * createIceCream() = 0;  
	};

	class AppleIceCreamFactory :public IceCreamFactory  // ���幤���࣬����һ���Ʒ��
	{
	public:
		IceCream * createIceCream() { return new AppleIceCream(); } // ���ɾ����Ʒ�����س����Ʒ���͡�
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


// ʹ�����̡�
//int main(int argc, char* argv[])
//{
//	fm::IceCream* ice;
//	fm::IceCreamFactory* iceFac;
//
//	iceFac = new fm::AppleIceCreamFactory();
//	ice = iceFac->createIceCream();
//	ice->taste();
//
//	delete iceFac;  // ɾ��ָ����ָ����
//	iceFac = new fm::BananaIceCreamFactory();
//	ice = iceFac->createIceCream();
//	ice->taste();
//
//	return 0;
//}



// ���󹤳�ģʽ��abstract factory model��
class phone  // ��Ʒ�����ࡣ
{
public:
	virtual ~phone() {};
	virtual void call(string num) = 0;
};

class androidPhone : public phone  // ��Ʒ�ࡣ
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

class mobileFactory  // ���󹤳��࣬���󹤳�����һ����Ʒ�壨�����Ʒ��������û��˳��Ҫ��
{
public:
	virtual ~mobileFactory(){}
	virtual phone* createPhone() = 0;  // �����ֻ���Ʒ(��Ʒ��)
	virtual pad* createPad() = 0;      // ����ƽ���Ʒ(��Ʒ��)
};

class androidFactory : public mobileFactory  // ���幤����
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


// ʹ�����̡�
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



// ������ģʽ��builder model��
class Builder  // ���������࣬������Ʒ����ֽ��첽��Ľӿڡ�
{
public:
	virtual ~Builder() {}
	virtual void BuildPartA() = 0;
	virtual void BuildPartB() = 0;
};

class ConcreteBuilder1 :public Builder  // ���彨���ߡ�
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

class Director  // ָ���ߣ����ý����߽ӿ���װ��Ʒ��
{
public:
	Director(Builder* pBuilder):__pBuilder(pBuilder){}
	~Director() { delete __pBuilder; __pBuilder = nullptr; }
	void construct() { __pBuilder->BuildPartA(); __pBuilder->BuildPartB(); } // ��˳����װ��Ʒ��

private:
	Builder * __pBuilder;  // ά��һ�������߶���ָ�롣
};


// ʹ�����̡�
//int main(int argc, char* argv[])
//{
//	Builder* b;
//	Director* d;
//
//	b = new ConcreteBuilder1();
//	d = new Director(b);
//	d->construct();
//
//	delete b;  // ���ﲻ��Ҫdelete d����Ϊdָ���Director��������������ָ�롣
//	b = new ConcreteBuilder2();
//	d = new Director(b);
//	d->construct();
//
//	delete b;
//
//	return 0;
//}



// ԭ��ģʽ��prototype model��
class Prototype  // ����ԭ���ࡣ
{
public:
	virtual ~Prototype(){}
	virtual Prototype* Clone() const { return nullptr; }

protected:    
	Prototype(){}
};

class ConcretePrototype : public Prototype  // ����ԭ�͡�
{
public:
	ConcretePrototype() { cout << "prototype is created.\n"; }
	ConcretePrototype(const Prototype& cp) { cout << "prototype is created.\n"; }
	~ConcretePrototype() { cout << "prototype is deleted.\n"; }

	Prototype* Clone() const override
	{ cout << "prototype is cloned.\n"; return new ConcretePrototype(*this); } // �����Լ���
};


// ʹ�����̡�
//int main(int argc, char* argv[])
//{
//	Prototype* p = new ConcretePrototype();
//	Prototype* p2 = p->Clone();
//	Prototype* p3 = p->Clone();
//
//	delete p;
//	p = nullptr;
//
//	if (p2)  // ɾ��p2ָ��Ķ���
//	{
//		delete p2;
//		p2 = nullptr;
//	}
//
//	if (p3)  // ɾ��p3ָ��Ķ���
//	{
//		delete p3;
//		p3 = nullptr;
//	}
//
//	return 0;
//}



// ģ�淽��ģʽ��template method��
class testPaper  // �㷨ģ������࣬�������㷨�������ܺ����̡�
{
public:
	void doTest() { name(); oneAsk(); twoAsk(); }  // �����㷨��ܺ����̡�
	void oneAsk() { cout << "question one��  "; answerOne(); }  // �����㷨���衣
	void twoAsk() { cout << "question two:   "; answerTwo(); }

	virtual void answerOne() = 0;  // �����㷨ʵ��, ��������ʵ�֡�
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


// ʹ�����̡�
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



// �н���ģʽ��mediator model��
extern class colleague;

class mediator   // �����н��ߡ�
{
public:
	virtual void registerColleague(colleague* coll)   // ��������ע�ᵽ�н��ߡ�
	{ __colleagues.emplace_back(coll); }  // emplace_back ���push_back, ����ʱ����ʱ������Ч�ʸ��ߡ�
	
	const list<colleague*>& getColleagues() const     
	{ return __colleagues; }  // ���������޸ĳ�Ա���������ز�����ָ�볣�б�����ã��������޸ģ���

	virtual void sendMessage(const colleague* sender, const string& msg) const = 0; // �н��߳���ӿڣ�����������Ϣ���͸����в����ߡ�

private:
	list<colleague*>  __colleagues;  // ά��һ�������ߵ�ָ���б�
};

class colleague  // ��������ߡ�
{
public:
	colleague(const string& name):__name(name){}

	string getName() const { return __name; }

	virtual void sendMessage(const mediator& md, const string& msg) const = 0; // ͨ���н��߷�����Ϣ��

	virtual void getMessage(const colleague* co, const string& msg) const = 0;  // ���շ����ߵ���Ϣ��

protected: 
	string __name;
};

class concreteMediator : public mediator     // �����н��ߡ�
{
public:
	void sendMessage(const colleague* sender, const string& msg) const override  // ͨ�ŷ�ʽ��
	{
		for (const colleague* c : getColleagues())  // ��ѭ����ʽ��
		{
			if (c != sender)  // colleague������ǰ���������ǵ����ó�Աʱ������Ѷ��塣
				c->getMessage(sender, msg); // ���๫��������˽�У�����ָ��ָ������ʵ���������������˽�г�Ա��
		}
	}
};

class concreteColleague : public colleague   // ��������ߡ�
{
public:
	concreteColleague(const string& name):colleague(name) {}

	void sendMessage(const mediator& md, const string& msg) const override
	{
		md.sendMessage(this, msg);  // this, ��ǰ�����ָָ�롣
	}

private:
	void getMessage(const colleague* sender, const string& msg) const override   // ͨ������ָ���̬���á�
	{
		cout << __name << " get the message from " << sender->getName() << ": " << msg << endl;
	}
};


// ʹ�����̡�
//int main(int argc, char* argv[])
//{
//	colleague *landlord = new concreteColleague("Tom");	// ����
//
//	colleague *jerry = new concreteColleague("Jerry");	// ���
//	colleague *tuffy = new concreteColleague("Tuffy");
//
//	concreteMediator mediator; // �н��� - ������
//	mediator.registerColleague(jerry);
//	mediator.registerColleague(tuffy);
//	mediator.registerColleague(landlord);
//
//	// ͨ���н齫��Ϣ���ͳ�ȥ
//	landlord->sendMessage(mediator, "Xi'erqi, two bedroom house, 6000/month.");
//	jerry->sendMessage(mediator, "want one bedroom house.");
//	tuffy->getMessage(jerry, "want to rent house with you.");  // ��̬c++��
//
//	return 0;
//}




// ����ģʽ��strategy model��
#define free_ptr(p) if(p) delete p; p = nullptr  

class wind  // ��������ࡣ
{
public:
	virtual ~wind() {}
	virtual void blow() = 0;  // ͳһ�ӿڡ�
};

class coldWind : public wind  // �����㷨�ࡣ
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

class windMode  // �ͻ��˽ӿ��ࡣ
{
public:
	windMode(wind* wind):__wind(wind){}
	~windMode() { free_ptr(__wind); }
	void blowWind() { __wind->blow(); }  // �ͻ��˽ӿڣ����ʲ��ԡ�

private:
	wind * __wind;  // ά��һ������������ָ�롣
};


// ʹ�����̡�
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




// ������ģʽ��visitor model��
extern class parkA;
extern class parkB;

class visitor  // ��������ߡ�
{
public:
	virtual ~visitor(){}
	virtual void visit(parkA*) = 0;  // ���ʶ���ӿڡ�
	virtual void visit(parkB*) = 0;
};

class tourist : public visitor  // ����ķ����ߣ����ʶ��󲻱䣬�����ض��ķ��ʲ�����
{
public:
	void visit(parkA*) override { cout << "tourist visit parkA.\n"; }  // ���ʲ�����
	void visit(parkB*) override { cout << "tourist visit parkB.\n"; }
};

class cleaner : public visitor
{
public:
	void visit(parkA*) override { cout << "cleaner clean parkA.\n"; }
	void visit(parkB*) override { cout << "cleaner clean parkB.\n"; }
};

class place  // ����Ԫ���࣬ΪԪ��ͳһ�����߽ӿڡ�
{
public:
	virtual ~place(){}
	virtual void accept(visitor*) = 0;  // Ԫ�ؽ�����Щ�����ߡ�
};

class parkA :public place
{
public:
	void accept(visitor* v) override 
	{ cout << "parkA accept visitor.\n"; v->visit(this); }  // ͨ����Ԫ���е�����Ӧ������ʵ�ַ��ʡ�
};

class parkB : public place
{
public:
	void accept(visitor* v) override
	{ cout << "parkB accept visitor.\n"; v->visit(this); }
};

class city  // �ͻ��ˡ�
{
public:
	~city() { for (auto* p : __places) { free_ptr(p); } } // �ͷ�ָ��ָ��Ŀռ䣬֮����list�ͷ�ָ��Ԫ�ء�

	void login(place* p) { __places.emplace_back(p); }  // ע��Ԫ�ء�

	void logout(place* p) { __places.remove(p); }       // �Ƴ�Ԫ�ء�

	void operation(visitor* v) { for (place* p : __places) p->accept(v); }  // �ͻ��˷�װ��

private:
	list<place*>  __places;  // ά��һ��Ԫ��ָ���б�������Ҫ�ͷ�ָ����ڴ棨֮ǰ��ģʽδ������
};


// ʹ�����̡�
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




// �۲���ģʽ��observer model��
extern class listenner;

class notifier  // ����֪ͨ�ߣ�֪ͨ�ߵı仯�ᱻ�����߼��������Ӷ��������ߵĶ�������
{
public:
	virtual ~notifier(){}  // ������������������ֹ���಻����������������ָ��ָ������ʵ����������������ࣩ��
	virtual void addListenner(listenner* l) = 0;    // ע������ߡ�
	virtual void removeListenner(listenner* l) = 0; // �Ƴ������ߡ�
	virtual void notify() = 0;                      // ֪ͨ���м����ߡ�
};

class listenner  // ��������ߣ�����֪ͨ�ߵ���Ϣ������ض�����
{
public:
	virtual void teacherComming() = 0;  // �����߶�֪ͨ�Ķ�����
};

class techerNotifier :public notifier  // ����֪ͨ�ߡ�
{
public:
	~techerNotifier() { for (auto* l : __listenners) { free_ptr(l); } }

	void addListenner(listenner* l) override { __listenners.emplace_back(l); }

	void removeListenner(listenner* l) override { __listenners.remove(l); delete l; }

	void notify() override { for (auto* l : __listenners) { l->teacherComming(); } }

private:
	list<listenner*>  __listenners;  // ά��һ��������ָ���б�
};

class listennerA :public listenner     // ��������ߡ�
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


// ʹ�����̡�
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



// ״̬ģʽ��state model��
enum tlState { Red, Green, Yellow };

class state  // ����״̬�ࡣ
{
public:
	virtual void handle() = 0;  // ״̬�Ķ�����
};

class trafficLight  // �������࣬����״̬������Ϊ��
{
public:
	trafficLight(); // ���ѭ�����ã������붨����롣
	~trafficLight() { free_ptr(__state); }

	void setState(state* s) { __state = s; }  // ����״̬��
	void operation() { __state->handle(); }   // ����״̬�������������޸�״̬��

private:
	state * __state;  // ά��һ��״ָ̬�롣
};

class redLight :public state   // ����״̬�ࡣ
{
public:
	redLight(trafficLight* tl):__traffic(tl){}
	void handle() override;    // �漰���ѭ�����ã��޷���һ���ļ���ʵ�֡�

private:
	trafficLight * __traffic;  // ά��һ�������Ļ�����
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


// ʹ�����̡�
//trafficLight::trafficLight()
//{
//	__state = new redLight(this);  // ��ʼ״̬��
//}
//
//void redLight::handle()
//{
//	cout << "red light now.\n";
//	__traffic->setState(new greenLight(__traffic));  // �Զ�״̬Ǩ�ơ�
//	delete this; // ɾ����ǰ״̬����
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
//	tlState state = Red;  // ��ʼ״̬Ϊ���
//	int i = 0;            // �ܴ���
//	int seconds;          // ����
//
//	while (true && i<10)
//	{
//		// ��ʾһ��������״̬�������->�̵�->�Ƶƣ��Ѿ����
//		if (i % 3 == 0)
//			cout << "*****" << "Session " << ((i + 1) / 3) + 1 << "*****\n";
//
//		// ���ݵ�ǰ״̬�����ó���ʱ�䣬��ƣ�6�룩���̵ƣ�4�룩���Ƶƣ�2�룩
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
//		// ����
//		Sleep(seconds * 1000);
//
//		tl.operation();
//		i++;
//	}
//
//
//	return 0;
//}




// ����¼ģʽ��memento model��
class memento  // ����¼, һ������¼�洢һ��״̬��
{
public:
	memento(string s):__state(s){}
	//void setState(string s) { __state = s; }
	string getState() { return __state; }

private:
	string __state;  // ״̬��
};

class life   // ԭ������ ��������¼��������¼��
{
public:
	void setState(string s) { cout << "set state to " << s << endl; __state = s; }
	void display() { cout << "current state is " << __state << endl;; }

	memento* createMemento() { return new memento(__state); }  // ��������¼��
	void getState(memento* m) { __state = m->getState(); }     // ��ȡ��¼��

private:
	string __state;
};

class pandoraBox  // �����ߣ����𱣴汸��¼��ԭ������
{
public:
	pandoraBox(life* l):__life(l){}
	~pandoraBox() 
	{
		for (int i = 0; i < __history.size();i++) delete __history.at(i);
		__history.clear();  // ������clear()ֻ��ǳɾ����ָ��ָ����ڴ���Ҫ�ֶ��ͷš�
	}

	void save() { cout << "save ...\n"; __history.emplace_back(__life->createMemento()); } // ���浱ǰ״̬(����¼)��
	void undo() { cout << "undo...\n"; __life->getState(__history.back()); }  // �ָ�֮ǰ��״̬������¼����

private:
	life * __life;    // ά��һ��ԭ��������
	vector<memento*>  __history;  // ά��ԭ�����ı���¼�б�
};


// ʹ�����̡�
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



// ְ����ģʽ��chain of responsibility model��
class handler  // �������ߡ�
{
public:
	handler() { __nextHandler = nullptr; }
	virtual ~handler(){}

	void setNextHandler(handler* h) { __nextHandler = h; }  // ������һ�������ߡ�
	virtual void handleRequst(int days) = 0;  // ��������

protected:
	handler * __nextHandler;    // ����ߣ���һ�������ߡ�
};

class director :public handler  // ���崦���ߡ�
{
public:
	void handleRequst(int days) override 
	{
		if(days <= 1) cout << "i'm director, have authority to agree 1 day off.\n";
		else __nextHandler->handleRequst(days);  // �����ǰ�����߲��ܴ�����󴫵ݴ�������
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


// ʹ�����̡�
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



// ������ģʽ��iterator model��
template<class Item>   // ģ��,���������
class Iterator         // �����������
{
public:
	Iterator(){}
	virtual ~Iterator(){}

	virtual void first() = 0;
	virtual void next() = 0;
	virtual Item* curItem() = 0;  // ���ص�ǰԪ�ء�
	virtual bool isDone() = 0;
};

template<class Item>
class Aggregate   // ����ۺ��࣬���е�������
{
public:
	Aggregate(){}
	virtual~Aggregate(){}

	virtual void pushData(Item item) = 0;
	virtual Iterator<Item>* createIterator() = 0;  // ����һ��Item���͵ĵ�����ָ�롣
	virtual Item& operator[](int index) = 0;  // ����[]������������һ���ۺ�Ԫ�ء�
	virtual int getSize() = 0;
};

template<class Item>
class ConcreteIterator :public Iterator<Item>  // �����������
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
	int  __cur;  // ��ǰԪ��������
	Aggregate<Item>* __aggr;  // ά��һ���ۺ���ָ�롣
};

template<class Item>
class ConcreteAggregate :public Aggregate<Item>  // ģ�游��ļ̳С�
{
public:
	ConcreteAggregate(){}
	~ConcreteAggregate(){}

	void pushData(Item i) { __data.emplace_back(i); }
	Iterator<Item>* createIterator() // ָ��ǰ�ۺϵĵ�������
	{ return new ConcreteIterator<Item>(this); } // ע��ģ�����ʹ�á�
	Item& operator[](int index) { return __data[index]; }  
	int getSize() { return __data.size(); }

private:
	vector<Item>  __data;  // �������б���������
};


// ʹ�����̡�
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






















