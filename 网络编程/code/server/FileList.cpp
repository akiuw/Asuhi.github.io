#include "FileList.h"
#include "TCP_com.h"
void FileList::GetDirInfo() {

	std::vector<std::string> dirvec;
	char filetype;
	struct stat st;
	std::string buf("/usr/pub/");
	dirvec.push_back(buf);
	std::list<std::string> filelist;

	while (dirvec.size() != 0) {
		buf = dirvec.back();
		dirvec.pop_back();

		dptr = opendir(buf.c_str());
		ptr = readdir(dptr);
		while (ptr) {
			std::string d_name(buf+std::string(ptr->d_name));
			lstat(d_name.c_str(), &st);
			if (*ptr->d_name == '.') {
				ptr = readdir(dptr);
				stat(ptr->d_name, &st);
				continue;
			}
			filetype = GetFileType(&st);
			if (filetype == 'd')
				dirvec.push_back(buf + std::string(ptr->d_name) + std::string("/"));

			filelist.push_back(std::string("name:")+std::string(ptr->d_name)+std::string("	type:").append(1,filetype));
			ptr = readdir(dptr);
		}
		dirlist.insert(std::pair<std::string, std::list<std::string>>(buf, filelist));
		filelist.clear();
	}
}

char FileList::GetFileType(struct stat * p) {
	switch (p->st_mode & S_IFMT){
		case S_IFSOCK:return 's'; break;
		case S_IFLNK:return 'l'; break;
		case S_IFBLK:return 'b'; break;
		case S_IFDIR:return 'd'; break;
		default:return '-';
	}
}


