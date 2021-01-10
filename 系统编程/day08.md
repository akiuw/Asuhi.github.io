#Linux系统编程 day08
[**本章源代码下载**](code/day08.rar)
主要介绍POSIX共享内存接口

##主要函数介绍
###shm_open
**函数原型：**`int shm_open(const char *name, int oflag, mode_t mode);`
**功能：**创建或打开一个共享内存
**参数：**

* nanme:内存对象的名字
* oflag:与open函数类似
	O_RDONLY O_RDWR O_CREAT O_EXCL O_TRUNC
* mode：如果flag没有指定O_CREAT可以填0
如果指定了就需要设定权限

与system v不一样的是不需要指定共享内存的大小
`int shmid = shm_open("/xyz",O_CREAT | O_RDWR,0666);`
创建成功之后使用
`ls /dev/shm` 可以查看到以及创建成功

###ftruncate
**函数原型：**`int ftruncate(int fd, off_t length);`
**功能：**修改共享内存大小
**参数**

* fd 为id
* length：为修改后的大小

`ftruncate(shmid, sizeof(STU));`
`ls -al /dev/shm `可与查看到共享内存被修改成了36字节

###fstat
**函数原型：**`int fstat(int fd, struct stat *buf);`
**功能：**指定了文件描述符，查看文件的状态信息
```
struct stat buf;
    if(fstat(shmid,&buf)<0){
        ERR_EXIT("fstat");
    }
printf(" size = %ld \n mode = %o \n",buf.st_size,buf.st_mode & 0777);//mode 需要与0777取出权限
```
我们可以看到文件的权限不是0666 而是0644
这是因为存在一个文件掩码umask 
使用命令`umask` 可以查看 为0022

###shm_unlink
**原型：**`int shm_unlink(const char *name);`
和`mq_unlink`类似删除共享内存对象

共享内存映射方法：`void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);`
映射方法和之前的方法一样，使用`mmap`函数映射到进程的地址空间


##重点代码编写
**`shm_read`程序编写**
```
typedef struct stu{
    char name[32];
    int age;
}STU;

int main(){
    int shmid = shm_open("/xyz",O_RDONLY,0666);
    
    if(shmid == -1){
        ERR_EXIT("shm_open");
    }
    struct stat buf;
    if(fstat(shmid,&buf)<0){
        ERR_EXIT("fstat");
    }
    printf("size = %ld \n mode = %o \n",buf.st_size,buf.st_mode & 0777);//mode 需要与0777取出权限
    STU *p;
    
    p =(STU* )mmap(NULL,buf.st_size,PROT_READ, MAP_SHARED,shmid,0);
    if(p == MAP_FAILED)
        ERR_EXIT("mmap");
    
    printf("name = %s\nage = %d\n",p->name,p->age);

    close(shmid);
    return 0;
}

```


