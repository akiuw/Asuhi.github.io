#Context



##context的使用



```
func main() {
	ctx, cancel := context.WithCancel(context.Background()) //初始化context 返回ctx句柄和cancel函数
    //cancel用来取消所有的ctx
	go watch(ctx, "g1") //创建协程1~3 ctx作为传入参数
	go watch(ctx, "g2")
	go watch(ctx, "g3")
	time.Sleep(10 * time.Second)
	fmt.Println("stop all goroutine")
	cancel() //使所有协程 <-ctx.Done()调用有效
	time.Sleep(3 * time.Second)//等待所有协程打印完毕
}

//协程做了两件事，1 每两秒打印goroutine working... 2 然后监听 <-ctx.Done() 是否返回，Done返回的时机就是main协程调用cancle()的时机
func watch(ctx context.Context, name string) {
	for {
		select {
		case <-ctx.Done():
			fmt.Println(name, "stoped...")
			return
		default:
			fmt.Println(name, "goroutine working...")
			time.Sleep(2 * time.Second)
		}
	}
}
```



###输出：



```
g3 goroutine working...
g1 goroutine working...
g2 goroutine working...
g1 goroutine working...
g3 goroutine working...
g2 goroutine working...
stop all goroutine
g3 stoped...
g1 stoped...
g2 stoped...
```



##context接口

```
type Context interface {
	// 当 context 被取消或者到了 deadline，返回一个被关闭的 channel
	Done() <-chan struct{}
	// 在 channel Done 关闭后，返回 context 取消原因
	Err() error
	// 返回 context 是否会被取消以及自动取消时间（即 deadline）
	Deadline() (deadline time.Time, ok bool)
	// 获取 key 对应的 value
	Value(key interface{}) interface{}
}
```



1. Deadline 返回截止时间，时间到了之后Done() <-chan struct{}不会阻塞，返回ok为true代表时间有效
2. Done 方法返回一个只读的chan，在goroutine中，如果该方法返回的chan可以读取，则意味着parent context已经发起了取消请求，我们通过Done方法收到这个信号后，就应该做清理操作，然后退出goroutine，释放资源
3. Err 表示context为何会被取消
4. Value方法获取该Context上绑定的值，是一个键值对，通过一个Key访问该值，协程安全的





##context初始化衍生



```
func WithCancel(parent Context) (ctx Context, cancel CancelFunc)
func WithDeadline(parent Context, deadline time.Time) (Context, CancelFunc)
func WithTimeout(parent Context, timeout time.Duration) (Context, CancelFunc)
func WithValue(parent Context, key, val interface{}) Context
```



* 第一个只是返回一个句柄和取消函数
* WithDeadline会设置一个时间点到时候会自动取消
* WithTimeout会设置一个时间段从调用开始算起到时候自动取消
* WithValue生成一个绑定了一个键值对数据的Context，这个绑定的数据可以通过Context.Value方法访问到



##context注意事项

1. Context不能放在结构体中，要以参数的方式进行传递

2. 以Context作为参数的方法，应该把Context作为第一个参数

3. 给一个函数方法传递Context的时候不要传递nil，如果不需要传context，就使用context.TODO

4. Context的Value相关方法应该传递必须的数据，不要什么数据都使用这个传递





context源码剖析：https://www.cnblogs.com/qcrao-2018/archive/2019/06/12/11007503.html#%E6%95%B4%E4%BD%93%E6%A6%82%E8%A7%88





