#include"head.h"
#include<time.h>


# ifdef __MergeSort
int main(int argc, char* argv[])
{
	int A[] = { 1,2,3,5,7,12, 8,9,22, 11, 42, 32, 35, 21, 25, 0 };
	int p = 0, r = 15;

	for (int i = 0; i <= 15; i++) printf("%d  ", A[i]); printf("\n");
	clock_t t_start = clock();

	MergeSort(A, p, r);

	clock_t t_end = clock();
	for (int i = 0; i <= 15; i++) printf("%d  ", A[i]); printf("\n");

	printf("runtime is %f\n", double(t_end - t_start)/CLOCKS_PER_SEC);

	system("pause");
	return 0;
}
# endif


# ifdef __MaxSubSum
int main(int argc, char* argv[])
{
	int arr[] = { 1, -5, 2, 4, 6, 23};
	int left = 0, right = 5;

	clock_t tstart = clock();

	int sum = MaxSubSum(arr, left, right);

	clock_t tstop = clock();

	printf("maxsubsum is %d\n", sum);
	printf("runtime is %f", double(tstop - tstart)/CLOCKS_PER_SEC);

	system("pause");
	return 0;
}
# endif


# ifdef __KnapsackDP
int main(int argc, char* argv[])
{
	int n = 5, W = 17;
	int Weights[] = { 3, 4, 7, 8, 9 };
	float Values[] = { 4, 5, 10, 11, 13 };

	clock_t tstart = clock();
	int** c = KnapsackDP(n, W, Weights, Values);
	OutputKnapsackDP(n, W, Weights, Values, c);
	clock_t tstop = clock();
	printf("runtime: %f", double(tstop - tstart)/CLOCKS_PER_SEC);

	system("pause");
	return 0;
}
# endif


# ifdef __Lcs_Leigths
int main(int argc, char* argv[])
{
	char str1[] = { 'A', 'B', 'C', 'B', 'D', 'A', 'B' };
	char str2[] = { 'B', 'D', 'C', 'A', 'B', 'A' };
	int str1_len = 7, str2_len = 6;

	clock_t tstart = clock();
	int** b = Lcs_Length(str1, str2, str1_len, str2_len);
	OutputLcs(str1,const_cast<const int**>(b), str1_len, str2_len);
	clock_t tstop = clock();
	printf("runtime : %f", double(tstop - tstart)/CLOCKS_PER_SEC);

	system("pause");
	return 0;
}
# endif


# ifdef __GreedyKnapsack
int main(int argc, char* argv[])
{
	int n = 5, W = 100;
	int Weights[] = { 30, 10, 20, 50, 40 };
	float Values[] = { 65, 20, 30, 60, 40 };
	float VW[] = { 2.1, 2, 1.5, 1.2, 1 };

 	clock_t tstart = clock();
	GreedyKnapsack(n, W, Weights, Values, VW);
	clock_t tstop = clock();
	printf("runtime: %f", double(tstop - tstart)/CLOCKS_PER_SEC);

	system("pause");
	return 0;
}
# endif


# ifdef __Knapsack
int main(int argc, char* argv)
{
	int n = 8, W = 110;
	float Values[] = { 11, 21, 31, 33, 43, 53, 55, 65 };
	int Weights[] = { 1, 11, 21, 23, 33, 43, 45, 55 };
	float VW[8];
	for (int i = 0; i < n; i++) VW[i] = (float)Values[i] / float(Weights[i]);
	int* x = (int*)malloc(sizeof(int)*n);

	clock_t tstart = clock();
	 x = Knapsack(Values, Weights, VW, n, W);
	clock_t tstop = clock();

	for (int i = 0; i < n; i++) printf("%d", x[i]);
	printf("runtime: %f", double(tstop - tstart) / CLOCKS_PER_SEC);

	system("pause");
	return 0;
}
# endif




