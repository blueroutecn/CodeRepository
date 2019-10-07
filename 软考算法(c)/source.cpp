#include"head.h"


/* ------ �鲢���� ------ */

void MergeSort(int A[], int p, int r)  // p,r������Ԫ�ص����ұ߽�������
{
	int q;
	if (p < r)
	{
		q = (p + r) / 2;         // �ֽ⣬����Ϊ�������������С�
		MergeSort(A, p, q);      // ��⣬�ݹ�������С�
		MergeSort(A, q + 1, r);  // ��⣬�ݹ�������С�
		Merge(A, p, q, r);       // �ϲ������ź���������кϲ�Ϊԭ���еĽ⡣
	}
}


void Merge(int A[], int p, int q, int r)
{
	int n1 = q - p + 1, n2 = r - q;  // n1��n2�ֱ������������еĳ��ȡ�
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
		if (L[i] < R[j])  // ����������Ȼ�����򣬵�Ԫ�ز�һ���������ġ�
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



/* ------ ����Ӷκ����� ------ */

int MaxSubSum(int *Array, int left, int right)
{
	int sum = 0;
	int i;
	if (left == right)  // �ֽ⵽�������������ɼ����ָ
	{
		if (Array[left] > 0) sum = Array[left];
		else sum = 0;
	}
	else
	{
		/* ��left��right���м�ֽ����顣*/
		int center = (left + right) / 2;  // ���ֵ�λ�á�
		int leftsum = MaxSubSum(Array, left, center);
		int rightsum = MaxSubSum(Array, center+1, right);

		/* ������������Ӷκͣ��ж������1��2��3��*/
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
		if (sum < leftsum) sum = leftsum;   // ���1.
		if (sum < rightsum) sum = rightsum; // ���2.
	}
	return sum;
}



/* ------ 0-1�������� ------ */
int** KnapsackDP(int n, int W, int* Weights, float* Values)
{
	int i, w;
	int** c = (int**)malloc(sizeof(int*)*(n + 1));  // ��ά���鿪�ٿռ䣬c[i, w]��
	for (i = 0; i <= n; i++) c[i] = (int*)malloc(sizeof(int)*(W + 1));  // ����ָ��Ԫ�ؿ��ٿռ䡣

	/* ��ά�����ʼ�� */
	for (w = 0; w <= W; w++) c[0][w] = 0;  
	for (i = 1; i <= n; i++)  // i��1��ʼ��
	{
		c[i][0] = 0;
		for (w = 1; w <= W; w++)  // w��1��ʼ��
		{
			if (Weights[i - 1] <= w)  // ��ǰ��Ʒ����С�ڱ���ʣ��������
			{
				if (Values[i - 1] + c[i - 1][w - Weights[i - 1]] > c[i - 1][w])
				{
					c[i][w] = Values[i - 1] + c[i - 1][w - Weights[i - 1]];  // ��ǰ��Ʒ���뱳����
				}
				else
				{
					c[i][w] = c[i - 1][w];  // ��ǰ��Ʒ�����뱳����
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
	int* x = (int*)malloc(sizeof(int)*n);  // һά���飬x[]�����Ž⡣
	int i;
	for (i = n; i > 1; i--)
	{
		if (c[i][W] == c[i - 1][W])  // ˵����ǰ������ѡ���в���������Ʒ��
		{
			x[i - 1] = 0;
		}
		else
		{
			x[i - 1] = 1;
			W = W - Weights[i - 1];  // ���µ�ǰ����������
		}
	}
	x[0] = (c[1][W] == 0) ? 0 : 1;  // ��һ����Ʒ�Ƿ���뱳����

	for (i = 0; i < n; i++)
	{
		if (x[i] == 1) printf("Weigh:%d, Value:%f\n", Weights[i], Values[i]);
	}
}



/* ------ ��󹫹������� ------ */
int** Lcs_Length(const char* str1, const char* str2, int str1_len, int str2_len)
{
	int i, j;

	/* Ϊ����l, b����ռ� */
	int** l = (int**)malloc(sizeof(int*)*(str1_len + 1));
	int** b = (int**)malloc(sizeof(int*)*(str1_len + 1));
	for (i = 0; i <= str1_len; i++)
	{
		l[i] = (int*)malloc(sizeof(int)*(str2_len + 1));
		b[i] = (int*)malloc(sizeof(int)*(str2_len + 1));
	}

	/* ��ʼ����֤ */
	for (i = 0; i <= str1_len; i++) l[i][0] = 0;
	for (i = 0; i <= str2_len; i++) l[0][i] = 0;
	for (i = 1; i <= str1_len; i++)
	{
		for (j = 1; j <= str2_len; j++)
		{
			if (str1[i - 1] == str2[j - 1])
			{
				l[i][j] = l[i - 1][j - 1] + 1;
				b[i][j] = 0;  // 0����ָ�����Ϸ��ļ�ͷ��
			}
			else if (l[i - 1][j] >= l[i][j - 1])
			{
				l[i][j] = l[i - 1][j];
				b[i][j] = 1;  // 1����ָ���Ϸ��ļ�ͷ��
			}
			else
			{
				l[i][j] = l[i][j - 1];
				b[i][j] = 2;  // 2����ָ���ҷ��ļ�ͷ��
			}
		}
	}
	return b;
}


void OutputLcs(const char* str1, const int** b, int str1_len, int str2_len)
{
	if (str1_len == 0 || str2_len == 0) return;  //X,Y��������һ������Ϊ0��

	if (b[str1_len][str2_len] == 0)  // ��ͷָ�����ϡ�
	{
		OutputLcs(str1, b, str1_len - 1, str2_len - 1);
		printf("%c\n", str1[str1_len - 1]);
	}
	else if (b[str1_len][str2_len] == 1)  // ��ͷ���ϡ�
	{
		OutputLcs(str1, b, str1_len - 1, str2_len);
	}
	else  // ��ͷ���ҡ�
	{
		OutputLcs(str1, b, str1_len, str2_len - 1);
	}
}



/* ------ �������� ------ */
float* GreedyKnapsack(int n, int W, int* Weights, float* Values, float* VW)
{
	int i;

	/* ����ռ估��ʼ�� */
	float* x = (float*)malloc(sizeof(float)*n);
	for (i = 0; i < n; i++) x[i] = 0;
	for (i = 0; i < n; i++)
	{
		if (Weights[i] <= W)  // �������ʣ����������װ�¸���Ʒ��
		{
			x[i] = 1;
			W -= Weights[i];
		}
		else break;
	}

	if (i < n) x[i] = W / (float)Weights[i];  // ���������Ʒ���Բ���װ�뱳����
	return x;
}



// 0-1�������⣨���ݷ���
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

	/* Ϊ����X��Y����ռ� */
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
			while (index != 0 && Y[index] != 1)  // ��ǰ���ݡ�
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