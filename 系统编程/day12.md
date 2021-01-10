#Linux网络编程 day12

##POSIX 信号量相关API
```
sem_open
sem_close
sem_unlink
```
sem_init 初始化一个无名的信号量
`int sem_init(sem_t *sem, int pshared, unsigned int value);`
pshared 为0
	非0 就可以用在不同进程间的多个线程通信，信号量对象必须存放在共享内存空间
sem_destory 销毁一个无名信号量

`sem_wait`函数，这个函数操作是原子的
从信号量的值减去一个1，永远等待该信号量为一个非零值才开始做减法
`sem_post`函数，给信号量加1


##POSIX互斥锁
```
pthread_mutex_init	初始化互斥锁
pthread_mutex_lock	锁定一个互斥锁
pthread_mutex_unlock解锁一个互斥锁
pthread_mutex_destory销毁一个互斥锁
```
这个互斥锁也是无名的

**任务：使用信号量与互斥锁解决消费者与生产者问题**


##自旋锁

自旋锁的效率高于互斥锁
线程在申请自旋锁时不会被挂起，处于忙等待状态

```
pthread_spin_init
pthread_spin_destroy
pthread_spin_lock
pthread_spin_unlock
```

##读写锁

互斥锁是进入临界区在释放之前其他线程不能进入
只要没有线程持有读写锁用于写，其他线程都可以持有读写锁用于读

当没有线程持有某个给定的读写锁用于读或用于写时，才能分配读写锁用于写

读写锁用于读属于共享锁
读写锁用于写属于排他锁

给临界区施加共享锁后还可以加共享锁
施加排他锁或者排他锁后不能再加共享锁和排他锁了

```
pthread_rwlock_init
pthread_rwlock_destroy
int pthread_rwlock_rdlock
int pthread_rwlock_wrlock
int pthread_rwlock_unlock
```


