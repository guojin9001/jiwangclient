#include "mysock.h"
#include<string.h>
mysock::mysock()
{
    //初始化 winsock
     WSAStartup(0x0101,&wsaData);
    /*if(nRC)
    {
        printf("Client initialize winsock error!\n");
        return;
    }*/
    /*if(wsaData.wVersion != 0x0101)
    {
        printf("Client's winsock version error!\n");
        WSACleanup();
        return;
    }*/
    //printf("Client's winsock initialized !\n");

    //创建 client socket
    clientSock = socket(AF_INET,SOCK_STREAM,0);
   /* if(clientSock == INVALID_SOCKET)
    {
        printf("Client create socket error!\n");
        WSACleanup();
        return;
    }
    printf("Client  socket create OK!\n");
*/
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(0);
    clientAddr.sin_addr.S_un.S_addr =INADDR_ANY;//inet_addr("222.20.104.92");
    bind(clientSock,(LPSOCKADDR)&clientAddr,sizeof(clientAddr));
  /*  if(nRC == SOCKET_ERROR)
    {
        printf("Client socket bind error!\n");
        closesocket(clientSock);
        WSACleanup();
        return;
    }
    printf("Client socket bind OK!\n");
    */
}
int mysock::conToSever(int servserport,QString svrAddrs)
{
    char *ch;
    QByteArray ba = svrAddrs.toLatin1();
    ch=ba.data();
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(servserport);
    srvAddr.sin_addr.S_un.S_addr = inet_addr(ch);
    if(connect(clientSock,(LPSOCKADDR)&srvAddr,sizeof(srvAddr))==SOCKET_ERROR)
        return 0;
    return 1;
}
 int mysock::RecvData(QString& str)
 {
      if(recv(clientSock,recvBuf,20,0) == SOCKET_ERROR)
       {
            closesocket(clientSock);
            WSACleanup();
            str="";
            return -1;
       }
      str= QString(QLatin1String(recvBuf));
      return 0;
}
int mysock::sendData(const char data[50])
{
    strcpy(sendBuf,data);
          if(strlen(sendBuf) > 0)
          {
              if(send(clientSock,sendBuf,sizeof(sendBuf),0) == SOCKET_ERROR)
              {
                  closesocket(clientSock);
                  WSACleanup();
                  return FALSE;
              }
          }
     strcpy(sendBuf,"\0");
     return TRUE;
}

mysock::~mysock()
{
        closesocket(clientSock);
        WSACleanup();
}
