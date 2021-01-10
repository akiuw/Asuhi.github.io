#Linux系统编程
##前言
预计11月15日之前结束

##day01
[**传送门**](day01.md)

* 进程间的同步和互斥
* 进程间通信方式
* `System V`与`POSIX`
* 系统进程通信的分类
* IPC对象的持续性
* 死锁
* 信号量与P,V原语

##day02
[**传送门**](day02.md)

* 消息对列参数查看`msgmax`,`msgmnb`,`msgmni`
* IPC对象的数据结构
* 函数`msgget`和`msgctl`

##day03
[**传送门**](day03.md)

* 发送消息函数`msgsnd`
* 接收消息函数`msgrcv`
* 参数解析函数`getopt`


##day04
[**传送门**](day04.md)

* 共享内存
* `mmap`函数
* `msync`函数
* `system V`共享内存
* 函数`shmget`
* 函数`shmat`
* 函数`shmdt`
* 函数`shmctl`

##day05
[**传送门**](day05.md)

* 使用信号量解决哲学家就餐问题
* 函数`semget`
* 函数`semctl`
* 函数`semop`
* 函数`ftok`

##day06
[**传送门**](day06.md)

* 用信号量解决消费者与生产者的问题
* 使用共享内存实现`shmfifo`
* 关键代码讲解
   * `shmfifo_init`
   * `shmfifo_put`
   * `shmfifo_get`
   * `shmfifo_destroy`

##day07
[**传送门**](day07.md)

* 创建打开POSIX消息队列
* POSIX消息队列相关api函数
   * `mq_close`
   * `mq_unlink`
   * `mq_getattr`和`mq_getattr`
   * `mq_send`

##day08
[**传送门**](day08.md)

* POSIX共享内存相关API函数
   * `shm_open`
   * `ftruncate`
   * `fstat`
   * `shm_unlink`
* `shm_read`程序编写

##day09
[**传送门**](day09.md)

* 程序，进程，线程
* 线程共享进程的数据
* 线程的优缺点
* 线程调度竞争范围
* 三种线程模型

##day10
[**传送门**](day10.md)

* POSIX线程相关API
* 对比进程线程API函数
* 改写回射服务器程序


##day11
[**传送门**](day11.md)

* 线程属性
* 线程特定数据

##day12
[**传送门**](day12.md)

* POSIX信号量
* POSIX互斥锁
* 自旋锁
* 读写锁

##day13
[**传送门**](day13.md)

* 线程池的实现

