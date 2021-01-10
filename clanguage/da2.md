#数据结构 day02
##栈
**栈是一个特殊的线性表**

使用线性表模拟栈
开口方向向后
如果选择头插来操作栈，就会涉及到大量元素移动，所以选择尾插比较好

```
#include "seqlist.h"
#include "My_Stack.h"
//使用day01的顺序表的静态库
#pragma comment(lib,"seqlist.lib")
Stack* Stack_Create(int cap) {
	return SeqList_Create(cap);
}
void Stack_Destroy(Stack* stack) {
	SeqList_Destroy(stack);
}
void Stack_Clear(Stack* stack) {
	SeqList_Clear(stack);
}
//向栈中压如元素相当于在线性表尾部插入元素
int Stack_Push(Stack* stack, void* item) {
	return SeqList_Insert(stack, item, SeqList_GetLenth(stack));
}
//从栈中弹出元素相当于从线性表尾部拿出元素
void* Stack_Pop(Stack* stack) {
	return SeqList_Delete(stack, SeqList_GetLenth(stack) - 1);
}
void* Stack_Top(Stack* stack) {
	return SeqList_Get(stack, SeqList_GetLenth(stack) - 1);
}
int Stack_Size(Stack* stack) {
	return SeqList_GetLenth(stack);
}
```

##栈的运用

###实现编译器检测括号成对的程序

**思路：**

1. 遇见普通符号忽略
2. 遇见左符号压栈
3. 遇见右符号弹出栈顶符号进行匹配
	* 匹配成功，进行下一步
	* 失败，停止，报错
4. 结束后栈中无内容成功，有则失败

###中缀表达式转换后缀表达式

**思路：**

1. 数字直接输出
2. 左括号进栈
	*  运算符：与栈顶的符号比较
	*  栈顶优先级低，进栈
	*  栈顶符号优先级不低，将栈顶符号弹出并且输出，之后进栈
3. 右括号：将栈顶的符号弹出并直接匹配左括号

###后缀表达式的计算
例：8 3 1 - 5 * + 
1. 对于数字直接进栈
2. 符号：
	* 从栈中弹出右操作数
	* 从栈中弹出左操作数
	* 计算出结果后，将结果压入栈中
3. 栈中的唯一数字是计算结果


