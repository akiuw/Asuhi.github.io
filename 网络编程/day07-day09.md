#Linux网络编程
##day07
* [本章源代码下载](../code/day07.rar)


今天进一步掌握学习`select`函数的用法以及理解`select`的不足之处


**`select`发生条件**

**对于可读的套接口**

1.	套接口缓冲区有数据可读，对等方发送数据过来填充了缓冲区
2.	链接的读一半关闭，TCP协议栈FIN端，读操作返回0，也能够通知select
3.	如果是一个监听套接口，对等方进行链接操作，此时已连接队列不为空
4.	套接口发生了一个错误待处理，对于这个错误我们可以用getsockopt指定SO_ERROR选项来获取

**对于可写的套接口**

1.	发送缓冲区中有空间//一般情况下不会被占满
2. 链接方的写一半关闭，在TCP协议栈收到RST段之后，并且再次进行write操作得到SIGPIPE信号
3.	套接口上面发生一个待处理错误，对于这个错误我们可以用getsockopt指定SO_ERROR选项来获取

**对于异常事件**

* 套接口上面存在带外数据

**不足之处**

* 对于单核的CPU select无法真正地并行
* 对于多核的CPU select无法充分地利用


**任务：**
使用`select`改进服务器程序

关键部分代码：

```
#include <sys/select.h>
int nready;
	int maxi = 0;//遍历时如果每次都遍历到FD_SETSIZE很浪费效率 
				 //maxi储存有套接字的client的最大数量，这样就只要遍历有套接字的部分了

	fd_set rset;
	fd_set allset;
	FD_ZERO(&rset);
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	int maxfd = listenfd; // 此时监听套接口的值等于3在服务刚开始时是最大的
	while (true) {
		rset = allset; //由于allset 再次监听时会产生事件 1监听套接口产生的事件 2已连接套接口产生的事件
						//由于rset里面的内容会改变如果没有保存在allset中rset里面是已经改变过的集合
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (nready == -1) {
			if (errno == EINTR) //被信号中断
				continue;
			perror("select");
			exit(EXIT_FAILURE);
		}
		if (nready == 0)
			continue; //没有发生事件
		if (FD_ISSET(listenfd, &rset)) {
			socklen_t peerlen = sizeof(peeraddr); //必须要有初始值
			if ((conn = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0) {//此时肯定不阻塞
				perror("accept()");
				exit(EXIT_FAILURE);
			}
			for (int i = 0; i < FD_SETSIZE; i++) {
				if (client[i] < 0) {
					client[i] = conn;
					if (i > maxi)
						maxi = i;
					break;
				}
				if (i == FD_SETSIZE) {
					fprintf(stderr, "to many clients\n");
					exit(EXIT_FAILURE);
				}
			}
			printf("ip = %s,port = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
			FD_SET(conn, &allset);//将conn添加到集合中
			if (conn > maxfd)
				maxfd = conn;
			if (--nready <= 0) //事件已经处理完了应当继续监听
				continue;
		}
		//遍历处理事件
		for (int i = 0; i <= maxi; i++) {//这里maxi是最大套接字的位置要将其包含所以要是闭区间[0,maxi] 使用<=符号
			conn = client[i];
			if (conn == -1) {
				continue;
			}
			if (FD_ISSET(conn, &rset)) {
				char recvbuf[1024];
				memset(recvbuf, 0, sizeof(recvbuf));
				ssize_t ret = readline(conn, recvbuf, 1024);
				if (ret == -1) {
					perror("readline()");
					break;
				}
				if (ret == 0) {
					printf("pid = %d 关闭了 0 \n", getpid());
					FD_CLR(conn, &allset);//如果已经关闭就将它从集合中清楚
					client[i] = -1;//置为空闲状态
				}

				fputs(recvbuf, stdout);
				writen(conn, recvbuf, strlen(recvbuf));
				if (--nready <= 0)
					break;
			}
		}
	}
```

##day08
###`close`与`shutdown`的区别

1. close终止了数据传送的两个方向
假设A与B通信
A端调用`close` 这就意味着A不能从sockA向B接收发送数据或从sockA读取数据B发送的数据
但对于B来说任然可以向A发送数据
TCP是可以看做一个全双工的管道
这就意味着 A可能接收不到发送FIN前B发给A的最后一波数据
如果A只想关闭发送的那一个方向，而不关闭读取的那一个方向
对于close来说就无法实现了
2. `shutdown`可以选择终止某个方向的数据传输
只需要终止A的发送方向`how = SHUT_WR`意味着A向B发送了FIN段
3. 并且close不能保证任何时刻都能向对方发送FIN段只有所有进程都退出后才能发送
例如之前使用fork方法的并发服务器父进程关闭conn时只是将套接字的引用计数减1
只有套接字引用计数为0时才会真正发送FIN段
而`shutdown`不会理会引用计数直接发送FIN段

引用计数：调用`fork`函数之后子进程会拷贝父进程所有的内存此时conn的引用计数会加一

**函数功能与参数**

* 函数原型`int shutdown(int sockfd, int how);`
* 
**功能:**

* 关闭套接字发送接收数据的某一方向


**参数**

* how: 
	* `SHUT_WR`关闭发送
	* `SHUT_RD`关闭接收
	* `SHUT_RDWR`关闭两个方向


**假如服务器繁忙大概4s会来回射一次数据**

*当我们的客户端在4s内发送数据为AAAA BBBB 在按下CTRL+D*

对应的服务器会依次
1. 接收AAAA 打印AAAA
2. 4s钟后回射AAAA
		**此时服务器缓冲区有:**BBBB RST段
3. 接下来服务器会接收并打印BBBB
**此时服务器缓冲区有:** RST段
4. 4s钟后回射BBBB
5. 接收RST段
6. 4s后调用`write`函数 
此时的情况是收到RST段后调用`write`函数得到SIGPIPE信号
导致服务器崩掉

使用shutdown优化回射客户端程序
```
if (FD_ISSET(fd_stdin, &rset)) {//检测到stdin可读

			if (fgets(sendbuf, sizeof(sendbuf), stdin) == NULL)
			{
				isend = true;
				shutdown(sock, SHUT_WR);//将写方向关闭任然可以从套接口读取数据
			}
			else
			{
				isend = false;
				writen(sock, sendbuf, strlen(sendbuf));
			}
		}
```
此时客户端会等待服务发送完毕后关闭 (也就是服务器收到RST段)

##day09

####套接口IO超时设置的方法
**方法一：alarm信号**

具体方法

```
void handler(int sig){return;}//只要返回即可不需要任何操作
signal(SIGALRM,handler);
alarm(5);//5秒钟闹钟
int ret = read(fd,buf,sizeof(buf));
if(ret = -1 && errno == EINTR){
	errno = ETIMEDOUT;
}//如果ret = -1 这就证明了5s种过后超时 将错误信息记录为ETIMEDOUT
else if(ret > 0){ //如果read收到了数据，就把闹钟取消掉
	alarm(0);
}
```

**这个方法产生的问题：**

* `alarm`信号可能有其他问题，这可能会引起很多冲突，通常处理起来很麻烦我们基本上不会使用这种方案

**方法二：套接字选项**

* `SO_SNDTIMEO`
* `SO_RCVTIMEO`

```
sctsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,5);//5s超时如果超时时间5s到了read的返回值会是-1 并且 errno的值为EWOULDBLOCK
```

* 但是有些TCP/IP的实现不支持这两个参数选项

**方法三：`select`函数实现**

封装函数：

* `read_timeout`
* `write_timeout`
* `accept_timeout`
* `connect_timeout`

具体方法如下：

这里只演示connect_timeout函数,这个函数难度相对较高
其他的可以自己尝试去实现
```
void activate_nonblock(int fd){//将IO设置成非阻塞
	int ret;
	int flags = fcntl(fd,F_GETFL);//得到标识符
	if(flags == -1){
		perror("fcntl");
	}
	flags |= O_NOMBLOCK; //将flags与O_NOMBLOCK相或如果要设置成阻塞模式只需要将flags &= ~O_NOMBLOCK 即可函数名为deactivate_nonblock(int fd)
	ret = fcntl(fd,F_SETF, flags);
	if(ret == -1){
		perror("fcntl");
	}
}
```

```
int connect_timeout(int fd,struct sockaddr_in *addr,unsigned int wait_seconds)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	if(wait_seconds > 0)
		activate_nonblock(fd);//如果等待时间大于零我们就要将套接字设置成非阻塞模式
	ret = connect(fd,(struct sockaddr*)addr,addrlen);//直接调用connect函数如果网络状况良好就可能直接返回成功
	if(ret < 0 && errno == EINPRDGRESS){//EINPRDGRESS代表连接正在处理
		fd_set connect_fdset;//定义一个连接的集合
		struct tineval timeout;
		FD_ZERO(&connect_fdset);
		FD_SET(fd,&connect_fdset);
		timeout.tv_sec = wait_seconds;//设置select时间等待为wait_seconds
		timeout.tv_usec = 0;
		do{
			ret = select(fd+1,NULL,&connect_fdset,NULL,&timeout);//一旦连接套接字就可写了
		}while(ret < 0 && errno == EINTR);//反复调用select直到连接成功或超时
		if(ret == 0){//如果返回值为0
			ret = -1;
			errno = ETIMEDOUT;//errno 设置为ETIMEDOUT
		}
		else if(ret < 0)
			return 0;
		else if(ret == 1){
			/*ret 返回为1时，有两种情况，连接建立成功产生可写事件,或者是套接字本身处于一种出错的状态
			此时错误信息不会保存在errno中，因此需要调用getsockopt来获取套接字错误
			*/
			int err;
			socklen_t socklen = sizeof(err);
			int sockoptret = getsockopt(fd,SOL_SOCKET,SO_ERROR,&err,&socklen);//通过SO_ERROR选项来获取将错误放在err中
			if(err == 0)
				return 0;//没有错误连接成功
			else{
				errno = err;//产生错误将err变量赋值给errno
				ret = -1;
			}
		}
	}
	if(wait_seconds > 0){
		deactivate_nonblock(fd);//如果直接连接成功，重新将套接字设置成阻塞模式
	}
	return ret;
}
```
