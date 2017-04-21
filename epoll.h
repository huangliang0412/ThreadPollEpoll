#ifndef EPOLL_H
#define EPOLL_H

#include "errexit.h"
#include <sys/epoll.h>

#include <queue>
#define MAX_EVENTS 5  /* Maximum number of events to be
                        returned from a single epoll_wait call*/
class Epoll {
public:
    Epoll();
    ~Epoll();
    int EpollCreat(int size);
    int AddEpollList(int fd) const;
    int ModWriteEpollList(int fd) const;
    int ModReadEpollList(int fd) const;
    //int DelEpollList(int fd) const;
    int WaitEvent(int timeout);
    int getEventDataFd(int n);   //acquire the idex of n data.id
    bool isReadAvailable(int n);
    bool isWriteAvailable(int n);
private:
    int epoll_fd;
    struct epoll_event evlist[MAX_EVENTS];
    std::queue<int> readyFd;
};

#endif // EPOLL_H
