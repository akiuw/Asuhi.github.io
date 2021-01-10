#Linux系统编程 day05
[**本章源码下载**](code/day05.md)
本章主要学习`system v`信号量

```
struct semid_ds {
   struct ipc_perm sem_perm;  /* Ownership and permissions */
   time_t          sem_otime; /* 最后一次执行pv操作的实际*/
   time_t          sem_ctime; /* Last change time */
   unsigned long   sem_nsems; /* 信号量集中的信号量个数 */
};
```
##函数semget

**函数原型：**`int semget(key_t key, int nsems, int semflg);`
**功能：**创建/链接信号量
**参数：**

* key：键值
* nsems：信号量的个数
* semflg：创建选项

##函数semctl

**函数原型：**`int semctl(int semid, int semnum, int cmd, ...);`
**功能：**控制信号量集
**参数：**

* semid：semget返回的标识码
* semnum：信号量集中的信号量的哪一个信号量被操作
* cmd：
	* SETVAL	设置信号量的计数值	
	* GETVAL	获取信号量的计数值
	* IPC_STAT	得到信号集的信息存放在`semid_ds`中
	* IPC_SET		吧信号量集设置成`semid_ds`指定的信息
	* IPC_RMID	删除信号集
* 最后一个参数根据cmd的不同而不同,在下面共用体中选择一个类型输入即可
```
/*该共用体需要自己定义*/
union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};
```

##函数semop

**函数原型：**`int semop(int semid, struct sembuf *sops, unsigned )`
**功能：**用来进行pv操作的函数
**参数：**

* sops：结构体指针
```
struct sembuf{
	unsigned short sem_num;  /* 信号量编号第一个序号为0 */
    short          sem_op;   /* -1表示p操作+1表示v操作*/
    short          sem_flg;  /* 操作选项不关心填0即可 如果填写IPC_NOWAIT 当资源不够时不阻塞，操作会返回-1，errno == EAGAIN
	SEM_UNDO 撤销，当一个进程终止时会对当前的pv操作撤销*/
};
```
* nsops：信号量个数

##函数ftok
**函数原型：**`key_t ftok(const char *pathname, int proj_id);`
**功能：**生成一个键值，根据下面两个参数会结合产生一个唯一的key值
**参数：**

* pathname：存在的路径
* proj_id：该参数的低八位必须是非0的

##任务
**使用信号集解决则学家就餐问题**
