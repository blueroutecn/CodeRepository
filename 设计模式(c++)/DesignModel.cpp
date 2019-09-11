#include "DesignModel.h"

int Singleton::__count = 0;
Singleton* Singleton::__pInstance = nullptr;


trafficLight::trafficLight()
{
	__state = new redLight(this);  // 初始状态。
}

void redLight::handle()
{
	cout << "red light now.\n";
	__traffic->setState(new greenLight(__traffic));  // 自动状态迁移。
	delete this; // 删除当前状态对象。
}

void greenLight::handle()
{
	cout << "green light now.\n";
	__traffic->setState(new yellowLight(__traffic));  
	delete this;
}

void yellowLight::handle()
{
	cout << "yellow light now.\n";
	__traffic->setState(new redLight(__traffic));
	delete this;
}


#define __DesignModel
#ifdef __DesignModel
int main(int argc, char* argv[])
{
	
	return 0;
}
#endif









