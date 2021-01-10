#Linux系统编程  day07
[**本章源码下载**](code/day07.rar)
今天开始学习POSIX IPC
POSIX消息队列是随内核持续的

##创建打开POSIX消息队列

`mqd_t mq_open(const char *name, int oflag);//打开消息队列`
`mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);//创建消息队列`

`mqd_t mqid = mq_open("/abc",O_CREAT | O_RDWR,0666 ,NULL);`
使用该代码创建消息队列之后查看该消息队列

`man 7 mq_overview`命令查看得知：
该文件系统是虚拟的，需要挂载在文件目录下才能查看
`mkdir /dev/mqueue`
`mount -t mqueue none /dev/mqueue`
`ls /dev/mqueue`

##POSIX消息队列相关API
###mq_close

**函数原型：**`int mq_close(mqd_t mqdes);`
关闭消息队列
不会删除该消息队列只是意味着不能通过这个文件描述符mqdes来访问它了而已

###mq_unlink
**函数原型：**`int mq_unlink(const char *name);`
断开消息队列的链接，如果连接数为0时就删除该消息队列

###mq_getattr和mq_getattr
`int mq_getattr(mqd_t mqdes, struct mq_attr *attr);//获取属性`
`int mq_getattr(mqd_t mqdes, struct mq_attr *newattr, struct mq_attr *oldattr);//设置属性`

```
struct mq_attr {
    long mq_flags;       /* Flags: 0 or O_NONBLOCK */
    long mq_maxmsg;      /* Max. # of messages on queue */
    long mq_msgsize;     /* Max. message size (bytes) */
    long mq_curmsgs;     /* # of messages currently in queue */
};
```
###mq_send
**函数原型：**`int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);`

向消息队列发送信息
**参数：**

* mqdes ：mqid
* msg_ptr：消息内容
* msg_len：消息大小
* msg_prio：标志位

