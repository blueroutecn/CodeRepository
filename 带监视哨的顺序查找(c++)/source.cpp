#include"head.h"



int seqsearch(int data[], int length, int obj)
{
	int i = 0;
	while (i < length && data[i] != obj) i++;  // 主要耗时在条件判断上。

	i = (i < length) ? i : -1;  // 如果没有发现待查元素，返回-1.
	return i;
}


int seqsearch_2(int data[], int length, int obj)
{
	int i = 0;
	int* new_data = new int[length+1];
	for (; i < length; i++)	new_data[i] = data[i]; new_data[length] = obj;  // 在数组末尾设置监视哨。

	i = 0;
	while (new_data[i] != obj) i++;  // 减少判断运算。

	i = (i < length) ? i : -1;
	return i;
}








