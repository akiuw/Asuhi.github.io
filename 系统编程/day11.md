#Linux系统编程 day11
[**本章源码下载**](code/day11.rar)

##线程属性

初始化和销毁线程属性
`int pthread_attr_init(pthread_attr_t *attr);`
`int pthread_attr_destroy(pthread_attr_t *attr);`


线程分离属性
`int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);`
`int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);`
* detachstate:有两个参数可选
	* PTHREAD_CREATE_DETACHED
	* PTHREAD_CREATE_JOINABLE


设置线程栈大小
`int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);`
`int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);`
stacksize为0的话采用系统默认大小一般这个参数为0


栈溢出保护区大小
`int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);`
`int pthread_attr_getguardsize(pthread_attr_t *attr, size_t *guardsize);`
默认是4k

线程竞争范围
`int pthread_attr_setscope(pthread_attr_t *attr, int scope);`
`int pthread_attr_getscope(pthread_attr_t *attr, int *scope);`
`PTHREAD_SCOPE_SYSTEM`	系统范围
`PTHREAD_SCOPE_PROCESS`	进程范围

调度策略
`int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);`
`int pthread_attr_getschedpolicy(pthread_attr_t *attr, int *policy);`
```
SCHED_FIFO	先进先出
SCHED_RR	强占
SCHED_OTHER	other
```

是否继承调度策略
`int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);`
`int pthread_attr_getinheritsched(pthread_attr_t *attr, int *inheritsched);`

PTHREAD_INHERIT_SCHED	继承调用者线程的策略属性
PTHREAD_EXPLICIT_SCHED	不继承需要自己设置

设置获取调度参数
`int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);`
`int pthread_attr_getschedparam(pthread_attr_t *attr, struct sched_param *param);`

关注调度优先级


并发级别：只限于N:M调度模型
`int pthread_setconcurrency(int new_level);`
`int pthread_getconcurrency(void);`
给内核一个提示
给定级别数量的核心线程来映射用户线程是高效的
假如有3个用户线程那么给定3个核心线程来映射是高效的
即便设定了该值，内核也不一定安装这个值来设定
0表示内核自己选择方式来映射


##线程特定数据

我们通常使用全局变量来实现多个线程共享数据

![](image/线程特点数据.png)

每个线程具有自己的特点数据一共128项
每个数据都有一共key和value
每个线程的key都是一样的但是这个key指向的数据是特有的
如果一个线程对该数据改变并不会影响到其他线程

`int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));`
创建一个key这个key被所有线程共享
`int pthread_key_delete(pthread_key_t key);`
删除key

`void *pthread_getspecific(pthread_key_t key);`
根据key值得到他所指向的数据
`int pthread_setspecific(pthread_key_t key, const void *value);`
吧value指向的数据设置进key里面

`int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));`
`pthread_once_t once_control = PTHREAD_ONCE_INIT;`
init_routine指向的函数即使被多个线程调用但也只执行一次

