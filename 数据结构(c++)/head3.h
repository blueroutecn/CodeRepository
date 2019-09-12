#pragma once
#include <iostream>


//===============================================
//栈
//===============================================

// 栈结构体
typedef struct Node
{
	int data;
	Node *next;
}*Stack; // 栈结构体指针变量类型


// 创建/初始化一个空栈
Stack InitStack();

// 判断栈是否为空
int isEmpty(Stack s);

// 栈顶入栈，更新栈顶指针
int Push(Stack s, int data);

// 栈顶出栈，更新栈顶指针
int Pop(Stack s);

// 读取栈顶元素
int Top(Stack s, Node &n);

// 清理栈
int Clear(Stack s);



//===============================================
//队列
//===============================================

#define MAXQSIZE 100

// 循环队列结构体
typedef struct SqQueue
{
	int *base;        // 循环队列的存储空间，存放整数类型数据
	int front;        // 指示队头，队头指针
	int rear;         // 指示队尾，队尾指针
}*Queue;


// 创建并初始化一个队列
Queue InitQueue();

// 判断队列是否为空
int isEmpty(Queue q);

// 判断队列是否为满
int isFull(Queue q);

// 入队，更新队尾指针
int EnQueue(Queue q, int data);

// 出队，更新队头指针
int DelQueue(Queue q,int &data);

// 读取队头元素
int FrontQueue(Queue q,int &data);

// 清空队列
int ClearQueue(Queue q);



//===============================================
//二叉树
//===============================================

// 二叉树结构体
typedef struct BiTnode
{
	int data;
	BiTnode *lchild, *rchild;
}*BiTree; // 二叉树结构体指针变量类型


// 先序遍历
void PreOrder(BiTree root);

// 中序遍历
void InOrder(BiTree root);

// 后序遍历
void PostOrder(BiTree root);


// 先序遍历（非递归）
int PreOrderTraverse(BiTree root);

// 中序遍历（非递归）
int InOrderTraverse(BiTree root);

// 后序遍历（非递归）
int PostOrderTraverse(BiTree root);


// 层序遍历
int LevelOrder(BiTree root);



//===============================================
//图
//===============================================
#define SIZE 10


//邻接矩阵存储
/*
1. 存储顶点：用一个连续的空间存储n个顶点，
如有5个顶点{a,b,c,d,e}，存储为char VertexList[] = {‘a’,’b’,’c’,’d’,’e’};
2. 存储顶点之间的边：将由n个顶点组成的边用一个n*n的矩阵来存储，
如果两个顶点之间有边，则表示为1，否则为0。

A --------------B          A   B   C   D   E
|             . |       A  0   1   0   0   1
|           .   |       B  1   0   1   1   0
|        D      |  =>>  C  0   1   0   1   1
|     .     .   |       D  0   1   1   0   1
|  .          . |       E  1   0   1   1   0
E --------------C

*/

class Graph
{
public:
	Graph();
	~Graph();

	void InsertVertex(char v);
	int GetVertexIndex(char v);
	void InsertEdge(char v1, char v2);
	void ShowGraph();
	int GetEdgeNum(char v);
	void DeleteVertex(char v);
	void DeleteEdge(char v1, char v2);

private:
	int MaxVertex;     // 图中最大节点数量
	int NumVertex;     // 图中节点个数
	int NumEdge;       // 图中边数量（各点的边数量加和值）
	char *Vertex;      // 顶点数组
	int **Edge;        // 邻接矩阵
};


//邻接表存储
/*
用数组存储顶点，用链表存储和顶点相关联的边。边值为当前顶点在数组中的下标。

A --------------B       A  0   ->1    ->4    ->NULL
|             . |       B  1   ->0	  ->2	 ->3	 ->NULL
|           .   |       C  2   ->1	  ->3	 ->4	 ->NULL
|        D      |  =>>  D  3   ->1	  ->2	 ->4	 ->NULL
|     .     .   |       E  4   ->2	  ->3	 ->0	 ->NULL
|  .          . |       
E --------------C

*/

struct Edge 
{
	Edge(int v):destvalue(v),link(NULL){}

	int destvalue;
	Edge* link;
};

struct Vertex
{
	Vertex() :list(NULL){}

	char data;
	Edge* list;
};

class GraphLink
{
public:
	GraphLink();
	~GraphLink();

	void InsertVertex(char v);
	int GetVertexIndex(char v);
	void InsertEdge(char v1, char v2);
	void show();
	void DeleteEdge(char v1, char v2);
	void DeleteVertex(char v);

private:
	int MaxVertex;
	int NumVertex;
	int NumEdge;
	Vertex* VertexTable;
};















