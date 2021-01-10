#pragma once
#include "head.h"
#include "tcpconn.h"


class Command:public TcpBase {
	std::stack<std::string> pathlist;
	std::string path;
	DirListType filelist;
public:
	Command(DirListType fl) :path("/usr/pub/"), filelist(fl){ pathlist.push(path); }
	void waitCommand(std::string command, int sock, int sock2);
	void SendFile(const char* path, int sock);
};