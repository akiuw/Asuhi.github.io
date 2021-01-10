#Linux系统编程 day03
[**本章源代码下载**](code/day03.rar)
##发送消息

###函数介绍：msgsnd

**函数原型：**`int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);`

**功能：** 往消息队列发送消息,将一条消息加到消息队列中。成功返回0失败返回-1

**参数介绍：**
* msqid：msgget返回的消息队列的id
* msgp：指向要发送的消息参考,形式:
`struct msgbuf{
	long mtype;
	char mtext[1];
};`
* msgsz：消息长度
* msgflg：发送选项
		IPC_NOWAIT，队列满的时候不等待，返回EAGAIN错误
		不关心的时候填0表示以阻塞的方式发送
	
参考使用代码：
```
int main(int argc,char *argv[]){
	if(argc != 3){
		fprintf(stderr,"error: use %s <bytes> <msgid>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	int msgid;
	int len = atoi(argv[1]);
	int type = atoi(argv[2]);
	msgid = msgget(1234, 0);
	if (msgid < 0)
		ERR_EXIT("msgget");
	struct msgbuf *buf;
	buf = (struct msgbuf* )malloc(sizeof(long) + len);
	buf->mtype = type;
	msgsnd(msgid, buf, len, IPC_NOWAIT);//0表示以阻塞的方式发送
	return 0;
}
```

##接收消息
###函数介绍：msgrcv

**函数原型：**`ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);`

**功能:  **按照参数指定的要求从消息队列中接收一条消息。成功返回接收到的字符个数，失败返回-1

**参数：**

*  msqid：msgget返回的消息队列的id
*  msgp：接收的缓冲区
*  msgsz:接收的大小
*  msgtyp：消息的类型
   * msgtype=0 返回队列中的第一条消息
   * msgtype<0 返回队列中的一条类型等于msgtyp的消息
   * msgtype>0 返回队列第一条类型小于等于msgtyp绝对值的消息
   * msgflg=IPC_NOWAIT	没有消息不等待返回EAGAIN 
   * msgflg=MSG_NOERROR	消息大小操作   msgsz是被截断
   * msgflg=MSC_EXCEPT且msgtyp>0	接收不等于msgtype的第一条消息

###函数介绍：getopt

**函数原型：**`int getopt(int argc, char * const argv[], const char *optstring);`

**功能：**参数解析，每次返回会得到一个参数选项，如果是非法选项就会返回一个'?'

**参数：**

* argc和argv ：argc、argv是调用main函数时传入的参数
* optstring：指定合法的选项。该字符串里面一个字母代表一个选项如果字符后面跟有‘:’，就代表这个选项有参数可选。并且这个参数会保存在optarg变量中
```
while(true){
		opt = getopt(argc,argv,"nt:");
		//如果opt接收到问号，表示有非法选项
		if(opt == '?'){
			exit(EXIT_FAILURE);
		}
		if(opt == -1)break;//所有的参数解析完成
		switch(opt){
			case 'n':flag |= IPC_NOWAIT;break;
			case 't':type = atoi(optarg);break;
		}
	}
```

接收消息关键代码：
```
int main(int argc,char *argv[]){
	int flag = 0;
	int type = 0;
	int opt;
	//解析参数
	while(true){
		opt = getopt(argc,argv,"nt:");
		//如果opt接收到问号
		if(opt == '?'){
			exit(EXIT_FAILURE);
		}
		if(opt == -1)break;//所有的参数解析完成
		switch(opt){
			case 'n':flag |= IPC_NOWAIT;break;
			case 't':type = atoi(optarg);break;
		}
	}
	int msgid;
	msgid = msgget(1234, 0);
	if (msgid < 0)
		ERR_EXIT("msgget");
	struct msgbuf* buf;
	buf = (struct msgbuf*)malloc(sizeof(struct msgbuf*) + MSGMAX);
	buf->mtype = type;
	int n = 0;	
	if((n = msgrcv(msgid, buf, MSGMAX, type, flag)) < 0)
		ERR_EXIT("MSGRECV");
	printf("read %d bytes type = %ld \n", n, buf->mtype);
	return 0;
}
```

