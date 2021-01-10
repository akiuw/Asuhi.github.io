#智能指针

##share_ptr接口

`shared_ptr`的接口主要是

* make_shared 构造一个shared_ptr指针
* use_count 返回引用计数
* reset 更改或放弃资源
* get 返回原始指针

对于类Object
```
class Object {
public:
	Object(int id) :m_id(id) {
		cout << "init obj" << endl;
	}
	~Object() {
		cout << "bye" << endl;
	}
	int id()const { return m_id; }
private:
	int m_id;
};
using  ObjectPtr = shared_ptr<Object>;

```
两个传参函数
```
void print(ObjectPtr obj) {
	cout << obj.use_count() << endl;//3
}
void printRef(const ObjectPtr& obj) {
	cout << obj.use_count() << endl;//2
}
```
```
static void interfaceOfSharedPtr() {
	ObjectPtr null;
	cout << null.use_count() << endl;//0
	//通过原始指针构造shared_ptr 这时new出来的资源将由智能指针接管
	ObjectPtr obj(new Object(1));
	cout << obj.use_count() << endl; // 1
	ObjectPtr obj2(obj);//当shared_ptr拷贝时引用计数会+1
	cout << obj.use_count() << endl;//2
	ObjectPtr obj3 = obj2;//同理
	cout << obj.use_count() << endl;//3
	//调用reset意味着obj2不在管理资源了所以引用计数减一
	obj2.reset();
	//或者obj2 = nullptr;
	cout << obj.use_count() << endl;//2
	ObjectPtr obj4;
	obj3.swap(obj4);//obj3和obj4交换使用权
	swap(obj3, obj4);
	cout << obj.use_count() << endl;//引用计数不变 2
	//auto p = obj.get(); // 传出一个object的原始指针
	//尽量别用
	print(obj); //传入shared_ptr的拷贝
	printRef(obj);//传入引用
	//效率上传引用会更高一些
	cout << obj.use_count() << endl;
}
```

##循环引用问题
下面两个类Parent和Child
在其成员里面有互相的shared_ptr指针

```
class Parent;
typedef shared_ptr<Parent> ParentPtr;
class Child {
public:
	ParentPtr father;
	~Child() { cout << "bye child" << endl; }
};
typedef shared_ptr<Child> ChildPtr;
class Parent {
public:
	ChildPtr son;
	~Parent() { cout << "bye parent" << endl; }
};
```
当我们使用时
```
void test() {
	ParentPtr p(new Parent); // p 计数为1
	ChildPtr c(new Child); // c计数为1
	p->son = c; // c计数为2 p为1
	c->father = p; // p计数为2
}
```
引用计数并不为0，这样会发生内存泄漏
这时就要使用weak_ptr来打破循环引用的问题
weak_ptr是shared_ptr的弱引用weak_ptr不会影响引用计数

将某一个类里面的shared_ptr换成weak_ptr即可
```
class Parent;
typedef shared_ptr<Parent> ParentPtr;
typedef weak_ptr<Parent> weakParentPtr;
class Child {
public:
	ParentPtr father;//换成weak解决循环引用问题
	~Child() { cout << "bye child" << endl; }
	void checkRelation();
};
typedef shared_ptr<Child> ChildPtr;
typedef weak_ptr<Child>  weakChildPtr;
class Parent {
public:
	weakChildPtr son;
	~Parent() { cout << "bye parent" << endl; }
	void checkRelation();
};

```
lock()方法将返回一个shared_ptr指针
```
void test2() {
	ObjectPtr obj(new Object(1));//计数为1
	WeakPtr weakObj(obj);//传入一个shared_ptr指针
	//weak_ptr不影响计数此次计数任然为1
	cout << obj.use_count() << endl;
	{
		auto p = weakObj.lock();
		//如果外部没有shared_ptr管理对象返回一个空指针
		//如果有shared_ptr管理返回一个shared_ptr<Object>
		//所以因为auto默认拷贝在这里会拷贝一份p
		if (p) {
			cout << p.unique() << endl;//false
			//p的计数 = 2
		}
		else {
		}
	}
	if (weakObj.expired()) {//判断资源是否过期
	}
	else {
	}
}
```


##this指针问题
如果一个类里面的成员函数需要用到this的shared_ptr指针
如果直接构造一个shared_ptr会在离开作用域时发生析构
我们让这个类派生于`enable_shared_from_this`奇异模板类
在调用函数时传入`shared_from_this`即可
```
void handleChildAndParent(const ParentPtr& p, const ChildPtr& c) {
	auto pc = p->son.lock();
	if (c->father == p && pc == c) {
		cout << "right relation" << endl;
	}
	else {
		cout << "oop \n";
	}
}

class Parent public enable_shared_from_this<Parent> {
public:
	weakChildPtr son;
	~Parent() { cout << "bye parent" << endl; }
	void checkRelation();
};
void Parent::checkRelation() {
	auto ps = son.lock();
	if (ps) {
		//ParentPtr p(this);
		//当出了if作用域时会调用析构函数
		//这样会调用两次析构函数(危险)
		//handleChildAndParent(p, ps);
		handleChildAndParent(shared_from_this(), ps);
		//shared_from_this在内部存在一个weak_ptr重新构造成shared_ptr
		//这样this就不会析构掉避免构造额外的智能指针
	}
}
```

##unique_ptr
release不再管理指针
reset资源释放
```
typedef unique_ptr<Object> UniqueObjectPtr;
```

```
void transfer(UniqueObjectPtr obj) {}
```
unique没有共享模式
拷贝构造和赋值构造不存在
如果要传入transfer函数需要使用move
unique存在移动构造
```
void test3() {
	UniqueObjectPtr obj(new Object(1));
	auto p = obj.get();//返回内部原始指针
	if (p) {
		//...
	}
	//cout << obj->id << endl;
	
	p = obj.release();//不再管理指针
	delete p;
	obj.reset();//吧资源释放
	obj.reset(new Object(2));//传入新指针,释放原有资源再管理新指针
	//unique不能拷贝左值但可以传入右值
	//这时obj不再管理原来的资源
	//这里的obj相当于空指针
	transfer(move(obj));
	//例如cout << obj->id << endl;这种行为，会抛出异常
	
	//将unique转化成shared_ptr
	ObjectPtr sharedObj(move(obj));//将管理权转换成shared_ptr
	//这时就不再管理obj指针了
	assert(obj == nullptr);

	//unique_ptr转移资源需要move
}
```

##总结

1. 不要自己手动管理资源
2. 一个裸指针不要用两个智能指针管理
例如：auto p = new Object(1);
shared_ptr<Object> obj(p);
shared_ptr<Object> obj2(p);
这里是错的
3. 当shared_ptr循环引用时要用weak_ptr打破
4. 当需要用this的智能指针时必须继承enable_shared_from_this<Object>
5. shared_ptr作为函数接口时使用const shared_ptr&形式
6. shared_ptr<Object> = make_shared<Object>(3);和shared_ptr<Object> p(new Object(1));
  使用make_shared比后面的方式在速度上快点空间上也小点
7. enable_shared_from_this 中 shared_from_this()在构造和析构里面不能使用
8. 优先使用unique_ptr其次考虑shared_ptr

