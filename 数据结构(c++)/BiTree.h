typedef int ElemType;       // 元素类型

typedef struct Node
{
	ElemType data;         //数据域，用于存储节点数据
	Node* lchild;          //指针，指向该节点的左子节点或前驱节点
	Node* rchild;          //指针，指向该节点的右子节点或后继节点
	int ltag, rtag;        //标志，指示指针指向子节点还是前驱后继节点
}ThrNode, *ThrTree;


ThrTree pre = NULL;         //全局变量，始终指向访问的前一个节点
 
// 中序线索化二叉树
void InThreading(ThrTree p)
{
	if(p)
	{
		InThreading(p->lchild);
		if(!p->lchild)     // 如果当前节点左子节点不存在，则其左指针指向前一个节点
		{
			p->ltag = 1;   // 修改标识符
			p->lchild = pre;
		}
		
		if((pre!=NULL)&&(!pre->rchild))  // 如果前一个节点的右子节点不存在，则其右指针指向当前节点
		{
			pre->rtag = 1;
			pre->rchild = p;
		}
		
		pre = p;
		InThreading(p->rchild);
	}
}



// 先序线索化二叉树
void PreThreading(ThrTree p)
{
	if(p)
	{
		if(!p->lchild)    
		{
			p->ltag = 1;   
			p->lchild = pre;
		}
		
		if((pre!=NULL)&&(!pre->rchild))  
		{
			pre->rtag = 1;
			pre->rchild = p;
		}
		
		pre = p;
		if(p->ltag != 1)
			PreThreading(p->lchild);
		if(p->rtag != 1)
			PreThreading(p->rchild);
	}
}




























