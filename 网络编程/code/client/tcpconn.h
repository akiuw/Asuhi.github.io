#pragma once
#include "head.h"

typedef std::map<std::string, std::list<std::string>> DirListType;

class TcpBase {
public:
	DirListType filelist; // map储存文件目录结构
public:
	ssize_t readn(int fd, void* buf, size_t count);
	ssize_t writen(int fd, void* buf, size_t count);
	ssize_t recv_peek(int sockfd, void *buf, size_t len);
	ssize_t readline(int sockfd, void *buf, size_t maxline);

};

class TcpFtp:public TcpBase {
public:
	int sock[2];//指令和文件传输各一个套接字
public:

	bool ConnToSer(std::string addrs, int port);
	void GetDirInfo(int conn);
};

