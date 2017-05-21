//
// Created by huangliang on 17-5-21.
//
#include "httpd.h"

int Httpd::get_line(char **data, char* buf) {
    if(**data == NULL && *data == NULL)
        return 0;
    size_t i = 0;
    while(**data != '\n') {
        buf[i++] = **data;
        (*data)++;
    }
    buf[i] = '\0';
    (*data)++;
    return i + 1;
}

char* Httpd::accept_request(char *data) {
    //return nullptr;
    char buf[BUF_MAX_SIZE];
    char result_buf[BUF_MAX_SIZE];
    char method[10];
    char url[URL_LEN];
    char path[PATH_LEN];
    int cgi = 0;
    struct stat st;
    int numchars;
    char *query_string = NULL;
    size_t i = 0;
    size_t j = 0;

    numchars = get_line(&data, buf);
    for(; i < 10; ++i) {
        if(buf[i] == ' ') {
            buf[i] = '\0';
            break;
        }
        else{
            method[i] = buf[i];
        }
    }
    j = i;
    /* 目前只支持get 和　post方法*/
    if(strcasecmp(method, "GET") && strcasecmp(method, "POST")) {
        unimplemented();
        return NULL;
    }

    /*如果支持POST方法,开启cgi*/
    if(strcasecmp(method, "POST") == 0)
        cgi = 1;

    i = 0;

    while(ISspace(buf[j]) && (j < numchars))
        j++;

    while(!ISspace(buf[j]) && (i < URL_LEN) && (j < numchars)) {
        url[i++] = buf[j++];
    }
    url[i] = '\0';

    if(strcasecmp(method, "GET") == 0) {
        /* query_string 保存请求参数 index.php?r=param  问号后面的 r=param */
        query_string = url;
        while((*query_string != '?') && (*query_string != '\0'))
            query_string++;
        /*如果有？表明是动态请求, 开启cgi */
        if(*query_string == '?') {
            cgi = 1;
            *query_string = '\0';
            query_string++;
        }
    }

    /* 根目录在htdocs 下, 默认访问当前请求下的index.html*/
    sprintf(path, "htdocs%s", url);
    //if(path[strlen(path) - 1] == '/')
    if(strlen(url) == 1 && *url == '/')
        strcat(path, "index.html");

    /*找到文件,保存在结构体st中*/
    if(stat(path, &st) == -1) {
        //文件没有找到,丢弃所有http请求头信息
        //清空缓冲区
        while ((numchars > 0) && strcmp("\n", buf))
            numchars = get_line(&data, buf);
        //返回404
        not_found(result_buf);
        //return result_buf;
    }
    else {
        //如果请求参数为目录, 自动打开index.html
        if ((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path, "/index.html");

        //文件可执行
        if ((st.st_mode & S_IXUSR) ||
            (st.st_mode & S_IXGRP) ||
            (st.st_mode & S_IXOTH)    )
            cgi = 1;
        if (!cgi) {
            /* 请求静态页面 */
            static_html(path, result_buf);

        }
        else
            /*　执行cgi 程序*/
            execute_cgi(path, method, query_string, result_buf);
    }

    return result_buf;

}
/* 返回　400 */
void Httpd::bad_request(char* buf) {
    char* temp = "HTTP/1.1 400 BAD REQUEST\n"
            "Content-type: text/html\r\n"
            "<p>Your browser sent a bad request</p>\n";
    strcpy(buf, temp);
}

/* 返回500 */
void Httpd::server_innererror(char* buf) {
    //char buf[BUF_MAX_SIZE] =
    char* temp = "HTTP/1.1 500 Internal Server Error\n"
            "Content-type: text/html\r\n"
            "<p>Server error</p>\n";
    strcpy(buf, temp);
}

/* 返回404 */
void Httpd::not_found(char* buf) {
    char* temp = "HTTP/1.1 404 NOT FOUND\n"
            "Server: huangliang's Server\n"
            "Content-Type: text/html\n"
            "<html><title>Not Found</title></html>\n";
    strcpy(buf, temp);
    //return buf;
}
/*
char *Httpd::success_header() {
    char* temp = "HTTP/1.1 200 OK\n"
            "Server: huangliang's Server\n"
            "Content-Type: text/html\r\n";
    return temp;
}
*/
void Httpd::read_htmlfile(FILE *fp, char* result_buf) {
   // char buf[BUF_MAX_SIZE];
   // FILE *fp = fopen("path", "r");
    //if(fp == NULL) {
    //    perror("open file error");
    //    return;
    //}
    char* success_header = "HTTP/1.1 200 OK\n"
            "Server: huangliang's Server\n"
            "Content-Type: text/html\r\n";
    size_t len = strlen(success_header);
    strncpy(result_buf, success_header, len);
    while(!feof(fp)) {
        fgets(result_buf + len, BUF_MAX_SIZE - len, fp);
    }
}
//返回静态文件
void Httpd::static_html(const char* filename, char* result_buf) {
    FILE *resource = fopen(filename, "r");
    if(resource == NULL) {
        not_found(result_buf);
        return;
    }
    else{
        //success_header();
        read_htmlfile(resource, result_buf);
    }

}

void Httpd::execute_cgi(const char* path, const char *method,
        const char* query_string, char* result_buf) {

    char buf[BUF_MAX_SIZE];
    int cgi_output[2];
    int cgi_input[2];
    pid_t pid;
    int status;
    int numchars = 1;
    int content_length = -1;

    if(strcasecmp(method, "GET") == 0) {
        while ((numchars > 0) && strcmp("\n", buf))
            /*  */
    }
    else if(strcasecmp(method, "POST") == 0) {

        /*
        numchars = get_line()


         */
    }
    else {
        //其他http方法
    }

    if(pipe(cgi_output) < 0) {
        server_innererror(result_buf);
        return;
    }

    if(pipe(cgi_input) < 0) {
        server_innererror(result_buf);
        return;
    }

    /*创建子进程执行cgi函数,获取cgi的标准输出管道传给父进程
     * 由父进程发给客户端*/
    if((pid = fork()) < 0) {
        server_innererror(result_buf);
        return;
    }
    //success_header();

    if(pid == 0) {
        char meth_env[255];
        char query_env[255];
        char length_env[255];

        dup2(cgi_output[1], STDOUT);
        dup2(cgi_input[0], STDIN);
        close(cgi_output[0]);  //关闭cgi_output读端
        close(cgi_input[1]);   //关闭cgi_input写端

        /*添加到子进程的环境变量中*/
        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);

        if (strcasecmp(method, "GET") == 0) {
            //设置QUERY_STRING环境变量
            sprintf(query_env, "QUERY_STRING=%s", query_string);
            putenv(query_env);
        }
        else {   /* POST */
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }
        // 最后，子进程使用exec函数簇，调用外部脚本来执行
        execl(path,path,NULL);
        exit(0);
    }
    else {    /* parent */
        /* 父进程关闭cgi_output的写端和cgi_input的读端 */
        close(cgi_output[1]);
        close(cgi_input[0]);
        /* 如果是POST方法, 继续读取写入到cgi_input管道, 这是子进程会从此管道读取 */
        if (strcasecmp(method, "POST") == 0)
            //for (size_t i = 0; i < content_length; i++) {
               // recv(client, &c, 1, 0);
                write(cgi_input[1], &c, 1);
            }
        /* 从cgi_output管道中读取子进程的输出, 发送给客户端 */
        while (read(cgi_output[0], result_buf, sizeof(result_buf)) > 0)
            //send(client, &c, 1, 0);
        /* 关闭管道 */
        close(cgi_output[0]);
        close(cgi_input[1]);
        /* 等待子进程退出 */
        waitpid(pid, &status, 0);
    }



}