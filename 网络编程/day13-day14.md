#Linux网络编程
##day13
[**本章源码下载**](../code/day13.rar)
本章内容较少主要介绍UNIX域的作用，并且初步实现UNIX回射服务/客户程序
####UNIX域协议：
* UNIX域套接字比TCP效率高，相差大约2倍
* UNIX域协议可以在各进程间传递文件描述符
* UNIX域是用路径来描述协议族

```
struct sockaddr_un{
	sa_family_t sun_family;					//AF_UNUX
	char 		sun_path[UNIX_PATH_MAX];	//pathname
};

```
**实现回射服务客户程序**

UNIX域流式套接字connect发现监听队列满时
会返回一个ECONNREFUSED，与TCP不同
如果监听队列满了，会忽略SYN，这样对方会重传SYN

##day14


