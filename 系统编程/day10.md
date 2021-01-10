#Linux系统编程 day10
[**本章源码下载**](code/day10.rar)
###主要学习POSIX线程的编程接口

##函数pthread_create

**原型：**`int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);`

**功能：**创建一个线程
成功返回0失败返回错误代码
一般的函数失败之后会设置全局变量`errno`的错误码
但pthread函数不会设置全局变量，而是将错误码返回
`pthread`对每个线程都提供了`errno`变量，每个线程都要自己的`errno`变量

**参数：**

* thread：返回的线程id
* attr：设置线程属性，如果填NULL表示使用默认属性
* start_routine：线程的入口地址，这是一个函数指针，这个函数要接收一个无类型指针，返回一个无类型指针
* arg：传递给上一个函数指针指向函数的参数


**相关函数：**

`int pthread_cancel(pthread_t thread)`
杀死一个线程
`pthread_t pthread_self(void);`
得到当前进程的id

`void pthread_exit(void *retval);`
`int pthread_join(pthread_t thread, void **retval);`
等待thread线程的结束

**参数：**

* thread：线程id
* retval：将退出状态传递到这个参数中
	可以说是return 或者是pthread_exit的信息

`pthread_detach`将线程设置成分离的
这样就会避免僵线程的出现

##线程和进程相似函数对比

函数功能|进程|线程
--|--:|--:|
描述符|`pid_t`|`thread_t`
创建|`fork`|`pthread_create`
等待结束|`waitpid`|`pthread_join`
退出|`exit`|`pthread_exit`
自己结束|在`main`中调用`return`|在线程入口函数中调用`return`
他杀|`kill`|`pthread_cancel`
僵尸进程/线程|父进程调用`wait`等函数才退出|主线程调用`pthread_join` `pthread_detach`


###任务
1. 调用创建线程并退出
2. 用线程的方式改写回射服务器
