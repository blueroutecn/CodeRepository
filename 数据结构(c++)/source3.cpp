#include "head3.h"
#include<stack>
#include<queue>



//===============================================
//栈
//===============================================

Stack InitStack()
{
	Stack s = (Node*)malloc(sizeof(Node));
	if (s == NULL) return NULL;

	s->data = -1;
	s->next = NULL;

	return s;
}
 
int isEmpty(Stack s)
{
	return ((s->next) == NULL) ? 1 : 0;
}

int Push(Stack s, int data)
{
	Node *node = (Node*)malloc(sizeof(Node));
	//Stack *node = (Stack*)malloc(sizeof(Stack)); 错误。
	if (node == NULL) return 0;
	
	node->data = data;

	Stack tmp = s->next;
	node->next = tmp;
	s->next = node;
	return 1;
}

int Pop(Stack s)
{
	if (s->next == NULL) return 1;
	
	Stack tmp = s->next; 
	s->next = tmp->next; 
	
	if (tmp != NULL) free(tmp);
	return 1;
}

int Top(Stack s, Node &n)
{
	if (s->next == NULL) return 0;

	n = *(s->next);
	return 1;
}

int Clear(Stack s)
{
	if (s->next == NULL) return 1;

	while (s->next != NULL)
	{
		Stack tmp = s->next;
		s->next = tmp->next;
		free(tmp);
	}
	return 1;
}



//===============================================
//队列
//===============================================

Queue InitQueue()
{
	Queue que = (SqQueue*)malloc(sizeof(SqQueue));
	if (!que) return NULL;

	que->base = (int*)malloc(sizeof(int)*MAXQSIZE);
	if (!que->base) return NULL;

	que->front = 0;
	que->rear = 0;

	return que;
}

int isEmpty(Queue q)
{
	if (q->front == q->rear) return 1;
	else return 0;
}

int isFull(Queue q)
{
	if ((q->rear + 1) % MAXQSIZE == q->front) return 1;
	else return 0;
}

int EnQueue(Queue q, int data)
{
	if ((q->rear + 1) % MAXQSIZE == q->front) return 0;

	q->base[q->rear] = data;
	q->rear = (q->rear + 1) % MAXQSIZE;
	return 1;
}

int DelQueue(Queue q, int &data)
{
	if ((q->rear + 1) % MAXQSIZE == q->front) return 0;

	data = q->base[q->front];  // 数据还在，但是不作维护（废弃）
	q->front = (q->front + 1) % MAXQSIZE; // 队头也随队尾，向前移动
	return 1;
}

int FrontQueue(Queue q, int &data)
{
	if ((q->rear + 1) % MAXQSIZE == q->front) return 0;

	data = q->base[q->front];
	return 1;
}

int ClearQueue(Queue q)
{
	if (q->front == q->rear) return 1;

	while (q->front != q->rear)
	{
		q->front = (q->front + 1) % MAXQSIZE;
	}

	return 1;
}



//===============================================
//二叉树
//===============================================

void PreOrder(BiTree root)
{
	if (root != NULL)
	{
		printf("%d", root->data);
		PreOrder(root->lchild);
		PreOrder(root->rchild);
	}
}

void InOrder(BiTree root)
{
	if (root != NULL)
	{
		InOrder(root->lchild);
		printf("%d", root->data);
		InOrder(root->rchild);
	}
}

void PostOrder(BiTree root)
{
	if (root != NULL)
	{
		PostOrder(root->lchild);
		PostOrder(root->rchild);
		printf("%d", root->data);
	}
}


int PreOrderTraverse(BiTree root)
{
	if (root == NULL) return 0;

	std::stack<BiTree> st;
	st.push(root);

	while (!st.empty())
	{
		root = st.top();
		st.pop();

		printf("%d\n", root->data);
	
		if (root->rchild != NULL)
		{
			st.push(root->rchild);// 入栈
		}

		if (root->lchild != NULL)
		{
			st.push(root->lchild);// 入栈
		}
	}	
	return 1;
}

int InOrderTraverse(BiTree root)
{
	if (root == NULL) return 0;

	std::stack<BiTree> st;
	while (!st.empty() || root != NULL)
	{
		while (root)
		{
			st.push(root);
			root = root->lchild;
		}
		root = st.top();
		st.pop();

		printf("%d\n", root->data);
		root = root->rchild;
	}
	return 1;
}

int PostOrderTraverse(BiTree root)
{
	if (root == NULL) return 0;

	std::stack<BiTree> st;
	BiTree flag = NULL;
	while (root != NULL || st.empty())
	{
		while (root != NULL)
		{
			st.push(root);
			root = root->lchild;
		}
		root = st.top();
		st.pop();

		if (root->rchild == NULL || root->rchild == flag)
		{
			printf("%d\n", root->data);
			flag = root;
			root = NULL;
		}
		else
		{
			st.push(root);
			root = root->rchild;
		}
	}
	return 1;
}


int LevelOrder(BiTree root)
{
	if (root == NULL) return 0;

	std::queue<BiTree> que;
	//int high = 0;
	que.push(root);

	while (!que.empty())
	{
		//high++;
		int len = que.size();

		while (len--)
		{
			root = que.front();
			que.pop();

			printf("%d\n", root->data); 
			if (root->lchild != NULL)
			{
				que.push(root->lchild);
			}

			if (root->rchild != NULL)
			{
				que.push(root->rchild);
			}
		}
	}
	return 1;
}


 
//===============================================
//图
//===============================================

Graph::Graph()
{
	MaxVertex = SIZE;
	NumVertex = NumEdge = 0;
	Vertex = new char[MaxVertex];
	Edge = new int*[MaxVertex];

	int i, j;
	for (i = 0; i < MaxVertex; i++)
	{
		Edge[i] = new int[MaxVertex];
	}
	for (i = 0; i < MaxVertex; i++)
	{
		for (j = 0; j < MaxVertex; j++)
		{
			Edge[i][j] = 0;
		}
	}
}

Graph::~Graph()
{
	delete[]Vertex;
	Vertex = NULL;

	for (int i = 0; i < MaxVertex; i++)
	{
		delete[]Edge[i];
		Edge[i] = NULL;
	}

	delete[]Edge;
	Edge = NULL;
	NumVertex = 0;
}

void Graph::InsertVertex(char v)
{
	if (NumVertex >= MaxVertex) return;

	Vertex[NumVertex++] = v;
}

int Graph::GetVertexIndex(char v)
{
	int i;
	for (i = 0; i < NumVertex; i++)
	{
		if (Vertex[i] == v) return i;
	}

	return -1;
}

void Graph::InsertEdge(char v1, char v2)
// 插入一条由点v1和v2组成的边
{
	int p1 = GetVertexIndex(v1);
	int p2 = GetVertexIndex(v2);
	if (p1 == -1 || p2 == -1) return;

	Edge[p1][p2] = Edge[p2][p1] = 1;  // 同时更改两组数据
	NumEdge++;
}

void Graph::ShowGraph()
{
	using std::cout;
	using std::endl;

	int i, j;
	std::cout << "  ";
	for (i = 0; i < NumVertex; i++)
	{
		cout << Vertex[i] << "  ";
	}	cout << endl;

	for (i = 0; i < NumVertex; i++)
	{
		cout << Vertex[i] << "  ";
		for (j = 0; j < NumVertex; j++)
		{
			cout << Edge[i][j] << "  ";
		}cout << endl;
	}
}

int Graph::GetEdgeNum(char v)
// 获取点的边数
{
	int p = GetVertexIndex(v);
	if (p == -1) return 0;

	int n = 0;
	for (int i = 0; i < NumVertex; i++)
	{
		if (Edge[p][i] == 1) n++;
	}

	return n;
}

void Graph::DeleteVertex(char v)
{
	int p = GetVertexIndex(v);
	if (p == -1) return;

	int i, j;
	int n = GetEdgeNum(v);
	for (i = p; i < NumVertex; i++)     // 从顶点数组中删除顶点（数据前移）
	{
		Vertex[i] = Vertex[i + 1];
	}

	for (i = p; i < NumVertex-1; i++)   // 从邻接矩阵中删除顶点（删除行）
	{
		for (j = 0; j < NumVertex; j++)
		{
			Edge[i][j] = Edge[i+1][j];  // 将该点的行覆盖掉（行上移）
		}
	}

	for (i = 0; i < NumVertex-1; i++)   // 行数减少一
	{
		for (j = p; j < NumVertex-1; j++)
		{
			Edge[i][j] = Edge[i][j + 1];// 将该点的列覆盖掉（列左移）
		}
	}	

	NumVertex--;
	NumEdge -= n;
}

void Graph::DeleteEdge(char v1, char v2)
// 删除图中的一条边
{
	int p1 = GetVertexIndex(v1);
	int p2 = GetVertexIndex(v2);
	if (p1 == -1 || p2 == -1) return;

	if (Edge[p1][p2] == 0) return;
	Edge[p1][p2] = Edge[p2][p1] = 0;

	NumEdge--;
}


GraphLink::GraphLink()
{
	MaxVertex = SIZE;
	NumVertex = NumEdge = 0;
	VertexTable = new Vertex[MaxVertex];
}

GraphLink::~GraphLink()
{
	Edge *p = NULL;
	for (int i = 0; i < NumVertex; ++i)
	{
		p = VertexTable[i].list;
		while (p)
		{
			VertexTable[i].list = p->link;
			delete p;
			p = VertexTable[i].list;
		}
	}
	delete[]VertexTable;
	VertexTable = NULL;
}

void GraphLink::InsertVertex(char v)
{
	if (NumVertex >= MaxVertex) return;

	VertexTable[NumVertex++].data = v;
}

int GraphLink::GetVertexIndex(char v)
{
	for (int i = 0; i < NumVertex; i++)
	{
		if (VertexTable[i].data == v) return i;
	}
	return -1;
}


void GraphLink::InsertEdge(char v1, char v2)
{
	int p1 = GetVertexIndex(v1);
	int p2 = GetVertexIndex(v2);
	if (p1 == -1 || p2 == -1) return;

	Edge* ed = new Edge(p2);          // p1 -> p2,插入v1的邻接链表
	ed->link = VertexTable[p1].list;
	VertexTable[p1].list = ed;

	ed = new Edge(p2);                // p2 -> p1,插入v2的邻接链表
	ed->link = VertexTable[p2].list;
	VertexTable[p2].list = ed;

	NumEdge++;
}

void GraphLink::show()
{
	using std::cout;
	using std::endl;

	int i;
	for (i = 0; i < NumVertex; i++)
	{
		cout << VertexTable[i].data << " -> ";
		Edge* p = VertexTable[i].list;

		while (p)
		{
			cout << p->destvalue << " -> ";
			p = p->link;
		}
		cout << " NULL " << endl;
	}
}

void GraphLink::DeleteEdge(char v1, char v2)
{
	int p1 = GetVertexIndex(v1);
	int p2 = GetVertexIndex(v2);
	if (p1 == -1 || p2 == -2) return;

	Edge *p = VertexTable[p1].list;
	Edge *s = NULL;
	while (p && p->destvalue != p2)  // p1 -> p2
	{
		s = p;
		p = p->link;
	}
	if (p == NULL) return;
	if (s == NULL) VertexTable[p1].list = p->link;// 跳过p2，指向之后的邻接点
	else s->link = p->link;
	delete p;

	p = VertexTable[p2].list;             
	s = NULL;
	while (p && p->destvalue != p1) // p2 -> p1
	{
		s = p;
		p = p->link;
	}
	if (s == NULL) VertexTable[p2].list = p->link;
	else s->link = p->link;
	delete p;

	p = NULL;
	NumEdge--;
}

void GraphLink::DeleteVertex(char v)
{
	int p = GetVertexIndex(v);
	if (p == -1) return;

	Edge *s = NULL;
	Edge *edp = VertexTable[p].list;
	while (edp)                        //删除相邻的节点中边
	{
		int destvalue = edp->destvalue;
		Edge *q = VertexTable[destvalue].list;
		s = NULL;

		while (q && q->destvalue != p)
		{
			s = q;
			q = q->link;
		}
		if (s == NULL) VertexTable[destvalue].list = q->link;
		else s->link = q->link;
		delete q;
		q = NULL;

		VertexTable[p].list = edp->link;
		delete edp;
		edp = VertexTable[p].list;
		NumEdge--;
	}

	NumVertex--;
	VertexTable[p].data = VertexTable[NumVertex].data;
	VertexTable[p].list = VertexTable[NumVertex].list;

	edp = VertexTable[p].list;
	while (edp)
	{
		int destvalue = edp->destvalue;
		s = VertexTable[destvalue].list;
		while (s && s->destvalue != NumVertex)
		{
			s = s->link;
		}
		s->destvalue = p;
		edp = edp->link;
	}


}



