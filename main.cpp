#include <iostream>
#include <stdio.h>
#include "tcpsocket.h"
#include "epoll.h"
#include "pthreadpoll.h"
//#include <>
#include "errexit.h"
using namespace std;
const int MAXDATASIZE = 1024;
struct ARG {
    int connfd;
    char message[MAXDATASIZE];
    //char reverseDate[MAXDATASIZE];
};

static ARG* shareMemory = new ARG();
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

Epoll* SocketEpoll = new Epoll;

ARG* read_message(int fd) {
    char recvbuf[MAXDATASIZE];
    int num;
    while ((num = ::recv(fd, recvbuf, MAXDATASIZE, 0)) > 0) {
        //while(true) {
        //num = ::recv(fd, recvbuf, MAXDATASIZE, 0);
        recvbuf[num] = '\0';
        printf("Receive client message %s\n", recvbuf);
    }
        if (num == 0) {
            close(fd);
            errExit("client has closed");
            //return NULL;
        }
       // if (num < 0 && (errno == EAGAIN || errno ==EWOULDBLOCK))
            //return;
        if(num < 0 && (errno != EAGAIN))
            errExit("recv error");
   // }
    ARG* data = new ARG;
    data->connfd = fd;
    //data->message = num;
    strcpy(data->message, recvbuf);
    return data;
}

void handler(void* arg) {
    ARG* messageDate = (ARG*) arg;
    int length = strlen(messageDate->message);
    //cout << length << endl;
    int s;
    char reverseDate[MAXDATASIZE];
    for(int i= 0; i < length; ++i) {
       reverseDate[i] = messageDate->message[length-i-1];
    }
    reverseDate[length] = '\0';
    s = pthread_mutex_lock(&mtx);
    if(s != 0)
        errExit("share memory lock error");
    shareMemory->connfd = messageDate->connfd;
    strcpy(shareMemory->message, reverseDate);
    SocketEpoll->ModWriteEpollList(shareMemory->connfd);
    s = pthread_mutex_unlock(&mtx);
    if(s != 0)
        errExit("sharememory unlock error");
    //send(messageDate->connfd, reverseDate, strlen(reverseDate), 0);
    delete messageDate;
}

int main(int argc, char *argv[])
{
    cout << "hello world" << endl;
    TcpSocket listenfd, connectfd;
    //Epoll* SocketEpoll = new Epoll;
    ThreadPoll* tPoll = new ThreadPoll;
    tPoll->initialize_threadpoll();
    tPoll->print_pid();
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

          nfds = SocketEpoll->WaitEvent(-1);
          for(int i = 0; i < nfds; ++i) {
              //int sockfd = SocketEpoll->getEventDataFd(i);
              if(SocketEpoll->getEventDataFd(i) == listenfd.m_sockfd) {
                  connectfd = listenfd.Accept();
                  connectfd.setNoBlock(true);
                  SocketEpoll->AddEpollList(connectfd.m_sockfd);
                }
              else if(SocketEpoll->isReadAvailable(i)) {
                  data = read_message(SocketEpoll->getEventDataFd(i));
                  Task* t = new Task(&handler, (void*) data);
                  tPoll->add_task(t);
              }
              else if(SocketEpoll->isWriteAvailable(i)) {
                  int sockfd = SocketEpoll->getEventDataFd(i);
                  send(shareMemory->connfd, shareMemory->message, strlen(shareMemory->message), 0);

                  SocketEpoll->ModReadEpollList(sockfd);
              }
          }
          //tPoll->pthreads_join();
    }
    //while(1) {
    //cout << "Hello World!" << endl;
      //  connectfd.Recv();
    //}
    //cout << "Hello World!" << endl;
    delete(tPoll);
    tPoll = NULL;
    delete(SocketEpoll);
    SocketEpoll = NULL;
    return 0;
}
