#pragma once
#include<stdio.h>
#include <windows.h>

int main() 
{
	int a[10002];
	int i = 0;
	double run_time;

	_LARGE_INTEGER time_start;	//开始时间
	_LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率

	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);	//计时开始

	for (i = 1; i <= 10000; i++)a[i] = i;	//要计时的程序

	QueryPerformanceCounter(&time_over);	//计时结束
	run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	//乘以1000000把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒

	printf("\nrun_time：%fus\n", run_time);
	return 0;
}



#include<stdio.h>
#include <stdlib.h>//用到rand()函数
#include<time.h>   //用到clock()函数


int main() 
{
	int begintime, endtime;
	int i = 0;
	int a[1002];

	begintime = clock();	//计时开始

	for (i = 1; i <= 1000; i++) //要计时的程序
	{
		a[i] = rand() % 200 - 100;//产生-100到+100之间的随机数
		printf("  %d", a[i]);
	}

	endtime = clock();	  //计时结束

	printf("\n\nRunning Time：%dms\n", endtime - begintime);
	return 0;
}

