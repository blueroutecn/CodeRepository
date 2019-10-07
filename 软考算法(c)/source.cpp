#include"head.h"


/* ------ 归并排序 ------ */

void MergeSort(int A[], int p, int r)  // p,r是排序元素的左右边界索引。
{
	int q;
	if (p < r)
	{
		q = (p + r) / 2;         // 分解，划分为左右两个子序列。
		MergeSort(A, p, q);      // 求解，递归解左序列。
		MergeSort(A, q + 1, r);  // 求解，递归解右序列。
		Merge(A, p, q, r);       // 合并，将排好序的子序列合并为原序列的解。
	}
}


void Merge(int A[], int p, int q, int r)
{
	int n1 = q - p + 1, n2 = r - q;  // n1，n2分别是左右子序列的长度。
	int i, j, k;

	int L[50], R[50];
	for (i = 0; i < n1; i++)
		L[i] = A[p + i];
	for (j = 0; j < n2; j++)
		R[j] = A[p + i + j];
	L[n1] = INT_MAX;
	R[n2] = INT_MAX;

	i = 0, j = 0;
	for (k = p; k < r + 1; k++)
	{
		if (L[i] < R[j])  // 左右子列虽然已排序，但元素不一定是连续的。
		{
			A[k] = L[i];
			i++;
		}
		else
		{
			A[k] = R[j];
			j++;
		}
	}
}



/* ------ 最大子段和问题 ------ */

int MaxSubSum(int *Array, int left, int right)
{
	int sum = 0;
	int i;
	if (left == right)  // 分解到单个整数，不可继续分割。
	{
		if (Array[left] > 0) sum = Array[left];
		else sum = 0;
	}
	else
	{
		/* 从left和right的中间分解数组。*/
		int center = (left + right) / 2;  // 划分的位置。
		int leftsum = MaxSubSum(Array, left, center);
		int rightsum = MaxSubSum(Array, center+1, right);

		/* 根据左右最大子段和，判断是情况1，2，3。*/
		int s1 = 0;
		int lefts = 0;
		for (i = center; i >= left; i--)
		{
			lefts = lefts + Array[i];
			if (lefts > s1) s1 = lefts;
		}
		int s2 = 0;
		int rights = 0;
		for (i = center + 1; i <= right; i++)
		{
			rights = rights + Array[i];
			if (rights > s2) s2 = rights;
		}

		sum = s1 + s2;
		if (sum < leftsum) sum = leftsum;   // 情况1.
		if (sum < rightsum) sum = rightsum; // 情况2.
	}
	return sum;
}



/* ------ 0-1背包问题 ------ */
int** KnapsackDP(int n, int W, int* Weights, float* Values)
{
	int i, w;
	int** c = (int**)malloc(sizeof(int*)*(n + 1));  // 二维数组开辟空间，c[i, w]。
	for (i = 0; i <= n; i++) c[i] = (int*)malloc(sizeof(int)*(W + 1));  // 数组指针元素开辟空间。

	/* 二维数组初始化 */
	for (w = 0; w <= W; w++) c[0][w] = 0;  
	for (i = 1; i <= n; i++)  // i从1开始。
	{
		c[i][0] = 0;
		for (w = 1; w <= W; w++)  // w从1开始。
		{
			if (Weights[i - 1] <= w)  // 当前物品重量小于背包剩余容量。
			{
				if (Values[i - 1] + c[i - 1][w - Weights[i - 1]] > c[i - 1][w])
				{
					c[i][w] = Values[i - 1] + c[i - 1][w - Weights[i - 1]];  // 当前物品放入背包。
				}
				else
				{
					c[i][w] = c[i - 1][w];  // 当前物品不放入背包。
				}
			}
			else
			{
				c[i][w] = c[i - 1][w];
			}
		}
	}
	return c;
}


void OutputKnapsackDP(int n, int W, int* Weights, float* Values, int** c)
{
	int* x = (int*)malloc(sizeof(int)*n);  // 一维数组，x[]是最优解。
	int i;
	for (i = n; i > 1; i--)
	{
		if (c[i][W] == c[i - 1][W])  // 说明当前层最优选择中不包含该物品。
		{
			x[i - 1] = 0;
		}
		else
		{
			x[i - 1] = 1;
			W = W - Weights[i - 1];  // 更新当前背包容量。
		}
	}
	x[0] = (c[1][W] == 0) ? 0 : 1;  // 第一个物品是否放入背包。

	for (i = 0; i < n; i++)
	{
		if (x[i] == 1) printf("Weigh:%d, Value:%f\n", Weights[i], Values[i]);
	}
}



/* ------ 最大公共子序列 ------ */
int** Lcs_Length(const char* str1, const char* str2, int str1_len, int str2_len)
{
	int i, j;

	/* 为矩阵l, b分配空间 */
	int** l = (int**)malloc(sizeof(int*)*(str1_len + 1));
	int** b = (int**)malloc(sizeof(int*)*(str1_len + 1));
	for (i = 0; i <= str1_len; i++)
	{
		l[i] = (int*)malloc(sizeof(int)*(str2_len + 1));
		b[i] = (int*)malloc(sizeof(int)*(str2_len + 1));
	}

	/* 初始化举证 */
	for (i = 0; i <= str1_len; i++) l[i][0] = 0;
	for (i = 0; i <= str2_len; i++) l[0][i] = 0;
	for (i = 1; i <= str1_len; i++)
	{
		for (j = 1; j <= str2_len; j++)
		{
			if (str1[i - 1] == str2[j - 1])
			{
				l[i][j] = l[i - 1][j - 1] + 1;
				b[i][j] = 0;  // 0代表指向右上方的箭头。
			}
			else if (l[i - 1][j] >= l[i][j - 1])
			{
				l[i][j] = l[i - 1][j];
				b[i][j] = 1;  // 1代表指向上方的箭头。
			}
			else
			{
				l[i][j] = l[i][j - 1];
				b[i][j] = 2;  // 2代表指向右方的箭头。
			}
		}
	}
	return b;
}


void OutputLcs(const char* str1, const int** b, int str1_len, int str2_len)
{
	if (str1_len == 0 || str2_len == 0) return;  //X,Y其中至少一个长度为0。

	if (b[str1_len][str2_len] == 0)  // 箭头指向右上。
	{
		OutputLcs(str1, b, str1_len - 1, str2_len - 1);
		printf("%c\n", str1[str1_len - 1]);
	}
	else if (b[str1_len][str2_len] == 1)  // 箭头向上。
	{
		OutputLcs(str1, b, str1_len - 1, str2_len);
	}
	else  // 箭头向右。
	{
		OutputLcs(str1, b, str1_len, str2_len - 1);
	}
}



/* ------ 背包问题 ------ */
float* GreedyKnapsack(int n, int W, int* Weights, float* Values, float* VW)
{
	int i;

	/* 分配空间及初始化 */
	float* x = (float*)malloc(sizeof(float)*n);
	for (i = 0; i < n; i++) x[i] = 0;
	for (i = 0; i < n; i++)
	{
		if (Weights[i] <= W)  // 如果背包剩余容量可以装下该物品。
		{
			x[i] = 1;
			W -= Weights[i];
		}
		else break;
	}

	if (i < n) x[i] = W / (float)Weights[i];  // 如果还有物品可以部分装入背包。
	return x;
}



// 0-1背包问题（回溯法）
float Bound(float* Values, int* Weights, float* VW, int n, int W, float Profit_Gained, int Weight_Used, int k)
{
	int i;
	for (i = k + 1; i < n; i++)
	{
		if (Weight_Used + Weights[i] <= W)
		{
			Profit_Gained += Values[i];
			Weight_Used += Weights[i];
		}
		else
		{
			Profit_Gained += VW[i] * (W - Weight_Used);
			Weight_Used = W;
			return Profit_Gained;
		}
	}
	return Profit_Gained;
}


int* Knapsack(float* Values, int* Weights, float* VW, int n, int W)
{
	int current_weight = 0;
	float current_profit = 0;
	int Weight = 0;
	float Profit = -1;
	int index = 0;

	/* 为数组X，Y分配空间 */
	int* X = (int*)malloc(sizeof(int)*n);
	int* Y = (int*)malloc(sizeof(int)*n);
	while (1)
	{
		while (index < n&&current_weight + Weights[index] < W)
		{
			current_profit += Values[index];
			current_weight += Weights[index];
			Y[index] = 1;
			index++;
		}

		if (index >= n)
		{
			Weight = current_weight;
			Profit = current_profit;
			index = n;
			for (int i = 0; i < n; i++) X[i] = Y[i];
		}
		else
		{
			Y[index] = 0;
		}

		while (Bound(Values, Weights, VW, n, W, current_profit, current_weight, index) <= Profit)
		{
			while (index != 0 && Y[index] != 1)  // 向前回溯。
			{
				index--;
			}

			if (index == 0) return X;

			Y[index] = 0;
			current_profit -= Values[index];
			current_weight -= Weights[index];
		}
		index++;
	}
}