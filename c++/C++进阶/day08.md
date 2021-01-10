#lambda表达式

基本形态：

```
[]mutable or exception->type(){}; 
```

##`[]`中的是函数对象参数

1. 可以为空，表示没有函数对象参数
2. 填`=`，表示该表达式可见局部变量为值传递(会拷贝)。
3. 填`&`，表示所在局部变量为传引用。
4. 填写具体的变量名如 a，表示a为值传递。
5. 如果填写`&a`，表示a为传引用。
6. 如果有多个使用`,`隔开，如`=,&a,&b`，表示除了a，b为引用，其他都是值传递。

##mutable 或 exception

> 可以省略

加上`mutable`后可以修改传进来的拷贝，而不是值本身。
加上`exception`后申明指定函数的异常，如`throw(int)`

##`->`返回值

相当于`int func();`中的`int`
如果不写，编译器会自动推导。

##`{}`函数体

实例
```
[](int x, int y){return x + y;}   //这里没写返回值，但编译器会推到出来是int
[](int& x){++x;}

[] (int x, int y) -> int { int z = x + y; return z; }//指定

```

```
std::vector<int> some_list = {1,2,3,4,5};
int total = 0;

std::for_each(begin(some_list), end(some_list), [&total](int x)
{
    total += x;
});
```

使用for_each 传入匿名函数对象total捕获局部引用，x为传入参数，传参调
用栈是通过 for_each
当这个代码段执行完后，tatol的值是vector中所有元素的和

显式调用 auto  匿名函数是一个仿函数，可以被存储或当成参数。
```
int main(){
	std::function<double(double)> f0 = [](double x){return 1;};
	auto f1 = [](double x){return x};
	decltype(f0) farr[3] = {f0,f1,[](double x){return x*x;}};//decltype 返回一个类型
	std::vector<decltype(f0)> fvec = {f0,f1}; //同理
	return 0;
}
```

