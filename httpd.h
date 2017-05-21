//
// Created by huangliang on 17-5-21.
//

#ifndef EPOLLPTHREADPOLL_HTTPD_H
#define EPOLLPTHREADPOLL_HTTPD_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define BUF_MAX_SIZE 1024
#define URL_LEN 255
#define PATH_LEN 512

#define SERVER_STRING "Server: huangliang's Server"

#define ISspace(x) isspace((int)(x))

class Httpd {
public:
    int get_line(char **data, char *buf);         //读取每一行
    char* accept_request(char *data);
    void bad_request(char*);      //400错误,　请求方法出错
    void server_innererror(char*);  //服务器错误
    void not_found(char*);        // 404错误
    //char* success_header();    //200 ok

    void read_htmlfile(FILE*, char*);      //读取html文件
    void unimplemented();      //请求方法没有实现
    void static_html(const char*, char*);       //返回静态页面
    void execute_cgi(const char*, const char*, const char*, char*);         //调用exec函数执行cgi脚本


};

#endif //EPOLLPTHREADPOLL_HTTPD_H
