#C++进阶 设计模式day03
##对象性能模式

> 面向对象很好解决了抽象的问题，但必须要付出代价
> 比如说虚函数和继承(C++),带来了大量的内存消耗
> 通常来说，面向对象的成本大都可以忽略不记
> 经典模式:

`Singleton`
`Flyweight`

##单件模式Singleton

> 在软件系统中通常存在一些特殊的类,必须保证他们在系统中只允许存在一个实例，才能保证逻辑的正确性,以及良好的效率
> 绕过构造器保证一个类只有一个实例(只new一次)

###简单来实现Singleton

```
class Singleton {
private:
	Singleton();
	Singleton(const Singleton& other);//简单来实现的话就吧构造函数和拷贝构造函数设置成私有的
public:
	static Singleton* getInstance();//静态函数得到对象
	static Singleton* m_instance;//静态对象的指针
};
Singleton* Singleton::m_instance = nullptr;

Singleton* Singleton::getInstance() {
	//判断是否是空指针
	//是的话创建对象否则直接返回指针
	if (m_instance == nullptr) {
		m_instance = new Singleton();
	}
	return m_instance;
}
```
###上述方式带来的问题：
> 这样实现是非线程安全的
> 意味着在多线程的情况下有可能这个对象被创建多个

###我们用锁的方式实现：
> 加锁但成本太高
> 假如线程A得到了锁
> 那么其他的线程必须等待锁的释放
> 当指针不是空的时候其他线程要进行读操作这样就没必要加锁
> 这样浪费了资源

####**在非高并发的情况下可以用加锁的方式实现**

```
Singleton* Singleton::getInstance() {
	Lock lock;//加锁
	//判断是否是空指针
	//是的话创建对象否则直接返回指针
	if (m_instance == nullptr) {
		m_instance = new Singleton();
	}
	return m_instance;
}
```
###双检查锁：锁前锁后都检查
> 线检查指针是否为空,如果是空的话就加锁

```
Singleton* Singleton::getInstance() {
	if (m_instance == nullptr) {
		Lock lock;
		if (m_instance == nullptr) {
			//这个if必须判断
			//如果不判断：A，B两个线程都进入到锁这里
			//只是互相等待而已，结果是都会创建对象
			m_instance = new Singleton();
		}
	}
	return m_instance;
}
```
####**注意：**
* 以上会出现内存读写的reorder的不安全，导致双检查锁的失效


###reorder：
> 代码有一串指令序列通常这个序列会按照我们想象的方式来执行
> 正常地来说语句`m_instance = new Singleton();`执行的顺序是
> 1. 线开辟一段内存空间
> 2. 调用构造函数
> 3. 将该内存的地址赋值给m_instance
> 
> 但实际上，到cpu和汇编的层面发生reorder后会不一样（只是有可能会发生）具体情况是
> 以上2，3的顺序会发生变化，这样就会导致多线程的情况下有的线程得不到一段有效的内存地址
> 就去进行读操作
> reorder发生的原因是处于不同cpu之间的优化方式不一样

###正确的双检查锁实现方式

####**必须要支持c++11版本**
```
//c++11版本之后的跨平台实现
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_muxtex;
Singleton* Singleton::getInstance() {
	Singleton* tmp = m_instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);//得到内存fence
	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(mutex);
		tmp = m_instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new Singleton();
			std::atomic_thread_fence(std::memory_order_release);//释放内存fence
			m_instance.store(tmp, std::memory_order_relaxed);
		}
	}
	return tmp;
}
```

###总结

1. Singleton模式中允许构造器protected允许子类派生
2. Singleton模式一般不要支持拷贝构造和Clone的接口
3. 在多线程的环境下实现安全的Singleton,特别注意双检查锁的实现

