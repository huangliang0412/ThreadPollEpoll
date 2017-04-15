#include "tcpsocket.h"
//#include "errexit.h"
using namespace std;

const int MAXDATASIZE = 100;
TcpSocket::TcpSocket() : m_sockfd(-1) {}

TcpSocket::~TcpSocket(){}

int TcpSocket::Creat() {
    if((m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        errExit("creat socket fail");
    }
    return 0;
}

int TcpSocket::Bind(unsigned short port, const char *ip) const{
    struct sockaddr_in ServerAddr;
    bzero(&ServerAddr, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    if(ip == NULL)
        ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        inet_aton(ip, &ServerAddr.sin_addr);

    if(::bind(m_sockfd, (const struct sockaddr*) &ServerAddr, sizeof(ServerAddr))
            == -1)
        errExit("bind fail");
    return 0;

}

int TcpSocket::Listen() const {
    if(::listen(m_sockfd, 5) == -1)
        errExit("listen error");
    return 0;
}

int TcpSocket::Listen(int BACKLOG) const {
    if(::listen(m_sockfd, BACKLOG) == -1)
        errExit("listen error");
    return 0;
}
TcpSocket TcpSocket::Accept() const {
    struct sockaddr_in ClientAddr;
    socklen_t sin_size;
    sin_size = sizeof(ClientAddr);
    TcpSocket connectFD;
    connectFD.m_sockfd = accept(this->m_sockfd, (struct sockaddr*) &ClientAddr, &sin_size);
    std::cout << "you got connection" << inet_ntoa(ClientAddr.sin_addr) << std::endl;
    if(connectFD.m_sockfd == -1)
        errExit("accept error");
    return connectFD;

}

int TcpSocket::setNoBlock(bool flag) const {
    int opt = fcntl(m_sockfd, F_GETFL, 0);
    if(flag)                                   // if flag = true, the socketfd gets noBlock.
        opt |= O_NONBLOCK;
    else
        opt &= ~O_NONBLOCK;
    if(fcntl(m_sockfd, F_SETFL, opt) == -1)
        errExit("set NoBlock error");
    return 0;
}

int TcpSocket::Close() {
    ::close(m_sockfd);
    return 0;
}

void TcpSocket::reUseAddr() const {
    int opt = SO_REUSEADDR;
    setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

int TcpSocket::Send(const std::string &str) const{
    char sendbuf[MAXDATASIZE];
    strcpy(sendbuf, str.c_str());
    if(::send(m_sockfd, sendbuf, strlen(sendbuf), 0) == -1)
        errExit("send message error");
    return strlen(sendbuf);
}

int TcpSocket::Recv() const {
    char recvbuf[MAXDATASIZE];
    int num;
    while((num = ::recv(m_sockfd, recvbuf, MAXDATASIZE, 0))) {
        recvbuf[num] = '\0';
        printf("Receive client message: %s", recvbuf);
    }
    if(num == 0)
        errExit("client has closed");
    if(num < 0)
        errExit("recv error");

    return 0;

}
