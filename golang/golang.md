#map slice string interface{} 协程安全探讨

##map

(1) 使用锁+普通map
		
```
type SafeMap struct {
	Data map[string]interface{}
	Lock sync.RWMutex
}
```

(2) 使用sync.map
(3) 使用chan+普通map
	

##slice

##go语言sync各个类的使用

###sync.Mutex 互斥锁

```
	mutex := &sync.Mutex{}
	mutex.Lock()
	// Update共享变量 (比如切片，结构体指针等)
	mutex.Unlock()
```

###sync.RWMutex 读写锁

```
	mutex := &sync.RWMutex{}
	mutex.Lock()
	// Update 共享变量
	mutex.Unlock()
	mutex.RLock()
	// Read 共享变量
	mutex.RUnlock()
```
**在频繁读取的使用使用RWMutex效率比Mutex高**

###sync.WaitGroup

一般使用时是一个goroutine等待一组goroutine执行完成

使用`sync.WaitGroup()`初始化后 调用`Add(int)方法将计数器值增加` 当 `Wait()`时如果计数器不为0会阻塞
只有当其他协程调用`Done() 计数器减1`或者`Add(-n) 计数器减n` 当计数器的值为0时才能继续执行协程。

```

wg := &sync.WaitGroup{}
 
for i := 0; i < 8; i++ {
  wg.Add(1)
  go func() {
    // Do something
    wg.Done()
  }()
}
 
wg.Wait()
// 继续往下执行...
```

###sync.Map

sync.Map是go语言中map的并发版本。
`Store(interface {}，interface {})`增加元素
`Load func(key interface{}) (value interface{}, ok bool)` 检索元素，输入key返回value和一个bool代表是否存在该值
`Delete(interface {})`删除元素
`Range(f func(key, value interface{}) bool)`遍历元素传入一个函数`func(key, value interface{}) bool`对象如果这个函数返回false，则停止遍历
`LoadOrStore(interface {}，interface {}) (interface {}，bool)` 如果存在元素则返回value和true 如果不存在则返回value和false

对比`map+sync.Mutex`方式
使用场景：
1. 对map频繁读取而很少写入时
2. 当多个协程分片写入时，比如有4个协程分别负责一部分key值而这些key值不会相交。

###sync.Pool

Pool 是可伸缩、并发安全的临时对象池，用来存放已经分配但暂时不用的临时对象，通过对象重用机制，缓解 GC 压力，提高程序性能。
我们在要使用对象时可以先去Pool里面取，如果取不到在进行分配内存。效率比直接分配并且等待GC回收要更加高效。
sync.Pool 是一个临时的对象池，适用于储存一些会在 goroutine 间共享的临时对象，其中保存的任何项都可能随时不做通知地释放掉，所以不适合用于存放诸如 socket 长连接或数据库连接的对象。

示例：
```
//初始化pool时可以指定对象创建的new函数
	pool := &sync.Pool{
		New: func() interface{} { //New函数
			return new(int)
		},
	}
	
	a := 0
	b := 1
	c := 2

//添加3个元素
	pool.Put(&a)
	pool.Put(&b)
	pool.Put(&c)

	p := pool.Get().(*int)
	p2 := pool.Get().(*int)
	p3 := pool.Get().(*int)
	p4 := pool.Get().(*int)//第4次由于pool里面已经没有对象了，就会调用New函数生成一个新的，如果没有指定New函数那么这里就会返回nil
	
	fmt.Println(*p, *p2, *p3, *p4) //0 2 1 0 对象Get的顺序与存入顺序无关
```

**Put时**
1. 检查当前goroutine的是否设置对象池私有值，如果没有则将x赋值给其私有成员，并将x设置为nil。
2. 如果当前goroutine私有值已经被设置，那么将该值追加到共享列表。

**Get时**
1. 尝试从本地P对应的那个本地池中获取一个对象值, 并从本地池冲删除该值。
2. 如果获取失败，那么从共享池中获取, 并从共享队列中删除该值。
3. 如果获取失败，那么从其他P的共享池中偷一个过来，并删除共享池中的该值(p.getSlow())。
4. 如果仍然失败，那么直接通过New()分配一个返回值，注意这个分配的值不会被放入池中。New()返回用户注册的New函数的值，如果用户未注册New，那么返回nil。


###sync.Once

确保一个函数仅执行一次
```

once := &sync.Once{}
for i := 0; i < 4; i++ {
    i := i
    go func() {
        once.Do(func() {   // 使用Do(func())方法
            fmt.Printf("first %d\n", i)
        })
    }()
}
```

###sync.Cond

`sync.Cond` 发出信号（一对一）或广播信号（一对多）到goroutine
他需要使用一个Mutex或者RWMutex初始化

```
	cond := sync.NewCond(&sync.Mutex{}) //创建条件变量

	for i := 0; i < 3; i++ {
		go func(i int) {
			cond.L.Lock() //尝试上锁
			cond.Wait() //等待条件，这里调用Wait时内部回进行解锁，当条件来到时回重新上锁
			fmt.Println("ok", i)
			cond.L.Unlock()
		}(i)
	}

	time.Sleep(1 * time.Second)
	cond.Broadcast() //唤醒所以因为cond变量Wait的协程
	time.Sleep(1 * time.Second)
```


##tag标签的特性


###关于tag

> tag相当于该字段的一个属性标签, 在Go语言中, 一些包通过tag来做相应的判断


```
type Student struct {
    Name string `ak:"av" bk:"bv" ck:"cv"`
}
```

###自定义标签使用

使用reflect包我们可以查看结构体自动的标签名和标签值

```
const tagName = "validate"  //自定义标签名

	type User struct {
		ID    int    `validate:"-"`
		Name  string `validate:"presence,min=2,max=32"`
		Email string `validate:"email,required"`
	}
	user := User{
		ID:    1,
		Name:  "John Doe",
		Email: "john@example",
	}

	t := reflect.TypeOf(user) //得到类型t

	fmt.Println(t.Name())
	fmt.Println(t.Kind()) //打印类型名和类型

	for i := 0; i < t.NumField(); i++ { //NumField可以得到类型里面有多少个字段
		field := t.Field(i)  //通过索引得到每一个字段

		tag := field.Tag.Get(tagName) //通过标签名，得到字段的标签值

		fmt.Printf("%d. %v(%v), tag:'%v'\n", i+1, field.Name, field.Type.Name(), tag)
	}
```


###常用tag标签


1. json 标签，使用Marshal与Unmarshal进行序列化，不加tag默认使用变量名，使用`"-"`表示转换时不处理该字段,使用`omitempty`忽略0值
2. db标签，使用sqlx 示例`db.Select(&users, "select * from public.user where id>$1", 900)`  其中`users=make([]User, 0)` 可以将select的结果全部写入users切片中
3. form标签，gin中使用，` if err := c.ShouldBind(&form); ` `form`是已经打上`form标签`的结构体的对象





