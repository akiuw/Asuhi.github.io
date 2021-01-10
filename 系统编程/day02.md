#Linux网络编程 day02
[**本章源码下载**](code/day02.rar)

##消息队列

* 消息队列提供了一个向另外的进程发送一块数据的方法
* 每个数据块都有类型，接收者可以接收不同类型的数据
* 每个消息大最大长度有上限(MSGMAX)，每个消息队列的总字节数有上限(MSGMNB)系统的总消息队列数量也有上限(MSGMNI)
* 查看方法:
```
		cat /proc/sys/kernel/msgmax
		cat /proc/sys/kernel/msgmnb
		cat /proc/sys/kernel/msgmni
```

##IPC对象的数据结构
```
struct ipc_perm{
	key_t	__key;			/*xxxget(2)创建*/
	uid_t	uid;			/**/
	gid_t	gid;			/**/
	uid_t	cuid;			/**/
	gid_t	cgid;			/**/
	unsigned short mode;	/*访问权限permissions*/
	unsigned short __seq;	/*序号sequence num*/
	
};
```
删除消息队列命令：ipcrm -q 消息的id号(msqid)
查看IPC对象命令	：ipcs

##函数msgget和msgctl

**函数原型:**`int msgctl(int msqid, int cmd, struct msqid_ds *buf);`
**功能：**访问/创建一个消息队列

**参数：**

* key：键值
* msgflg：由九个权限标志构成，与创建文件的mode模式一样

失败返回-1



**函数原型**`int msgctl(int msqid, int cmd, struct msqid_ds *buf);`

**功能：**消息队列控制

**参数：**

* msqid：msgget返回的消息队列的id
* cmd：	采取的动作
	* IPC_STAT	获取消息队列中一些关联的值存放在msqid_ds中
    * IPC_SET		在足够的权限下，可以设置消息队列成msqid_ds给出的数据
    * IPC_RMID	删除消息队列
* buf:消息队列的数据结构
```
struct msqid_ds {
               struct ipc_perm msg_perm;     /* Ownership and permissions */
               time_t          msg_stime;    /* Time of last msgsnd(2) */
               time_t          msg_rtime;    /* Time of last msgrcv(2) */
               time_t          msg_ctime;    /* Time of last change */
               unsigned long   __msg_cbytes; /* Current number of bytes in
                                                queue (nonstandard) */
               msgqnum_t       msg_qnum;     /* Current number of messages
                                                in queue */
               msglen_t        msg_qbytes;   /* Maximum number of bytes
                                                allowed in queue */
               pid_t           msg_lspid;    /* PID of last msgsnd(2) */
               pid_t           msg_lrpid;    /* PID of last msgrcv(2) */
           };
```

