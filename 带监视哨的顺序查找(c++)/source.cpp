#include"head.h"



int seqsearch(int data[], int length, int obj)
{
	int i = 0;
	while (i < length && data[i] != obj) i++;  // ��Ҫ��ʱ�������ж��ϡ�

	i = (i < length) ? i : -1;  // ���û�з��ִ���Ԫ�أ�����-1.
	return i;
}


int seqsearch_2(int data[], int length, int obj)
{
	int i = 0;
	int* new_data = new int[length+1];
	for (; i < length; i++)	new_data[i] = data[i]; new_data[length] = obj;  // ������ĩβ���ü����ڡ�

	i = 0;
	while (new_data[i] != obj) i++;  // �����ж����㡣

	i = (i < length) ? i : -1;
	return i;
}








