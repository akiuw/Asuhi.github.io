#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <sys/stat.h>

#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <list>


//command define

#define GETLIST 0

