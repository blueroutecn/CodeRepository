#include"head.h"


#define ELEMS 1000000         // 插入元素个数
#define REPS 100              // 重复次数


int main()
{
	clock_t start;

	// 内存分配性能对比。


	// 使用STL默认分配器。
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



	//使用内存池。
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













