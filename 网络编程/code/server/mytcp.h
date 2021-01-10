#pragma once
#include "head.h"
#include "TCP_com.h"
#include "FileList.h"
class MyFtp :public TCP_com {

	FileList filelist;
public:
	MyFtp(const char* addrs, short port) :TCP_com(addrs, port) {}

	void SendDirInfo(int conn);
	int do_svr(int conn);

	void SendFile(const char* path, int sock);
	void RecvFile(const char* path, int sock);
};

