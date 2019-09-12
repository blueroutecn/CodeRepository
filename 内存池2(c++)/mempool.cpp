#include"head.h"


#define ELEMS 1000000         // ����Ԫ�ظ���
#define REPS 100              // �ظ�����


int main()
{
	clock_t start;

	// �ڴ�������ܶԱȡ�


	// ʹ��STLĬ�Ϸ�������
	StackAlloc<int, std::allocator<int>> stackDefault;
	start = clock();
	for (int j = 0; j < REPS; j++)
	{
		assert(stackDefault.empty());

		for (int i = 0; i < ELEMS; i++)
		{
			stackDefault.push(i);
		}

		for (int i = 0; i < ELEMS; i++)
		{
			stackDefault.pop();
		}

	}
	cout << "Default Allocator Time:";
	cout << (double)(clock() - start) / CLOCKS_PER_SEC << "\n\n";



	//ʹ���ڴ�ء�
	StackAlloc<int, MemoryPool<int>> stackPool;
	start = clock();
	for (int j = 0; j < REPS; j++)
	{
		assert(stackPool.empty());

		for (int i = 0; i < ELEMS; i++)
		{
			stackPool.push(i);
		}

		for (int i = 0; i < ELEMS; i++)
		{
			stackPool.pop();
		}
	}
	cout << "MemoryPool Allocator Time:";
	cout << (double)(clock() - start) / CLOCKS_PER_SEC << "\n\n";




	return(0);
}













