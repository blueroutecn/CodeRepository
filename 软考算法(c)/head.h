#pragma once
#include<iostream>



/* ------ 分治法（Divide-and-Conquer）------ */
/*
分治法的一般思路：
1. 分解，将原问题分解成一系列独立的子问题。
2. 求解，递归地求解各子问题；若子问题足够小，则直接求解。
3. 合并，将子问题的解合并成原问题的解。
*/


// 归并排序（Merge Sort）
/*
算法思路：
将待排序元素分成大小大致相同的两个子序列，分别对这两个子序列进行排序；
最终，将排序好的子序列合并为所要求的序列。
1. 分解，将n个元素分成各含n/2个元素的子序列；
2. 求解，用归并排序对子序列递归的排序；
3. 合并，合并两个已经排好序的子序列以得到排序结果。
(时间复杂度O(nlgn))
*/
void MergeSort(int A[], int p, int r);     // p,r是待排序元素的左右边界索引。
void Merge(int A[], int p, int q, int r);  // p, q是左右边界索引，r是分割索引。


// 最大子段和问题
/*
由给定的n个整数（可以有负整数）组成的序列a1, a2, ... an, 求其形如∑ak的子段和最大值。
1. 分解， 将所给序列的A[1...n]分为长度相等的两段，分别求出最大子段和。
=>>这里有3种情况；
1.1. 原序列的最大子段和同左子序列的最大子段和相同；
1.2. 原序列的最大子段和同右子序列的最大子段和相同；
1.3. 原序列的最大子段和无法单独通过单一子序列得出。
2. 解决， 对1.1、1.2的情况可以通过递归求解得到；对1.3的情况可以求和左右子序列各自的最大子段和得到。
3. 合并， 对比3种情况下的最大子段和，取最大者为原问题的解。
(时间复杂度O(nlgn))
*/
int MaxSubSum(int *Array, int left, int right);  // Array是数组， left、right是数组左右边界下标。



/* ------ 动态规划法（Dynamic Program）------ */
/*
动态规划法的一般思路：
将待求解问题分解为若干个可能重叠的子问题，用一个表记录所有已解决的子问题的答案，在需要时找出已求得的答案。
1. 找出最优解的性质，刻画其结构特征。
2. 递归地定义最优解的值。
3. 以自底向上的方式计算出最优值。
4. 根据计算最优值时得到的信息，构造一个最优解。
*/


// 0-1背包问题
/*
有n个物品，第i个物品的价值为vi，重量为wi；背包的容量为W；问如何装包（物品集合x）可以使总物品价值最大。
目标函数 ，max∑vixi
约束条件 ，∑wixi <= W, xi∈{0, 1}
1. 分析出最优解的性质。可以将问题的求解过程看作是进行一系列的决策过程（即哪些放、哪些不放背包）。
（若一个问题的最优解包含物品n（xn=1），则x1...xn-1一定构成子问题1...n-1在容量W-wn时的最优解；）
（若一个问题的最优解不含物品n（xn=0），则x1...xn-1一定构成子问题1...n-1在容量W时的最优解.）
2. 递归定义最优解的值。设c[i,w]表示容量为w时i个物品导致的最优解的总价值。
（c[i,w]: 0，i=0或w=0; c[i-1, w], wi>w; max{c[i-1, w-wi]+vi, c[i-1, w]}, i>0或wi<=w.）
3. 计算最优解的值。
4. 构造问题最优解。
（时间复杂度（O(nW)））
*/
int** KnapsackDP(int n, int W, int* Weights, float* Values);  //n.w是物品数量和背包容量，Weights,Values是物品重量和价值数组。
void OutputKnapsackDP(int n, int W, int* Weights, float* Values, int** c);  //c是物品装包方案。


// 最长公共子序列
/*
求解两个序列（X=x1x2...xm, Y=y1y2...yn）的最长公共子序列(Z=z1...zk)，其中子序列下标递增且不一定连续。
1. 分析最优解的结构。
（若xm=ym，则zk=xm=yn，且Zk-1是Xm-1,Yn-1的一个最长公共子序列；）
（若xm!=ym，且zk!=xm, 则Z是Xm-1,Y的一个最长公共子序列；）
（若xm!=ym，且zk!=ym, 则Z是X,Yn-1的一个最长公共子序列.）
2. 递归定义最优解的值。设l[i, j]是Xi, Yj的最长公共子序列的长度。
（l[i, j]: 0, i*j=0; l[i-1, j-1]+1，xi=yj; max(l[i-1, j], l[i, j-1]), xi!= yj.）
3. 计算最优解。自底向上求出最优解的值。
4. 构造最优解。
（时间复杂度（O(nm)））
*/
int** Lcs_Length(const char* str1, const char* str2, int str1_len, int str2_len); // str1, str2是序列X,Y.
void OutputLcs(const char* str1, const int** b, int str1_len, int str2_len);  // b是最大公共子序列。



/* ------ 贪心算法（Greedy Algorithm） ------ */
/*
贪心算法类似动态规划算法，但是其整体最优解可以通过一系列局部最优解的选择来得到。
贪心算法不一定可以得到整体最优解。
*/


// 背包问题
/*
背包问题类似0-1背包问题，差别在物品可以部分装包，即0<= xi <=1。
三种思路：
1. 按最大价值优先原则装包。
2. 按最小重量优先原则装包。
3. 按最大单位重量价值优先原则装包。
*/
float* GreedyKnapsack(int n, int W, int* Weights, float* Values, float* VW);  //VW是单位重量价值数组。



/* ------ 回溯法（BackTrack Algorithm）------ */
/*
回溯法的一般思路：
在包含问题的所有解的解空间树中，按照深度优先的策略，从更节点出发搜索解空间树。算法搜索至解空间树的任一节点时，
总是先判断该节点是否肯定不包含问题的解。若是，则跳过以该节点为根的子树，逐层返回祖先节点；否则，进入该树搜索。
1. 定义问题的解空间。
2. 确定易于搜索的解空间结构。
3. 设计合理高效的界限函数剪枝。
4. 以深度优先的方式搜索解空间。

回溯法的实现框架：
1. 非递归方式。
    计算解X的第一个元素的候选集S
    k <- 1
    while k > 0 do
    	while Sk != Φ do
    	xk <- Sk的下一个元素
    	Sk <- Sk-{xk}
    	if X={x1, x2, ..., xk}是问题的解
    		then 输出X
    	k <- k+1
    	计算解X的第k个元素的候选集Sk
    	k <- k-1
    return
2. 递归方式
	if X={x1,x2,...,xk}是问题的解
		then 输出X
		else k <- k+1
			计算解X的第k个元素的候选集合Sk
			while Sk != Φ do
				xk <- Sk的下一个元素
				Sk <- Sk-{xk}
				BackTrackingDFS(X,k)
	return 
*/


// 0-1背包问题
// Profit_Gained, Weight_Used, k是当前已获得的价值，背包重量，已确定的物品。
float Bound(float* Values, int* Weights, float* VW, int n, int W, float Profit_Gained, int Weight_Used, int k);
int* Knapsack(float* Values, int* Weights, float* VW, int n, int W);




