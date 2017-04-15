#include <iostream>
#include "tcpsocket.h"
#include "epoll.h"
#include "pthreadpoll.h"
#include "errexit.h"
using namespace std;
const int MAXDATASIZE = 100;
struct ARG {
    int connfd;
    char message[MAXDATASIZE];
};

ARG* read_message(int fd) {
    char recvbuf[MAXDATASIZE];
    int num;
    while((num = ::recv(fd, recvbuf, MAXDATASIZE, 0))) {
        recvbuf[num] = '\0';
        printf("Receive client message: %s", recvbuf);
    }
    if(num == 0)
        errExit("client has closed");
    if(num < 0)
        errExit("recv error");
    ARG* data = new ARG;
    data->connfd = fd;
    data->message = num;
    return data;
}

void handler(void* arg) {
    ARG* messageDate = (ARG*) arg;
    int length = strlen(messageDate->message);
    char reverseDate[MAXDATASIZE];
    for(int i= 0; i < length-1; ++i) {
       reverseDate[i] = messageDate[length-i -2];
    }
    reverseDate[length-1] = '\0';
    send(messageDate->connfd, reverseDate, strlen(reverseDate), 0);
    delete messageDate;
}

int main(int argc, char *argv[])
{
    TcpSocket listenfd, connectfd;
    Epoll* SocketEpoll = new Epoll;
    ThreadPoll* tPoll = new ThreadPoll;
    ARG* data;
    int nfds;
    SocketEpoll->EpollCreat(5);


    listenfd.Creat();
    listenfd.reUseAddr();
    listenfd.Bind(1234);
    listenfd.Listen();
    //connectfd =listenfd.Accept();

    SocketEpoll->AddEpollList(listenfd.m_sockfd);

    while(true)
    {

          nfds = SocketEpoll->WaitEvent();
          for(int i = 0; i < nfds; ++i) {
              if(SocketEpoll->getEventDataFd(i) == listenfd.m_sockfd) {
                  connectfd = listenfd.Accept();
                  connectfd.setNoBlock(true);
                  SocketEpoll->AddEpollList(connectfd.m_sockfd);
                }
              else {
                  data = read_message(SocketEpoll->getEventDataFd(i));
                  Task* t = new Task(&handler, (void*) data);
                  tPoll->add_task(t);
              }
          }

    }
    //while(1) {
    //cout << "Hello World!" << endl;
      //  connectfd.Recv();
    //}
    //cout << "Hello World!" << endl;

    return 0;
}
