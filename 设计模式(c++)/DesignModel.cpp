#include "DesignModel.h"

int Singleton::__count = 0;
Singleton* Singleton::__pInstance = nullptr;


trafficLight::trafficLight()
{
	__state = new redLight(this);  // ��ʼ״̬��
}

void redLight::handle()
{
	cout << "red light now.\n";
	__traffic->setState(new greenLight(__traffic));  // �Զ�״̬Ǩ�ơ�
	delete this; // ɾ����ǰ״̬����
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









