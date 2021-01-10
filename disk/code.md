#CGI代码编写

##登录

1. FCGI_Accept阻塞等待用户连接
2. getenv("CONTENT_LENGTH")得到数据长度
3. len大于0表示有用户信息，调用fread读取
4. get_login_info得到用户名和密码
5. check_user_pwd判断正确性
6. set_token设置token
7. 返回000代表成功，并且返回token


get_login_info 
得到user的json对象，child1 在得到valuestring 
得到pwd的json对象，child2 得到valuestring
```
int get_login_info(char *login_buf, char *user, char *pwd)
{
    int ret = 0;

    //解析json包
    //解析一个json字符串为cJSON对象
    cJSON * root = cJSON_Parse(login_buf);
    if(NULL == root)
    {
        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "cJSON_Parse err\n");
        ret = -1;
        goto END;
    }

    //返回指定字符串对应的json对象
    //用户
    cJSON *child1 = cJSON_GetObjectItem(root, "user");
    if(NULL == child1)
    {
        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "cJSON_GetObjectItem err\n");
        ret = -1;
        goto END;
    }

    //LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "child1->valuestring = %s\n", child1->valuestring);
    strcpy(user, child1->valuestring); //拷贝内容

    //密码
    cJSON *child2 = cJSON_GetObjectItem(root, "pwd");
    if(NULL == child2)
    {
        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "cJSON_GetObjectItem err\n");
        ret = -1;
        goto END;
    }

    //LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "child1->valuestring = %s\n", child1->valuestring);
    strcpy(pwd, child2->valuestring); //拷贝内容

END:
    if(root != NULL)
    {
        cJSON_Delete(root);//删除json对象
        root = NULL;
    }

    return ret;
}
```

check_user_pwd
获取数据库用户名、用户密码、数据库标示等信息
连接数据库msql_conn 使用sql语句查询user对于的密码把查询结果保存到tmp
对比tmp和pwd
```
int check_user_pwd( char *user, char *pwd )
{
    char sql_cmd[SQL_MAX_LEN] = {0};
    int ret = 0;

    //获取数据库用户名、用户密码、数据库标示等信息
    char mysql_user[256] = {0};
    char mysql_pwd[256] = {0};
    char mysql_db[256] = {0};
    get_mysql_info(mysql_user, mysql_pwd,  mysql_db);
    LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "mysql_user = %s, mysql_pwd = %s, mysql_db = %s\n", mysql_user, mysql_pwd, mysql_db);

    //connect the database
    MYSQL *conn = msql_conn(mysql_user, mysql_pwd, mysql_db);
    if (conn == NULL)
    {
        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "msql_conn err\n");
        return -1;
    }

    //设置数据库编码，主要处理中文编码问题
    mysql_query(conn, "set names utf8");

    //sql语句，查找某个用户对应的密码
    sprintf(sql_cmd, "select password from user where name=\"%s\"", user);

    //deal result
    char tmp[PWD_LEN] = {0};

    //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
    process_result_one(conn, sql_cmd, tmp); //执行sql语句，结果集保存在tmp
    if(strcmp(tmp, pwd) == 0)
    {
        ret = 0;
    }
    else
    {
        ret = -1;
    }

    mysql_close(conn);


    return ret;
}
```

set_token
读取redis的信息并且连接redis
产生4个1000以内的随机数
使用DesEnc加密，在使用base64，在使用md5
redis保存这个token,24小时
```
int set_token(char *user, char *token)
{
    int ret = 0;
    redisContext * redis_conn = NULL;

    //redis 服务器ip、端口
    char redis_ip[30] = {0};
    char redis_port[10] = {0};

    //读取redis配置信息
    get_cfg_value(CFG_PATH, "redis", "ip", redis_ip);
    get_cfg_value(CFG_PATH, "redis", "port", redis_port);
    LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "redis:[ip=%s,port=%s]\n", redis_ip, redis_port);

    //连接redis数据库
    redis_conn = rop_connectdb_nopwd(redis_ip, redis_port);
    if (redis_conn == NULL)
    {
        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "redis connected error\n");
        ret = -1;
        goto END;
    }

    //产生4个1000以内的随机数
    int rand_num[4] = {0};
    int i = 0;

    //设置随机种子
    srand((unsigned int)time(NULL));
    for(i = 0; i < 4; ++i)
    {
        rand_num[i] = rand()%1000;//随机数
    }

    char tmp[1024] = {0};
    sprintf(tmp, "%s%d%d%d%d", user, rand_num[0], rand_num[1], rand_num[2], rand_num[3]);
    LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "tmp = %s\n", tmp);

    //加密
    char enc_tmp[1024*2] = {0};
    int enc_len = 0;
    ret = DesEnc((unsigned char *)tmp, strlen(tmp), (unsigned char *)enc_tmp, &enc_len);
    if(ret != 0)
    {
        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "DesEnc error\n");
        ret = -1;
        goto END;
    }

    //to base64
    char base64[1024*3] = {0};
    base64_encode((const unsigned char *)enc_tmp, enc_len, base64); //base64编码
    LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "base64 = %s\n", base64);

    //to md5
    MD5_CTX md5;
    MD5Init(&md5);
    unsigned char decrypt[16];
    MD5Update(&md5, (unsigned char *)base64, strlen(base64) );
    MD5Final(&md5, decrypt);


    char str[100] = { 0 };
    for (i = 0; i < 16; i++)
    {
        sprintf(str, "%02x", decrypt[i]);
        strcat(token, str);
    }

    // redis保存此字符串，用户名：token, 有效时间为24小时
    ret = rop_setex_string(redis_conn, user, 86400, token);
    //ret = rop_setex_string(redis_conn, user, 30, token); //30秒


END:
    if(redis_conn != NULL)
    {
        rop_disconnect(redis_conn);
    }

    return ret;

}
```


##注册

和登录相同得到带注册的用户名和密码
调用注册函数user_register
002表示成功004表示失败

连接数据库
从传过来的json文件中得到注册信息
得到当前时间戳
使用sql语句插入到mysql数据库
```
int user_register( char *reg_buf )
{
    int ret = 0;
    MYSQL *conn = NULL;

    //获取数据库用户名、用户密码、数据库标示等信息
    char mysql_user[256] = {0};
    char mysql_pwd[256] = {0};
    char mysql_db[256] = {0};
    ret = get_mysql_info(mysql_user, mysql_pwd,  mysql_db);
    if(ret != 0)
    {
        goto END;
    }
    LOG(REG_LOG_MODULE, REG_LOG_PROC, "mysql_user = %s, mysql_pwd = %s, mysql_db = %s\n", mysql_user, mysql_pwd, mysql_db);

    //获取注册用户的信息
    char user[128];
    char nick_name[128];
    char pwd[128];
    char tel[128];
    char email[128];
    ret = get_reg_info(reg_buf, user, nick_name, pwd, tel, email);
    if(ret != 0)
    {
        goto END;
    }
    LOG(REG_LOG_MODULE, REG_LOG_PROC, "user = %s, nick_name = %s, pwd = %s, tel = %s, email = %s\n", user, nick_name, pwd, tel, email);

    //connect the database
    conn = msql_conn(mysql_user, mysql_pwd, mysql_db);
    if (conn == NULL)
    {
        LOG(REG_LOG_MODULE, REG_LOG_PROC, "msql_conn err\n");
        ret = -1;
        goto END;
    }

    //设置数据库编码，主要处理中文编码问题
    mysql_query(conn, "set names utf8");

    char sql_cmd[SQL_MAX_LEN] = {0};

    sprintf(sql_cmd, "select * from user where name = '%s'", user);

    //查看该用户是否存在
    int ret2 = 0;
    //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
    ret2 = process_result_one(conn, sql_cmd, NULL); //指向sql查询语句
    if(ret2 == 2) //如果存在
    {
        LOG(REG_LOG_MODULE, REG_LOG_PROC, "【%s】该用户已存在\n");
        ret = -2;
        goto END;
    }

    //当前时间戳
    struct timeval tv;
    struct tm* ptm;
    char time_str[128];

    //使用函数gettimeofday()函数来得到时间。它的精度可以达到微妙
    gettimeofday(&tv, NULL);
    ptm = localtime(&tv.tv_sec);//把从1970-1-1零点零分到当前时间系统所偏移的秒数时间转换为本地时间
    //strftime() 函数根据区域设置格式化本地时间/日期，函数的功能将时间格式化，或者说格式化一个时间字符串
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm);

    //sql 语句, 插入注册信息
    sprintf(sql_cmd, "insert into user (name, nickname, password, phone, createtime, email) values ('%s', '%s', '%s', '%s', '%s', '%s')", user, nick_name, pwd, tel, time_str ,email);

    if(mysql_query(conn, sql_cmd) != 0)
    {
        LOG(REG_LOG_MODULE, REG_LOG_PROC, "%s 插入失败：%s\n", sql_cmd, mysql_error(conn));
        ret = -1;
        goto END;
    }

END:
    if(conn != NULL)
    {
        mysql_close(conn); //断开数据库连接
    }

    return ret;
}

```


##上传

1. 读取数据库信息read_cfg
2. 得到上传文件并且缓存到服务器recv_save_file
3. 将该文件存入fastDFS中,并得到文件的file_id  upload_to_dstorage
4. 删除本地临时存放的上传文件unlink
5. 得到文件所存放storage的host_name   make_file_url
6. 将该文件的FastDFS相关信息存入mysql中  store_fileinfo_to_mysql
7. 008上传成功 009上传失败


recv_save_file

```
int recv_save_file(long len, char *user, char *filename, char *md5, long *p_size)
{
    int ret = 0;
    char *file_buf = NULL;
    char *begin = NULL;
    char *p, *q, *k;

    char content_text[TEMP_BUF_MAX_LEN] = {0}; //文件头部信息
    char boundary[TEMP_BUF_MAX_LEN] = {0};     //分界线信息

    //==========> 开辟存放文件的 内存 <===========
    file_buf = (char *)malloc(len);
    if (file_buf == NULL)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "malloc error! file size is to big!!!!\n");
        return -1;
    }

    int ret2 = fread(file_buf, 1, len, stdin); //从标准输入(web服务器)读取内容
    if(ret2 == 0)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "fread(file_buf, 1, len, stdin) err\n");
        ret = -1;
        goto END;
    }

    //===========> 开始处理前端发送过来的post数据格式 <============
    begin = file_buf;    //内存起点
    p = begin;

    /*
       ------WebKitFormBoundary88asdgewtgewx\r\n
       Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
       Content-Type: application/octet-stream\r\n
       \r\n
       真正的文件内容\r\n
       ------WebKitFormBoundary88asdgewtgewx
       */

    //get boundary 得到分界线, ------WebKitFormBoundary88asdgewtgewx
    p = strstr(begin, "\r\n");
    if (p == NULL)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"wrong no boundary!\n");
        ret = -1;
        goto END;
    }

    //拷贝分界线
    strncpy(boundary, begin, p-begin);
    boundary[p-begin] = '\0';   //字符串结束符
    //LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"boundary: [%s]\n", boundary);

    p += 2;//\r\n
    //已经处理了p-begin的长度
    len -= (p-begin);

    //get content text head
    begin = p;

    //Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    p = strstr(begin, "\r\n");
    if(p == NULL)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"ERROR: get context text error, no filename?\n");
        ret = -1;
        goto END;
    }
    strncpy(content_text, begin, p-begin);
    content_text[p-begin] = '\0';
    //LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"content_text: [%s]\n", content_text);

    p += 2;//\r\n
    len -= (p-begin);

    //========================================获取文件上传者
    //Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    //                                ↑
    q = begin;
    q = strstr(begin, "user=");

    //Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    //                                      ↑
    q += strlen("user=");
    q++;    //跳过第一个"

    //Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    //                                          ↑
    k = strchr(q, '"');
    strncpy(user, q, k-q);  //拷贝用户名
    user[k-q] = '\0';

    //去掉一个字符串两边的空白字符
    trim_space(user);   //util_cgi.h

    //========================================获取文件名字
    //"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    //   ↑
    begin = k;
    q = begin;
    q = strstr(begin, "filename=");

    //"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    //             ↑
    q += strlen("filename=");
    q++;    //跳过第一个"

    //"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    //                    ↑
    k = strchr(q, '"');
    strncpy(filename, q, k-q);  //拷贝文件名
    filename[k-q] = '\0';

    trim_space(filename);   //util_cgi.h

    //========================================获取文件MD5码
    //"; md5="xxxx"; size=10240\r\n
    //   ↑
    begin = k;
    q = begin;
    q = strstr(begin, "md5=");

    //"; md5="xxxx"; size=10240\r\n
    //        ↑
    q += strlen("md5=");
    q++;    //跳过第一个"

    //"; md5="xxxx"; size=10240\r\n
    //            ↑
    k = strchr(q, '"');
    strncpy(md5, q, k-q);   //拷贝文件名
    md5[k-q] = '\0';

    trim_space(md5);    //util_cgi.h

    //========================================获取文件大小
    //"; size=10240\r\n
    //   ↑
    begin = k;
    q = begin;
    q = strstr(begin, "size=");

    //"; size=10240\r\n
    //        ↑
    q += strlen("size=");

    //"; size=10240\r\n
    //             ↑
    k = strstr(q, "\r\n");
    char tmp[256] = {0};
    strncpy(tmp, q, k-q);   //内容
    tmp[k-q] = '\0';

    *p_size = strtol(tmp, NULL, 10); //字符串转long

    begin = p;
    p = strstr(begin, "\r\n");
    p += 4;//\r\n\r\n
    len -= (p-begin);

    //下面才是文件的真正内容

    /*
       ------WebKitFormBoundary88asdgewtgewx\r\n
       Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
       Content-Type: application/octet-stream\r\n
       \r\n
       真正的文件内容\r\n
       ------WebKitFormBoundary88asdgewtgewx
       */

    begin = p;
    //find file's end
    p = memstr(begin, len, boundary);//util_cgi.h， 找文件结尾
    if (p == NULL)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "memstr(begin, len, boundary) error\n");
        ret = -1;
        goto END;
    }
    else
    {
        p = p - 2;//\r\n
    }

    //begin---> file_len = (p-begin)

    //=====> 此时begin-->p两个指针的区间就是post的文件二进制数据
    //======>将数据写入文件中,其中文件名也是从post数据解析得来  <===========

    int fd = 0;
    fd = open(filename, O_CREAT|O_WRONLY, 0644);
    if (fd < 0)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"open %s error\n", filename);
        ret = -1;
        goto END;
    }

    //ftruncate会将参数fd指定的文件大小改为参数length指定的大小
    ftruncate(fd, (p-begin));
    write(fd, begin, (p-begin));
    close(fd);

END:
    free(file_buf);
    return ret;
}
```


upload_to_dstorage

```
int upload_to_dstorage(char *filename, char *fileid)
{
    int ret = 0;

    pid_t pid;
    int fd[2];

    //无名管道的创建
    if (pipe(fd) < 0)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"pip error\n");
        ret = -1;
        goto END;
    }

    //创建进程
    pid = fork();
    if (pid < 0)//进程创建失败
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"fork error\n");
        ret = -1;
        goto END;
    }

    if(pid == 0) //子进程
    {
        //关闭读端
        close(fd[0]);

        //将标准输出 重定向 写管道
        dup2(fd[1], STDOUT_FILENO); //dup2(fd[1], 1);


        //通过execlp执行fdfs_upload_file
        //execlp("fdfs_upload_file", "fdfs_upload_file", FDFS_CLIENT_CONF, filename, NULL);

        //读取fdfs client 配置文件的路径
        char fdfs_cli_conf_path[256] = {0};
        get_cfg_value(CFG_PATH, "dfs_path", "client", fdfs_cli_conf_path);

        //通过execlp执行fdfs_upload_file
        execlp("fdfs_upload_file", "fdfs_upload_file", fdfs_cli_conf_path, filename, NULL);

        //执行失败
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "execlp fdfs_upload_file error\n");

        close(fd[1]);
    }
    else //父进程
    {
        //关闭写端
        close(fd[1]);

        //从管道中去读数据
        read(fd[0], fileid, TEMP_BUF_MAX_LEN);

        //去掉一个字符串两边的空白字符
        trim_space(fileid);

        if (strlen(fileid) == 0)
        {
            LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"[upload FAILED!]\n");
            ret = -1;
            goto END;
        }

        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "get [%s] succ!\n", fileid);

        wait(NULL); //等待子进程结束，回收其资源
        close(fd[0]);
    }

END:
    return ret;
}
```

make_file_url

```
int make_file_url(char *fileid, char *fdfs_file_url)
{
    int ret = 0;

    char *p = NULL;
    char *q = NULL;
    char *k = NULL;

    char fdfs_file_stat_buf[TEMP_BUF_MAX_LEN] = {0};
    char fdfs_file_host_name[HOST_NAME_LEN] = {0};  //storage所在服务器ip地址

    pid_t pid;
    int fd[2];

    //无名管道的创建
    if (pipe(fd) < 0)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "pip error\n");
        ret = -1;
        goto END;
    }

    //创建进程
    pid = fork();
    if (pid < 0)//进程创建失败
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"fork error\n");
        ret = -1;
        goto END;
    }

    if(pid == 0) //子进程
    {
        //关闭读端
        close(fd[0]);

        //将标准输出 重定向 写管道
        dup2(fd[1], STDOUT_FILENO); //dup2(fd[1], 1);

        //读取fdfs client 配置文件的路径
        char fdfs_cli_conf_path[256] = {0};
        get_cfg_value(CFG_PATH, "dfs_path", "client", fdfs_cli_conf_path);

        execlp("fdfs_file_info", "fdfs_file_info", fdfs_cli_conf_path, fileid, NULL);

        //执行失败
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "execlp fdfs_file_info error\n");

        close(fd[1]);
    }
    else //父进程
    {
        //关闭写端
        close(fd[1]);

        //从管道中去读数据
        read(fd[0], fdfs_file_stat_buf, TEMP_BUF_MAX_LEN);
        //LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "get file_ip [%s] succ\n", fdfs_file_stat_buf);

        wait(NULL); //等待子进程结束，回收其资源
        close(fd[0]);

        //拼接上传文件的完整url地址--->http://host_name/group1/M00/00/00/D12313123232312.png
        p = strstr(fdfs_file_stat_buf, "source ip address: ");

        q = p + strlen("source ip address: ");
        k = strstr(q, "\n");

        strncpy(fdfs_file_host_name, q, k-q);
        fdfs_file_host_name[k-q] = '\0';

        //printf("host_name:[%s]\n", fdfs_file_host_name);

        //读取storage_web_server服务器的端口
        char storage_web_server_port[20] = {0};
        get_cfg_value(CFG_PATH, "storage_web_server", "port", storage_web_server_port);
        strcat(fdfs_file_url, "http://");
        strcat(fdfs_file_url, fdfs_file_host_name);
        strcat(fdfs_file_url, ":");
        strcat(fdfs_file_url, storage_web_server_port);
        strcat(fdfs_file_url, "/");
        strcat(fdfs_file_url, fileid);

        //printf("[%s]\n", fdfs_file_url);
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "file url is: %s\n", fdfs_file_url);
    }

END:
    return ret;
}

```


store_fileinfo_to_mysql
```
int store_fileinfo_to_mysql(char *user, char *filename, char *md5, long size, char *fileid, char *fdfs_file_url)
{
    int ret = 0;
    MYSQL *conn = NULL; //数据库连接句柄

    time_t now;;
    char create_time[TIME_STRING_LEN];
    char suffix[SUFFIX_LEN];
    char sql_cmd[SQL_MAX_LEN] = {0};

    //连接 mysql 数据库
    conn = msql_conn(mysql_user, mysql_pwd, mysql_db);
    if (conn == NULL)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "msql_conn connect err\n");
        ret = -1;
        goto END;
    }

    //设置数据库编码
    mysql_query(conn, "set names utf8");

    //得到文件后缀字符串 如果非法文件后缀,返回"null"
    get_file_suffix(filename, suffix); //mp4, jpg, png

    //sql 语句
    /*
       -- =============================================== 文件信息表
       -- md5 文件md5
       -- file_id 文件id
       -- url 文件url
       -- size 文件大小, 以字节为单位
       -- type 文件类型： png, zip, mp4……
       -- count 文件引用计数， 默认为1， 每增加一个用户拥有此文件，此计数器+1
       */
    sprintf(sql_cmd, "insert into file_info (md5, file_id, url, size, type, count) values ('%s', '%s', '%s', '%ld', '%s', %d)",
            md5, fileid, fdfs_file_url, size, suffix, 1);

    if (mysql_query(conn, sql_cmd) != 0) //执行sql语句
    {
        //print_error(conn, "插入失败");
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "%s 插入失败: %s\n", sql_cmd, mysql_error(conn));
        ret = -1;
        goto END;
    }

    LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "%s 文件信息插入成功\n", sql_cmd);


    //获取当前时间
    now = time(NULL);
    strftime(create_time, TIME_STRING_LEN-1, "%Y-%m-%d %H:%M:%S", localtime(&now));

    /*
       -- =============================================== 用户文件列表
       -- user 文件所属用户
       -- md5 文件md5
       -- createtime 文件创建时间
       -- filename 文件名字
       -- shared_status 共享状态, 0为没有共享， 1为共享
       -- pv 文件下载量，默认值为0，下载一次加1
       */
    //sql语句
    sprintf(sql_cmd, "insert into user_file_list(user, md5, createtime, filename, shared_status, pv) values ('%s', '%s', '%s', '%s', %d, %d)", user, md5, create_time, filename, 0, 0);
    if(mysql_query(conn, sql_cmd) != 0)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "%s 操作失败: %s\n", sql_cmd, mysql_error(conn));
        ret = -1;
        goto END;
    }

    //查询用户文件数量
    sprintf(sql_cmd, "select count from user_file_count where user = '%s'", user);
    int ret2 = 0;
    char tmp[512] = {0};
    int count = 0;
    //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
    ret2 = process_result_one(conn, sql_cmd, tmp); //执行sql语句
    if(ret2 == 1) //没有记录
    {
        //插入记录
        sprintf(sql_cmd, " insert into user_file_count (user, count) values('%s', %d)", user, 1);
    }
    else if(ret2 == 0)
    {
        //更新用户文件数量count字段
        count = atoi(tmp);
        sprintf(sql_cmd, "update user_file_count set count = %d where user = '%s'", count+1, user);
    }


    if(mysql_query(conn, sql_cmd) != 0)
    {
        LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "%s 操作失败: %s\n", sql_cmd, mysql_error(conn));
        ret = -1;
        goto END;
    }

END:
    if (conn != NULL)
    {
        mysql_close(conn); //断开数据库连接
    }

    return ret;
}
```


##下载

1. 解析json中的信息get_md5_info
2. 登录验证verify_token
3. 秒传处理deal_md5


get_md5_info
```
int get_md5_info(char *buf, char *user, char *token, char *md5, char *filename)
{
    int ret = 0;

    //解析json中信息
    /*
     * {
        user:xxxx,
        token: xxxx,
        md5:xxx,
        fileName: xxx
       }
     */

    //解析json包
    //解析一个json字符串为cJSON对象
    cJSON * root = cJSON_Parse(buf);
    if(NULL == root)
    {
        LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "cJSON_Parse err\n");
        ret = -1;
        goto END;
    }

    //返回指定字符串对应的json对象
    //用户
    cJSON *child1 = cJSON_GetObjectItem(root, "user");
    if(NULL == child1)
    {
        LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "cJSON_GetObjectItem err\n");
        ret = -1;
        goto END;
    }

    //LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "child1->valuestring = %s\n", child1->valuestring);
    strcpy(user, child1->valuestring); //拷贝内容

    //MD5
    cJSON *child2 = cJSON_GetObjectItem(root, "md5");
    if(NULL == child2)
    {
        LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "cJSON_GetObjectItem err\n");
        ret = -1;
        goto END;
    }
    strcpy(md5, child2->valuestring); //拷贝内容

    //文件名字
    cJSON *child3 = cJSON_GetObjectItem(root, "fileName");
    if(NULL == child3)
    {
        LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "cJSON_GetObjectItem err\n");
        ret = -1;
        goto END;
    }
    strcpy(filename, child3->valuestring); //拷贝内容

    //token
    cJSON *child4 = cJSON_GetObjectItem(root, "token");
    if(NULL == child4)
    {
        LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "cJSON_GetObjectItem err\n");
        ret = -1;
        goto END;
    }

    strcpy(token, child4->valuestring); //拷贝内容

END:
    if(root != NULL)
    {
        cJSON_Delete(root);//删除json对象
        root = NULL;
    }

    return ret;
}
```

deal_md5
```
int deal_md5(char *user, char *md5, char *filename)
{
    int ret = 0;
    MYSQL *conn = NULL;
    int ret2 = 0;
    char tmp[512] = {0};
    char sql_cmd[SQL_MAX_LEN] = {0};
    char *out = NULL;

    //connect the database
    conn = msql_conn(mysql_user, mysql_pwd, mysql_db);
    if (conn == NULL)
    {
        LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "msql_conn err\n");
        ret = -1;
        goto END;
    }

    //设置数据库编码，主要处理中文编码问题
    mysql_query(conn, "set names utf8");

    /*
    秒传文件：
        文件已存在：{"code":"004"}
        秒传成功：  {"code":"005"}
        秒传失败：  {"code":"006"}

    */

    //查看数据库是否有此文件的md5
    //如果没有，返回 {"code":"006"}， 代表不能秒传

    //如果有
    //1、修改file_info中的count字段，+1 （count 文件引用计数）
    //   update file_info set count = 2 where md5 = "bae488ee63cef72efb6a3f1f311b3743";
    //2、user_file_list插入一条数据

    //sql 语句，获取此md5值文件的文件计数器 count
    sprintf(sql_cmd, "select count from file_info where md5 = '%s'", md5);

    //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
    ret2 = process_result_one(conn, sql_cmd, tmp); //执行sql语句

    if(ret2 == 0) //有结果，说明服务器已经有此文件
    {
        int count = atoi(tmp); //字符串转整型，文件计数器

        //查看此用户是否已经有此文件，如果存在说明此文件已上传，无需再上传
        sprintf(sql_cmd, "select * from user_file_list where user = '%s' and md5 = '%s' and filename = '%s'", user, md5, filename);

        //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
        ret2 = process_result_one(conn, sql_cmd, NULL); //执行sql语句，最后一个参数为NULL，只做查询
        if(ret2 == 2) //如果有结果，说明此用户已经保存此文件
        {
            LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "%s[filename:%s, md5:%s]已存在\n", user, filename, md5);
            ret = -2; //0秒传成功，-1出错，-2此用户已拥有此文件， -3秒传失败
            goto END;
        }

        //1、修改file_info中的count字段，+1 （count 文件引用计数）
        sprintf(sql_cmd, "update file_info set count = %d where md5 = '%s'", ++count, md5);//前置++
        if(mysql_query(conn, sql_cmd) != 0)
        {
            LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "%s 操作失败： %s\n", sql_cmd, mysql_error(conn));
            ret = -1;
            goto END;
        }

        //2、user_file_list, 用户文件列表插入一条数据
        //当前时间戳
        struct timeval tv;
        struct tm* ptm;
        char time_str[128];

        //使用函数gettimeofday()函数来得到时间。它的精度可以达到微妙
        gettimeofday(&tv, NULL);
        ptm = localtime(&tv.tv_sec);//把从1970-1-1零点零分到当前时间系统所偏移的秒数时间转换为本地时间
        //strftime() 函数根据区域设置格式化本地时间/日期，函数的功能将时间格式化，或者说格式化一个时间字符串
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm);

        //sql语句
        /*
        -- =============================================== 用户文件列表
        -- user	文件所属用户
        -- md5 文件md5
        -- createtime 文件创建时间
        -- filename 文件名字
        -- shared_status 共享状态, 0为没有共享， 1为共享
        -- pv 文件下载量，默认值为0，下载一次加1
        */
        sprintf(sql_cmd, "insert into user_file_list(user, md5, createtime, filename, shared_status, pv) values ('%s', '%s', '%s', '%s', %d, %d)", user, md5, time_str, filename, 0, 0);
        if(mysql_query(conn, sql_cmd) != 0)
        {
            LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "%s 操作失败： %s\n", sql_cmd, mysql_error(conn));
            ret = -1;
            goto END;
        }

        //查询用户文件数量
        sprintf(sql_cmd, "select count from user_file_count where user = '%s'", user);
        count = 0;

        //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
        ret2 = process_result_one(conn, sql_cmd, tmp); //指向sql语句
        if(ret2 == 1) //没有记录
        {
            //数据库插入此记录
            sprintf(sql_cmd, " insert into user_file_count (user, count) values('%s', %d)", user, 1);
        }
        else if(ret2 == 0)
        {
            //更新用户文件数量count字段
            count = atoi(tmp);
            sprintf(sql_cmd, "update user_file_count set count = %d where user = '%s'", count+1, user);
        }


        if(mysql_query(conn, sql_cmd) != 0)
        {
            LOG(MD5_LOG_MODULE, MD5_LOG_PROC, "%s 操作失败： %s\n", sql_cmd, mysql_error(conn));
            ret = -1;
            goto END;
        }

    }
    else if(1 == ret2)//没有结果，秒传失败
    {
       ret = -3;//0秒传成功，-1出错，-2此用户已拥有此文件， -3秒传失败
       goto END;
    }


END:
    //ret的值：0秒传成功，-1出错，-2此用户已拥有此文件， -3秒传失败
    /*
    秒传文件：
        文件已存在：{"code":"005"}
        秒传成功：  {"code":"006"}
        秒传失败：  {"code":"007"}

    */

    //返回前端情况
    if(ret == 0)
    {
        out = return_status("006"); //common.h
    }
    else if(ret == -2)
    {
        out = return_status("005"); //common.h
    }
    else
    {
        out = return_status("007"); //common.h
    }

    if(out != NULL)
    {
        printf(out); //给前端反馈信息
        free(out); //记得释放
    }


    if(conn != NULL)
    {
        mysql_close(conn); //断开数据库连接
    }

    return ret;
}

```
