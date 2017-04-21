使用Socket + pthreadpoll + epoll IO 多路复用，实现server
高并发

模型的核心：
主线程负责所有I/O操作，收其一个请求所有数据之后，交给工作线程进行处理。
处理完成之后，把需要写回的数据还给主线程去发送给客户端，

epoll模型的使用方式，
epoll_wait()等待事件的发生，判断是否为新的套接字，如果是添加到队列
如果套接字可读，将数据读出，并传给子线程处理，子线程处理完成后，调用
epoll_ctl() EPOLL_CTL_MOD将套接字设置成EPOLLOUT，并回传给主线程，主线程
调用send()，发送给客户端并之后再次调用epoll_ctl() EPOLL_CTL_MOD将套接字
设置成EPOLLINT。
