#pragma once
#include "head.h"

typedef std::map<std::string, std::list<std::string>> DirListType;

class FileList {
private:
	DIR* dptr;
	struct dirent *ptr;
public:
	DirListType dirlist;
public:
	FileList() :dptr(NULL), ptr(NULL) {}
	void GetDirInfo();
	char GetFileType(struct stat * p);
	~FileList() {}
};


