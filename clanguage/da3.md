#数据结构03

##树

1. 数时一种非线性结构
2. 树是递归定义的
3. 树是m(m>=0)个互不相交的集合

树的一些常用概念：

1. 叶子节点：终端节点(没有后续)
2. 节点的度：节点有几个孩子就有几个度
3. 树的度：节点的度的最大值

树的存储：
1 把每一个结点存入数据库
2 把结点直接的关系存入数据库
结点表：
	序号1：结点序号
	序号2：孩子序号


##二叉树

> * **度<=2**
> * **有两个子结点左子树和右子树**

二叉树的性质：
* 叶子结点比度数为2的结点多1
* 对于完全二叉树编号为i的结点，其左孩子的编号是2i 右孩子是2i+1

##二叉数的结点

结点定义
* 二叉法

```
typedef struct _BitNode {
	int data;
	struct BitNode* lchild,*rchild;
}BitNode,*BitTree;
```
* 三叉法

```
typedef struct _BitNode {
	int data;
	struct BitNode* lchild,*rchild;
	struct BitNode* parents;//双亲的地址
}BitNode,*BitTree;

```

* 双亲链表：子结点中保存了双亲的位置信息

```
typedef struct BPTNode{
	int data;
	int parentsPosition; 	//指向双亲的指针//即数组下标
	char LKTag;		//左右孩子标志位
}BPTNode;

```
使用数组存储二叉树
```
typedef struct BPTree{
	BPTNode nodes[100]; //结点位置之间是分散的，我们需要存储到数组中其编号是数组下表
	int num_node;		//结点数目
	int root；		//根位置
}BPTree;
```

##二叉树的遍历


例：A(B(D,E),C)
* 先序遍历：先根再左再右		ABDEC
* 中序遍历：先左再根再右		DBEAC
* 后序遍历：先左再右在根		DEBCA

求树的高度:(递归遍历)
思路：求树的高度 == 求左子树高度和右子树高度，再选其大 + 1

```
int Depth(BitNode* T){
	
	int deptleft = 0;//左子树的高度
	int deptright = 0;//右子树的高度
	int deptval = 0;
	//递归结束条件
	if(T == NULL){
		deptval = 0;
		return deptval;
	}
	deptleft = Depth(T->lchild); //左子树的高度递归求出
	deptright = Depth(T->rchild);//右子树的高度
	//大的一方+1
	deptval = 1 + ((deptleft>deptright) ? deptleft : deptright);
	
	return deptval;
}
```

非递归遍历：

思路：
1.	如果节点有左子树，该节点入栈，并指向左子树重复步骤1
	如果没有，访问该节点
2.	如果有右子树，指向该右子树，重复1
	如果没有右子树(结点访问完毕)，根据栈顶指示回退，访问栈顶元素，访问右子树重复1
如果栈为空遍历结束

```
BitNode* goLeft(BitNode* T,stack<BitNode* T> &s){
	if(T == NULL){
		return NULL;
	}
	//如果节点有左子树，该节点入栈，并指向左子树重复步骤1
	while(T->lchild != NULL){
		s.push(T);
		T = T->lchild;
	}
	//如果没有，访问该节点
	return T;
}

void Inorder(BitNode* T){
	BitNode* t = NULL;
	stack<BitNode*> s;
	
	t = goLeft(T,s);
	
	while(true){
		printf("%d\n",t->data);
		//如果有右子树，指向该右子树，重复1
		if(t->rchild != NULL){
			t = goLeft(t->rchild);//右子树中遍历起点
		}
		//如果没有右子树(结点访问完毕)，根据栈顶指示回退
		else if(!s.empty()){
			t = s.top();
			s.pop();
		}
		//如果没有右子树,栈为空,遍历结束
		else{
			t = NULL;
		}
	}
}
```


##求copy树

> 对一个已经存在的二叉树进行拷贝

```
BitNode* CopyTree(BitNode* T){
	BitNode* newNode = NULL;
	BitNode* newLp = NULL;
	BitNode* newRp = NULL;
	if(T == NULL){
		return NULL;
	}
	if(T->lchild != NULL){
		newLp = CopyTree(T->lchild);
	}
	else{
		newLp = NULL;
	}
	
	if(T->rchild != NULL){
		newRp = CopyTree(T->rchild);
	}
	else{
		newRp = NULL;
	}
	
	//malloc根结点
	newNode = (BitNode* )malloc(sizeof(BitNode));
	if(new == NULL){
		return NULL;
	}
	
	newNode->lchild = newLp;
	newNode->rchild = newRp;
	
	newNode->data = T->data;
	
	return newNode;
}
```
##二叉树的创建
\#号法创建二叉树：所有节点若左/右子树为空使用#号填充
```
BitNode* Create_Tree){
	BitNode* node = NULL;
	BitNode* pL = NULL;
	BitNode* pR = NULL;
	char h;
	scanf("%c"，&h);
	if(h == '#')//递归结束条件
		return NULL;
	else{
		node = (BitNode*)malloc(sizeof(BitNode));
		memset(node,0,sizeof(BitNode));
		
		node->data = h;
		//递归创建左子树
		pL = Create_Tree();
		if(pL != NULL)
			node->lchild = pL;
		else node->lchild = NULL;
		//递归创建左子树
		pR = Create_Tree();
		if(pR != NULL)
			node->rchild = pR;
		else node->rchild = NULL;
	}
	
	return node;
}
```

##二叉树线索化

> 通俗地来说
> 二叉树线索化就是希望像链表一样去遍历一棵树

**解决方法：使用空闲的指针域**
增加两个标志域(LTag,RTag)存放指针域是线索化(1)的还是正常的(0)

```
void InThreading(BitNode* p){
	if(p){
		InThreading(p->lchild);//左孩子线索化
		if(!p->lchild){
			p->LTag = Thr ead;p->lchild = pre;//
		}
		if(!pre->rchild){
			pre->RTag = Thread;pre->rchild = p;
		}
		InThreading(pre->rchild);//右孩子线索化
	}
}
```



