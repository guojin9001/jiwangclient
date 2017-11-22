#ifndef MYSOCK_H
#define MYSOCK_H
#include<WinSock2.h>
#include<QString>
#pragma comment(lib,"ws2_32.lib")
class mysock
{
    WSADATA wsaData;


    char sendBuf[1024],recvBuf[1024];
    FD_SET rfds,wfds;
    u_long uNonBlock;
public:
    SOCKET clientSock;
    sockaddr_in srvAddr,clientAddr;
    mysock();
    int conToSever(int servserport,QString svrAddrs);
    int sendData(const char data[50]);
    int RecvData(QString &str);
    ~mysock();
};
#endif // MYSOCK_H
