#数据结构

##节点定义
```
typedef struct Node_t{
	Node* pNext;
	int data;
	Node(int n = 0):pNext(NULL),data(n){}
}Node;
```

##删除当前结点时间复杂度O(1)

```
/*
   1->2->3->4
 curr↑
 curr↓
   1->3->3->4
 next↓→ →↑
 delete curr->next
   1->3 ->  4
*/

void delNode(Node* curr){
	if(curr == nullptr)
		return;
	Node* tmp = curr->next;
	curr->val = tmp->next->val;
	curr->next = tmp->next;
	delete tmp;
}
```
##DummyNode技巧

> 单链表没有前向指针避免头结点丢失

##判断链表是否有环

1 用set转存
```
bool func(ListNode* head){
	set<ListNode*> s;
	for (; head; head = head->next) {
		if(s.find(head) != s.end()) return true; 
		//如果找到了find的返回值肯定不是end // 没找到的话返回end这时候就将新节点加入
		s.insert(head);
	}
	return false;
}
```
2 快慢指针

1. p1 走一步 p2 走两步
2. p1和p2 第一次相遇后 p1回到head,p1 p2 都开始只走一步
3. 第二次相遇时p1 p2都是环的起点 此时走过的a步就是柄的长度

```
ListNode* func(ListNode* head){
	ListNode* p1 = head;
	ListNode* p2 = head;
	
	do{
		if(p2 == NULL || p2->next != NULL){
				return NULL;
		}
		p1 = p1->next;
		p2 = p2->next->next;
	}while(p1 != p2);//第一次相遇推出循环，如果没有返回就肯定有环了
	p2 = head;
	do{
		p1 = p1->next;
		p2 = p2->next;
	}while(p1 != p2);//第二次相遇肯定是环的入口节点
	return p1;
}
```

##两个单链表的交点Y字形

让长的链表先走X-Y步 在一起走，相遇时就是交点
```
int getLength(ListNode* head){
	ListNode* tmp = head;
	int ret = 0;
	if(!head)
		return 0;
	for(;tmp ;tmp = tmp->next){
		ret++;
	}
	return ret;
}
ListNode* findIntersectionNode(ListNode* head1,ListNode* head2){
	int len1 = getLength(head1);
	int len2 = getLength(head2);
	//让长的先走|len1 - len2|步
	if(len1 > len2)
		for(int i = len1 - len2; i > 0; --i){
			p1 = p1->next;
		}
	else
		for(int i = len2 - len1; i > 0; --i){
			p2 = p2->next;
		}
	//相遇时就是交点
	while(p1 != p2 && p1 && p2){
		p1 = p1->next;
		p2 = p2->next;
	}
	return p1;
}
```

##复制带有random指针的链表




