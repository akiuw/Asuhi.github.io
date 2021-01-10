#Linux 网络编程
##day01    
* [**本章源码下载**](../code/day01.rar)

今天主要学习socket编程流程以及TCP链接客户端/服务器，简单的程序编写
**函数`socket`功能以及参数介绍**

函数原型：`int socket(int domain,int type,int protocol)`
功能：

* 创建一个套接字用于通讯
成功返回非负

参数：

   1. domain: 指协议族(protocol family)
   2. type 指socket类型
		流式套接字SOCK_STREAM
		数据报套接字SOCK_DGRAM
		原始套接字SOCK_RAW
   3. protocol:协议类型

常见用法:

```
lisenfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)
lisenfd = socket(PF_INET,SOCK_STREAM,0)
//这两个都代表TCP
```
**函数`bind`功能以及参数介绍**

函数原型：`int bind(int sockfd,const struct sockaddr* addr,socklent_t addrlen)`

功能：

* 绑定一个本地地址到套接字
成功返回0 否则返回-1

参数：

1. sockfd:socket()函数的返回值
2. addr:绑定的地址(将IPV4的地址结构强制转化成通用地址结构)
3. addrlen:地址长度

关于`struct sockaddr_in servaddr`结构体填充

`struct sockaddr_in servaddr`结构体填充:
```
memset(&servaddr,0,sizeof(servaddr));
servaddr.sin_family = AF_INET; //IPV4
servaddr.sin_port = static_cast<in_port_t>(htons(5000));//端口号使用htons()函数转化成网络字节序
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY表示本机的任意地址
```

**注意:**

* port要调用htons()将整型变量从主机字节顺序转变成网络字节顺序
	s_addr调用htonl()将一个32位数从主机字节顺序转换成网络字节顺序
	如果调用错误port将被赋值为0

**用法举例:**

`bind(listenfd, reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr))`

函数`listen`功能及参数介绍
函数原型：`int listen(int sockfd,int backlog)`

功能：

* 将套接字用于监听进入的链接，一旦调用这个函数后套接字就变成了`被动套接字`

主动套接字是用来发起链接的
被动套接字是用来接收链接的

一般来说在调用`socket`和`bind`函数之后再调用`listen`函数，在`accept`之前调用

* 对于给定的监听套结构内核要维护两个队列

   1. 服务器正在等待完成相应TCP三次握手的队列(握手过程)
   2. 已经完成握手（链接）的队列
两个队列之和不超过backlog(并发链接数)
成功返回0 否则返回-1

**参数:**

* sockfd:socket()函数的返回值

* backlog:内核规定套接字排队的最大链接数

**函数`accept`功能及参数介绍**

函数原型:`int accept(int sockfd,struct sockaddr* addr,socklen_t *addrlen)`

功能:

* 已经完成链接队列里面返回第一个链接，如果这个队列为空，则阻塞

**参数:**

* sockfd: socket()函数的返回值
* addr: 输出型参数 链接的地址进行
* addrlen: addr的长度

常见用法:

```
struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr); //必须要有初始值
	int conn = -1;  //已连接套接字(这是一个主动套接字)
	if ((conn = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0) {
		perror("accept()\n");
		return 0;
	}
```

##day02
* [**本章源码下载**](../code/day02.rar)

对于day01的例程只能实现服务器与客户端的1对1的通信
因为在服务器端`accept`之后是进入一个死循环来处理和发送信息
解决方法:在`accept`成功之后创建一个子进程和客户端进行通信
在父进程中继续等待链接
**注意:**

* 在父进程中要关闭链接套接字
* 在子进程中要关闭监听套接字

####点对点模式聊天
**功能:**

* 单一的点对点进行通信
服务器开启，客户端与之链接后可以相互发送字符串
在一方关闭后另一方也会随之关闭

**实现方法:**

* 使用多进程的方式:
服务器程序中链接成功后子进程进行输入发送，父进程将读到的数据输出
当父进程死亡时要通知子进程结束(使用信号的方式)
进程关闭之前要将通信用的套接字关闭
客户端程序中链接后子进程将读到的数据输出，父进程将输入的数据发送出去

####关于粘包问题
**原因:**

TCP是一种数据流,在两个数据包之间没有特定的分割,所以就会存在粘包问题
**解决方法:**

1. 定长包
封装函数接收确定字节的大小(双方都确定一个确定的大小
编写`readn()`和`writen()`两个函数实现
2. 自己定义协议(运用层)   
   * 区分消息与消息之间的边界
```
struct packet{
	int len;		//buf的大小
	char buf[1024];	//真实的数据
};
```
   * 在接收数据时先接收len得到数据包的大小
   * 再按照指定字长读取buf
   * 发送时先计算数据包buf的大小n
   * 将n转化成网络字节序后和数据打包成packet发送出去



##day03
* [**本章源码下载**](../code/day03.rar)

本章主要内容是编写读取一行的方法`readline`
先介绍一个函数
```ssize_t recv(int sockfd, void *buf, size_t len, int flags)```
与read函数功能一样都能从套接口缓冲区中获取数据到buf
**不同点是:**

   * recv函数只能用于套接口io不能用于文件io以及其他的io
   * recv多了一个flags选项：
     MSG_OOB：通过紧急指针发送数据
     MSG_PEEK：接收缓冲区数据之后并不将缓冲区清楚

**任务：**

* 继续优化回射服务器
	编写函数`readline()`
* 函数内容以及注解:
```
ssize_t readline(int sockfd, void *buf, size_t maxline) {
	ssize_t ret;
	int nread;
	char *bufp = (char*)buf;
	int nleft = maxline;
	while (true) {
		ret = recv_peek(sockfd, bufp, nleft);//使用recv_peek窥视
		if (ret < 0)
			return ret;
		else if (ret == 0) {
			return ret;
		}
		nread = ret;
		for (int i = 0; i < nread; i++) {
			if (bufp[i] == '\n') {		//如果在最大一行的范围内读到了'\n'就读取一个i+1字节定长包
				ret = readn(sockfd, bufp, i + 1);
				if (ret != i + 1)
					exit(EXIT_FAILURE);
				return ret;
			}
		}
		if (nread > nleft) {
			perror("error");
			exit(EXIT_FAILURE);
		}
		//如果走到这一步说明数据大于最大行数，那么读取一个最大定长包后，继续去窥视直到读到'\n'
		nleft -= nread;
		ret = readn(sockfd, bufp, nread);
		if (ret != nread)
			exit(EXIT_FAILURE);
		bufp += nread;
	}
	return -1;
}
```
**介绍3个有关地址的函数**

```
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
//调用getsockname能知道自己的地址和端口号

int  getpeername(int  sockfd,  struct sockaddr *addr, socklen_t *addrlen)
//得到对方的地址

int gethostname(char *name, size_t len)
//函数将获得本机地址保存到name
```

