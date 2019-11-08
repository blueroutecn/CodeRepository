#include<string>
#include<map>



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
	static map<string, iPlayer*>  __map;  // 静态数据成员，享元池(map(key, value)提供一对一的hash函数，相当于字典）。
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