#C++进阶
##操作符重载的运用
###conversion function 转换函数

先看一个类：
```
//分数类
class Fraction {
public:
	Fraction(int num, int den = 1.0f) :numerator(num), denominator(den) {}
	/*operator 开头后面跟 [类型] 没有返回类型 通常会加上const*/
	operator float() const {
		return (float)numerator / (float)denominator;
	}
private:
	int numerator;  //分子
	int denominator;//分母
};
int main() {
	Fraction f(3, 5);
	float d = 4 + f;//这里会调用operator float() 先将f转换成float 在和4相加
	cout << d << endl;
}
```
这个类中重载了`float()`在`float d = 4 + f;`这里会调用operator float() 先将f转换成float 在和4相加

###nom-explicit-one-argument ctor
`explicit`关键字
```

class Fraction {
public:
	//explicit Fraction(int num, int den = 1.0f) :numerator(num), denominator(den) {}
	Fraction(int num, int den = 1.0f) :numerator(num), denominator(den) {}
	operator float() const {
		return (float)numerator / (float)denominator;
	}
	Fraction operator+(const Fraction& f){
		return Fraction(this->denominator*f.numerator+f.numerator,this->denominator+f.denominator);
	}
private:
	int numerator;  //分子
	int denominator;//分母
};
int main() {
	Fraction f(3, 5);
	Fraction d = 4 + f;//编译器会考虑吧4转化成Fraction，没有explicit时编译器会这样做
	//如果加上了explicit不会选择吧4转化成Fraction，并且报错
}
```
在vector 容器中[]操作符的返回值被传入转换函数转换成bool值

###point-like class  智能指针

在智能指针中必定会对*和->进行重载

迭代器是一种智能指针其中对++和--进行重载

前缀自增 ++	成员函数：`类名& operator++(int i)const`	友元函数：`类名& operator++(类名&，int)`
后缀自增 ++	成员函数：`类名 operator++(int)`		友元函数：`类名 operator ++ (类名&,int)`

###function-like class 仿函数

（)操作符叫做function call操作符
标准库中的仿函数：
例如
```
//判断是否是类型T
template <class T>
struct identity:public unary_function<T,T>{
	const T& operator()(const T& x)const{ return x; }
};
//取出Pair类型中的第一个元素
template <class Pair>
struct select1st:public unary_function<Pair,typename Pair::first_type>{
	const typename Pair::first_type& operator()(const Pair& x)const { return x.first; }
};
```

##模板的几种形式
###模板
如果使用了函数模板
**编译器会进行两次编译**
第一次：会检查模板的语法是否正确，并保留函数头
第二次：会将模板的类型匹配后生成真正的函数


###class template 类模板
其语法如下
```
template<class T>
class complex{
	//....
private:
	T re,im;
};
```
使用时要指定类型
例如：
`complex<int> c(2,3);`

###Function template 函数模板

例如：
```
typename <class T>
inline const T& min(const T& a,const T& b){
	return b < a ? b : a;
}
```
使用时不要指定类型，类型会自动匹配

###member template 成员模板

相当于上面两种的结合
```
member template 成员模板
template <class T1,class T2>
struct pair{
	typedef T1 first_type;
	typedef T2 second_type;
	//....
	T1 first;
	T2 second;
	typename <class U1,class U2>
	pair(const pair<U1,U2>&p)：first(p.first),second(p.second){}
};
```
这种用法一般出现在构造函数的设计

如果这么使用
`pair<Derved1,Derved2> p;`
`pair<Base1,Base2> p2(p);`
	↓这个动作会转换为这样就好用到成员模板
`pair<Base1,Base2> p2(pair<Derved1,Derved2>());`

###specialization 模板特化
例如：
标准库中的hash类
```
template <class Key>
struct hash{...};

template<>
struct hash<int>{...};//如果Key是int类型会使用这段代码

template<>
struct hash<char>{...};//如果Key是char类型会使用这段代码

template<>
struct hash<long>{...};//如果Key是long类型会使用这段代码
```

###partial specialization 模板偏特化

**个数上的偏，遇到特定类型的偏特化**

```
template <typename T,typename Alloc=...>
class vector{
	//...
};
//如果放的是bool类型vector 会偏特化成如下类型
template <typename Alloc=...>
class vector<bool,Alloc>{
	//...
};
```

**范围上的偏，某种类型上的偏特化**
```
template <typename T>
class C{...};

template <typename U>//如果T是指针类型就会进行偏特化
class C<U*>{...};
```

###可变模板参数
c++11特性
```
void print(){}//当传递到最后一个参数时没有后面的一堆参数可以匹配进
//所以写一个空的print函数来时打印结束
template<typename T,typename... Types>
//可变参数函数
void print(const T& firstArg,const Types&... args){
	cout << firstArg << endl;
	print(args...);
}
```
关于reference 我们一般用于函数参数传递(pass by reference)
reference的传递速度比直接传参更快

