
参考 https://zhuanlan.zhihu.com/p/54050093
侵删
##右值引用

> 1. 在C++11中，每个表达式有两个属性：类型和值类型
> 2. 值类型包括：lvalue、prvalue与xrvalue
> 3. lvalue我们称为左值
> 4. prvalue与xrvalue被称为右值

###先介绍一下左值引用
左值引用必须初始化：
```
int x = 10;
int& rx = x;
//int& r; //error
```
我们可以理解rx是x的一个别名，操作rx就和操作x的效果一样
但是引用也分为const引用与non-const引用，对于non-const引用，其只能用non-const左值来
```
int x = 20;
int& rx1 = x;   // non-const引用可以被non-const左值初始化
const int y = 10;
// int& rx2 = y;  // 非法：non-const引用不能被const左值初始化
// int& rx3 = 10; // 非法：non-const引用不能被右值初始化
```
但是const引用可以用右值初始化：
```
int x = 10;
const int cx = 20;
const int& rx1 = x;   // const引用可以被non-const左值初始化
const int& rx2 = cx;  // const引用可以被const左值初始化
const int& rx3 = 9;   // const引用可以被右值初始化
```
###关于右值引用
> 在C++11中引入了右值引用，就是为了重用右值。定义右值引用需要使用&&
`int&& rrx = 200;`

1. 右值引用一定不能被左值所初始化，只能用右值初始化
2. 因为右值引用的目的是为了延长用来初始化对象的生命周期，对于左值，其生命周期与其作用域有关，你没有必要去延长

```
int x = 20;    // 左值
//int&& rrx1 = x;   // 非法：右值引用无法被左值初始化
//const int&& rrx2 = x;  // 非法：右值引用无法被左值初始化
```
```
int x = 20;   // 左值
int&& rx = x * 2;  // x*2的结果是一个右值，rx延长其生命周期
int y = rx + 2;   // 因此你可以重用它：42
rx = 100;         // 一旦你初始化一个右值引用变量，该变量就成为了一个左值，可以被赋值
```

####用于函数参数

**左右值是函数签名的一部分**

```
// 接收左值
void fun(int& lref){
    cout << "l-value reference\n";
}
// 接收右值
void fun(int&& rref){
    cout << "r-value reference\n";
}
int main(){
    int x = 10;
    fun(x);   // output: l-value reference
    fun(10);  // output: r-value reference
}
```

##移动语义

> 将内存的所有权从一个对象转移到另外一个对象，高效的移动用来替换效率低下的复制，对象的移动语义需要实现移动构造函数（move constructor）和移动赋值运算符（move assignment operator）。

```
template <typename T>
class DynamicArray{
public:
    explicit DynamicArray(int size) :
        m_size{ size }, m_array{ new T[size] }{
        cout << "Constructor: dynamic array is created!\n";
    }
    virtual ~DynamicArray(){
        delete[] m_array;
        cout << "Destructor: dynamic array is destroyed!\n";
    }

    // 复制构造函数
    DynamicArray(const DynamicArray& rhs) :
        m_size{ rhs.m_size } {

        m_array = new T[m_size];
        for (int i = 0; i < m_size; ++i)
            m_array[i] = rhs.m_array[i];
        cout << "Copy constructor: dynamic array is created!\n";
    }

    // 复制赋值操作符
    DynamicArray& operator=(const DynamicArray& rhs){
        cout << "Copy assignment operator is called\n";
        if (this == &rhs)
            return *this;
        delete[] m_array;
        m_size = rhs.m_size;
        m_array = new T[m_size];
        for (int i = 0; i < m_size; ++i)
            m_array[i] = rhs.m_array[i];
        return *this;
    }
    // 索引运算符
    T& operator[](int index) {
        // 不进行边界检查
        return m_array[index];
    }
    const T& operator[](int index) const{
        return m_array[index];
    }
    int size() const { return m_size; }
private:
    T* m_array;
    int m_size;
};
```

生产int动态数组的工厂函数
```
DynamicArray<int> arrayFactor(int size)
{
    DynamicArray<int> arr{ size };
    return arr;
}
```
测试：
```
int main(){
    {
        DynamicArray<int> arr = arrayFactor(10);
    }
    return 0;
}
```

输出：
```
Constructor: dynamic array is created!
Copy constructor: dynamic array is created!
Destructor: dynamic array is destroyed!
Destructor: dynamic array is destroyed!
```
> 1. 首先，你调用arrayFactor函数，内部创建了一个动态数组，所以普通构造函数被调用
> 2. 然后将这个动态数组返回，但是这个对象是函数内部的，函数外是无法获得的，所以要生成一个临时对象，然后用这个动态数组初始化，函数最终返回的是临时对象
> 3. 我们知道这个动态数组即将消亡，所以其是右值，那么在构建临时对象时，会调用复制构造函数
> 4. 因为你返回的这个临时对象又拿去初始化另外一个对象arr，当然调用也是复制构造函数。调用两次复制构造函数完全没有必要，编译器也会这么想，所以将其优化
> 5. 直接拿函数内部创建的动态数组去初始化arr。所以仅有一次复制构造函数被调用，但是一旦完成arr的创建，那个动态数组对象就被析构了
> 6. 最后arr离开其作用域被析构。我们看到编译器尽管做了优化，但是还是导致对象被创建了两次

直接将其申请的空间直接转移到arr

移动构造函数与移动赋值操作符的实现，相比复制构造函数与复制赋值操作符，前者没有再分配内存，而是实现内存所有权转移。
```
template <typename T>
class DynamicArray{
public:
    // ...
    // 移动构造函数
    DynamicArray(DynamicArray&& rhs) :
        m_size{ rhs.m_size }, m_array{rhs.m_array}{
        rhs.m_size = 0;
        rhs.m_array = nullptr;
        cout << "Move constructor: dynamic array is moved!\n";
    }
    // 移动赋值操作符
    DynamicArray& operator=(DynamicArray&& rhs){
        cout << "Move assignment operator is called\n";
        if (this == &rhs)
            return *this;
        delete[] m_array;
        m_size = rhs.m_size;
        m_array = rhs.m_array;
        rhs.m_size = 0;
        rhs.m_array = nullptr;
        return *this;
    }
};
```
```
Constructor: dynamic array is created!
Move constructor: dynamic array is moved!
Destructor: dynamic array is destroyed!
Destructor: dynamic array is destroyed!
```
> 1. 调用的是移动构造函数，那么函数内部申请的动态数组直接被转移到arr从而减少了一份相同内存的申请与释放。
> 2. 注意析构函数被调用两次，这是因为尽管内部进行了内存转移，但是临时对象依然存在，只不过第一次析构函数析构的是一个nullptr
> 3. 这个4个函数一旦自己实现一个，就应该养成实现另外3个的习惯。

###std::move
**我们知道对象的移动语义的实现是依靠移动构造函数和移动赋值操作符**
**但是前提是你传入的必须是右值**

我们通过`std::move`将v1转化为右值，从激发v3的移动构造函数，实现移动语义

```
vector<int> v1{1, 2, 3, 4};
vector<int> v2 = v1;             // 此时调用复制构造函数，v2是v1的副本
vector<int> v3 = std::move(v1);  // 此时调用移动构造函数，v3与v1交换：v1为空，v3为{1, 2, 3, 4}
```
`std::swap`的实现如下
```
template <typename T>
void swap(T& a, T& b)
{
    T temp{std::move(a)};   // 调用移动构造函数
    a = std::move(b);       // 调用移动赋值运算符
    b = std::move(tmp);     // 调用移动赋值运算符
}
```
仅通过三次移动，实现两个对象的交换，由于没有复制，效率更高

`std::move`函数内部实现，下面给出一个简化版本的`std::move`
其实`std::move`函数并不“移动”，它仅仅进行了类型转换
```
template <typename T>
typename remove_reference<T>::type&& move(T&& param){
    using ReturnType = typename remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}
```

举一个由于误用std::move而无效的例子:
```
class Annotation{
public:
    explicit Annotation(const string& text):
        m_text {std::move(text)}{}
    const string& getText() const { return m_text; }
private:
    string m_text;
};
```
```
int main(){
    string text{ "hello" };
    Annotation ant{ text };
    cout << ant.getText() << endl;  // output: hello
    cout << text << endl;           // output: hello 不是空，移动语义没有实现
    return 0;
}
```
> 1. 从直观上看，假如移动语义成功了，那么text会发生改变，这会违反其const属性
> 2. 其实，std::move函数会在推导形参时会保持形参的const属性，所以其最终返回的是一个const右值引用类型

`string`的内部有两个构造函数可能会匹配

```
class string{
    // ...
    string(const string& rhs);   // 复制构造函数
    string(string&& rhs);    // 移动构造函数
}
```

移动构造函数不会被匹配，因为不接受const对象
复制构造函数会匹配,const左值引用可以接收右值，const右值更可以
调用了复制构造函数，那么移动语义当然无法实现
如果想接下来进行移动，那不要把std::move引用在const对象上

##完美转发

> 定义一个函数模板，该函数模板可以接收任意类型参数，然后将参数转发给其它目标函数，且保证目标函数接受的参数其类型与传递给模板函数的类型相同。

完美转发的实现要依靠`std::forward`函数
不论传入wrapper的参数是左值还是右值，一旦传入之后，param一定是左值

```
// 目标函数
void foo(const string& str);   // 接收左值
void foo(string&& str);        // 接收右值
template <typename T>
void wrapper(T&& param){
    foo(std::forward<T>(param));  // 完美转发
}
```
> 1. 当一个类型为string类型的右值传递给wrapper时，T被推导为string，param为右值引用类型，但是一旦传入后，param就变成了左值，所以你直接转发给foo函数，将丢失param的右值属性，那么std::forward就确保传入foo的值还是一个右值
> 2. 当类型为const string的左值传递给wrapper时，T被推导为const string&，param为const左值引用类型，传入后，param仍为const左值类型，所以你直接转发给foo函数，没有问题，此时应用std::forward函数可以看成什么也没有做
> 3. 当类型为string的左值传递给wrapper时，T被推导为string&，param为左值引用类型，传入后，param仍为左值类型，所以你直接转发给foo函数，没有问题，此时应用std::forward函数可以看成什么也没有做

std::forward函数确保传入的右值依然转发为右值，而对左值传入不做处理。
实现如下：
```
template<typename T> 
T&& forward(typename remove_reference<T>::type& param) 
{
    return static_cast<T&&>(param);
}
```
std::foward函数是有条件地将传入的参数转化为右值，而std::move无条件地将参数转化为右值，这是两者的区别。但是本质上，两者什么没有做，做多就是进行了一次类型转换。
