#Linux系统编程 day06
[**本章源码下载**](code/day06.rar)
##用信号量解决消费者与生产者的问题
**假定仓库容量是10**

`sem_full(10)`	初始计数值是10
`sem_empty(0)` 	当前可以消费的产品个数
`sem_mutex`		互斥量


**生产者进程：**
P(sem_full)

如果不满
	P(sem_mutex)
	生产产品
	v(sem_mutex)
	
V(sem_empty)


**消费者进程：**
P(sem_empty)

如果里面有产品可消费
	P(sem_mutex)
	消费产品
	v(sem_mutex)
	
V(sem_full)


*生产者和消费者都可以是多个*
*他们之间是互斥的关系*

##使用共享内存实现shmfifo

假定放入缓冲区是定长的
并且有多个进程可以从缓冲区写入/读取数据

**将这块缓冲区的使用状态存放在缓冲区的头部**

```
struct shmhead{
	unsigned int blksize;//块大小
	unsigned int blocks;//总块数
	unsigned int rd_index;//读索引
	unsigned int wr_index;//写索引
}shmhead_t;

struct shmfifo{
	struct shmhead *p_shm;	//头部指针
	char *p_payload;		//负载首地址指针
	
	int shmid;		//共享内存ID
	int sem_mutex;	//互斥量
	int sem_full;	//控制内存是否满的信号量
	int sem_empty;	//控制内存是否空的信号量
}shmfifo_t;
```

##关键代码讲解

```
shmfifo_t* shmfifo_init(int key, int blksize,int blocks){
    shmfifo_t *fifo = (shmfifo* )malloc(sizeof(shmfifo_t));
    assert(fifo != NULL);
    memset(fifo,0,sizeof(shmfifo_t));
    int shmid;
    int size = sizeof(shmhead_t)+blksize * blocks;//整个共享内存的大小==头部大小+块数*块的大小
    shmid = shmget(key,0,0);
    if(shmid == -1){
        fifo->shmid = shmget(key,size,IPC_CREAT | 0666); // 如果打开失败直接创建共享内存
          if(fifo->shmid == -1){
            ERR_EXIT("shmget");
        }
        fifo->p_shm = (shmhead_t* )shmat(fifo->shmid,NULL,0);
        //初始化p_shm各个参数
        fifo->p_shm->blksize = blksize;
        fifo->p_shm->blocks = blocks;
        fifo->p_shm->rd_index = 0;
        fifo->p_shm->wr_index = 0;
      
        fifo->p_payload = (char* )(fifo->p_shm + 1);
        fifo->sem_mutex = sem_create(key);
        fifo->sem_full = sem_create(key + 1);//使用key+1来区分两个不同的信号量
        fifo->sem_empty = sem_create(key + 2);
        if(fifo->p_shm == (shmhead_t*)-1)
            ERR_EXIT("shmat");
        sem_setval(fifo->sem_mutex,1);
        sem_setval(fifo->sem_full,blocks);
        sem_setval(fifo->sem_empty,0);
    }
    else{//打开成功
        fifo->shmid = shmid;
        fifo->p_shm = (shmhead_t* )shmat(fifo->shmid,NULL,0);
        if(fifo->p_shm == (shmhead_t*)-1)
            ERR_EXIT("shmat");
        fifo->p_payload = (char*)(fifo->p_shm + 1);
        sem_setval(fifo->sem_mutex,1);
        sem_setval(fifo->sem_full,blocks);
        sem_setval(fifo->sem_empty,0);
        fifo->sem_mutex = sem_open(key);
        fifo->sem_full = sem_open(key + 1);//使用key+1来区分两个不同的信号量
        fifo->sem_empty = sem_open(key + 2);
    }
    return fifo;
}                 
```
```
void shmfifo_put(shmfifo_t* fifo,const void* buf){
    P(fifo->sem_full);
    P(fifo->sem_mutex);
    memcpy(fifo->p_payload + fifo->p_shm->blksize * fifo->p_shm->wr_index,buf,fifo->p_shm->blksize);
    fifo->p_shm->wr_index = (fifo->p_shm->wr_index + 1) % fifo->p_shm->blocks;//由于这是一个环形缓冲区所以在到达末尾时下一个缓冲块的位置在头。
    V(fifo->sem_mutex);
    V(fifo->sem_empty);
}  
```
`void shmfifo_get(shmfifo_t* fifo,char* buf)`与上面函数类似

```
void shmfifo_destroy(shmfifo_t* fifo){
    sem_d(fifo->sem_mutex);
    sem_d(fifo->sem_full);
    sem_d(fifo->sem_empty);
    shmdt(fifo->p_shm);
    shmctl(fifo->shmid,IPC_RMID,0);
    free(fifo);
}
```
