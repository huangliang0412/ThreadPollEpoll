#include <string.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>                //本程序中包含了bzero()函数
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>     //互联网地址族
#include<stdlib.h>
#include<netdb.h>           //hostent gethostbyname()
#define PORT 1234   //定义端口号
#define MAXDATASIZE 5       //定义缓冲区大小

int main(int argc, char *argv[])
{
	int fd, numbytes;     //files描述符
	char buf[MAXDATASIZE];       //缓冲区用来存储接受到的文本
//	struct hostent *he;           //该结构体用来获得本地路由的信息
	//hostent记录主机的信息，包括主机名，别名，地址类型，长度，列表
	char sendBuf[MAXDATASIZE];
	struct sockaddr_in server;    //服务器端地址信息
	if(argc !=2) {
	printf("Usage: %s<IP adress>\n", argv[0]);
	exit(1);
	}
//	he=gethostbyname(argv[1]);
	//gethostbyname()返回对应于给定主机名的包含主机名字和地址信息的hostent结构指针
//	if(he==NULL) {
//		printf("gethostname() error\n");
//	}
	fd=socket(AF_INET, SOCK_STREAM, 0);
	if(fd==-1) {
		printf("socket() error");
		exit(1);
	}
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
//	server.sin_addr=*((struct in_addr *) he ->h_addr);
	inet_pton(AF_INET, argv[1], &server.sin_addr);
	if(connect(fd, (struct sockaddr *) &server, sizeof(struct sockaddr))==-1) {
		printf("connect() error\n");
		exit(1);
	}
	//printf("you got a connection ")
	while(1) 
	{ 
		printf("please input data\n");
		scanf("%s", sendBuf);
		send(fd, sendBuf, strlen(sendBuf), 0);
			
		numbytes=recv(fd, buf, MAXDATASIZE, 0);
		if(numbytes==-1)
		{
			printf("recv() error\n");
			exit(1);
		}
		buf[numbytes]='\0';
		printf("sever MESSAGE: %s\n", buf);
	}
	close(fd);         //关闭套接字
}
