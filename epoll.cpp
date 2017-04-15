#include "epoll.h"
using namespace std;
Epoll::Epoll() : epoll_fd(-1) {}

Epoll::~Epoll(){}

int Epoll::EpollCreat(int size) {
    epoll_fd = epoll_create(size);
    if(epoll_fd == -1)
        errExit("epoll create error");
    return 0;
}

int Epoll::AddEpollList(int fd) const {
    struct epoll_event ev;
    ev.events = EPOLLIN;                 // Only interested in iput events
    ev.data.fd = fd;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
        errExit("add interested event failed");
    return 0;
}

int Epoll::WaitEvent(int timout) {
    //struct epoll_event evlist[MAX_EVENTS];
    int ready;
    //queue<int> readyFD;
    ready = epoll_wait(epoll_fd, evlist, MAX_EVENTS, timout);
    if(ready == -1) {
        //if(errno == EINTR)
       //     continue;
       // else
            errExit("epoll_wait error");
    }
    if(ready == 0)
        errExit("beyond time");

    for(int j = 0; j < ready; ++j) {
        if(evlist[j].events & EPOLLIN)
            readyFd.push(evlist[j].data.fd);
    }
    return ready;
}

int Epoll::getEventDataFd(int n) {
    return evlist[n].data.fd;
}
