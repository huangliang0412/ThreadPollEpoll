#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <string>

#include "errexit.h"
class TcpSocket {
public:
    TcpSocket();
    ~TcpSocket();

    int Creat();
    int Bind(unsigned short port, const char* ip = NULL) const;
    int Listen() const;
    int Listen(int BACKLOG) const;
    int Connect(unsigned short port, const char* ip) const;
    TcpSocket Accept() const;
    int Close();

    int Send(const std::string& str) const;
    int Recv() const;

    void reUseAddr() const;               //set ipadress reuse

    int setNoBlock(bool flag) const;      //set block or unblock
    /*
    int getSocketFd() const {
        return m_sockfd;
    }

    void setSockFd(int sockfd) const {
        m_sockfd = sockfd;
    }

private:
    int m_sockfd;
    */
    int m_sockfd;
};

#endif // TCPSOCKET_H
