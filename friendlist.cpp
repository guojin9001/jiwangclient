#include "friendlist.h"
#include "ui_friendlist.h"

friendlist::friendlist(QString str1, QString str3,mysock *sock,QString str2, QWidget *parent) :
    Friendname(str1),
    lixianXX(str3),
    Username(str2),
    QDialog(parent),
    soc(sock),
    ui(new Ui::friendlist)
{
    qRegisterMetaType<SOCKET>("SOCKET");
    qRegisterMetaType<sockaddr_in>("sockaddr_in");
    ui->setupUi(this);
   int j=0;
   for(int i=0;i!=str1.length();i++)
   {
       if(str1[i]=='#')
       {
           QStringList userName;
           userName<<(str1.mid(j,i-j));
           if((str1.mid(j,i-j))!=Username)
           QTreeWidgetItem *root=new QTreeWidgetItem(ui->treeWidget,userName);
           j=i+1;
       }
   }
   j=0;
   for(int i=0;i!=str3.length();i++)
   {
       if(str3[i]=='#')
       {
           QString lixianxiaoxi;
           lixianxiaoxi=str3.mid(j,i-j);
           ui->textBrowser->append(lixianxiaoxi);
           j=i+1;
       }
   }
   //以下是建立接收p2p聊天的线程以及参数传递
    param *pa=new param;
    pa->l=this;
    pa->s=(*sock).clientSock;
    connect(this,SIGNAL(valuechanged(QString)),this,SLOT(uichange(QString)));
    connect(this,SIGNAL(p2pconnect(SOCKET,sockaddr_in)),this,SLOT(newliaotian(SOCKET,sockaddr_in)));
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)acceptForP2P,(LPVOID*)pa, 0, 0);
}

friendlist::~friendlist()
{
    delete this;
}

DWORD WINAPI acceptForP2P(LPARAM lparam)
{
    param *q=new param;
    q->l=((param *)lparam)->l;
    WSADATA wsaData;
    sockaddr_in srvAddr ,clientAddr;;
    SOCKET SerSocket;
//    emit q->l->valuechanged("enter");
    int nAddrLen = sizeof(sockaddr);
    if(WSAStartup(0x0101,&wsaData))
    {
    //ui->textBrowser->append("Server initialize winsock error!\n");
    emit q->l->valuechanged("创建失败");
    return -1;
    }
    if(wsaData.wVersion != 0x0101)
    {
    //ui->textBrowser->append("Server's winsock version error!\n");
    WSACleanup();
    return -1;
    }
    //ui->textBrowser->append("Server's winsock initialized !\n");

    //创建 TCP socket
    SerSocket = socket(AF_INET,SOCK_STREAM,0);
    if(SerSocket == INVALID_SOCKET)
    {
    //ui->textBrowser->append("Server create socket error!\n");
        emit q->l->valuechanged("scoket error");
    WSACleanup();
    return -1;
    }
    //ui->textBrowser->append("Server TCP socket create OK!\n");
    //绑定 socket to Server's IP and port 5050
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(5060);
    srvAddr.sin_addr.S_un.S_addr = INADDR_ANY;

    if(bind(SerSocket,(LPSOCKADDR)&srvAddr,sizeof(srvAddr))== SOCKET_ERROR)
    {
    //ui->textBrowser->append("Server socket bind error!\n");
        emit q->l->valuechanged("bind error");
    closesocket(SerSocket);
    WSACleanup();
    return -1;
    }
    //ui->textBrowser->append("Server socket bind OK!\n");

    if(listen(SerSocket,10) == SOCKET_ERROR)
    {
    //printf("Server socket listen error!\n");
    emit q->l->valuechanged("listen error");
    closesocket(SerSocket);
    WSACleanup();
    return -1;
    }

    while (1)
    {
        //新建一个socket，用于客户端
        SOCKET *sClient = new SOCKET;
        //等待客户端的连接
        *sClient= accept(SerSocket, (sockaddr*)&clientAddr, &nAddrLen);
 //       emit q->l->valuechanged("有人");
        if (INVALID_SOCKET == *sClient)
        {
            printf("socket accept failed\n");
            WSACleanup();
            closesocket(SerSocket);
            delete sClient;
            return 0;
        }
        q->s=*sClient;
        emit q->l->p2pconnect(*sClient,clientAddr);
//        emit q->l->valuechanged("有人2");
    }
}


void friendlist::uichange(QString str)
{
    ui->textBrowser->append(str);
}




void friendlist::newliaotian(SOCKET sock, sockaddr_in ipaddr)
{
    ui->textBrowser->append("123");
    liaotian *LiaoTian=new liaotian(sock,ipaddr,Username);
    LiaoTian->move(0,0);
    LiaoTian->show();

}
/*void friendlist::on_treeWidget_clicked(const QModelIndex &index)
{
    ui->textBrowser->append(this->Username);
     QTreeWidgetItem * item=ui->treeWidget->currentItem();
     QString str=item->text(0);


     char recvbuf[1024]="";
     char *ch;
     QString panduan="3";

     QByteArray ba = (panduan).toLatin1();
     ch=ba.data();
     soc->sendData(ch);
Sleep(100);//
     ba = (str).toLatin1();
     ch=ba.data();
     soc->sendData(ch);
Sleep(100);//
     recv(soc->clientSock,recvbuf,sizeof(recvbuf),0);

     str=QString(QLatin1String(recvbuf));

     ui->textBrowser->append(str);



     liaotian *LiaoTian=new liaotian(this->soc->clientSock);
     LiaoTian->show();

}
*/
void friendlist::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    //ui->textBrowser->append(this->Username);
     QString str=item->text(0);
     QString str2;

     char recvbuf[1024]="";
     char *ch;
     QString panduan="3";
     QByteArray ba = (panduan).toLatin1();
     ch=ba.data();
     soc->sendData(ch);
Sleep(100);//
     ba = (str).toLatin1();
     ch=ba.data();
     soc->sendData(ch);
Sleep(100);//
     recv(soc->clientSock,recvbuf,sizeof(recvbuf),0);
     str=QString(QLatin1String(recvbuf));
     ui->textBrowser->append(str);
Sleep(100);//
    recv(soc->clientSock,recvbuf,sizeof(recvbuf),0);
    str2=QString(QLatin1String(recvbuf));
     if(str2=="1")
     {
         mysock *p2psoc=new mysock;
         p2psoc->conToSever(5060,str);
         liaotian *LiaoTian=new liaotian(p2psoc->clientSock,p2psoc->srvAddr,Username);
         LiaoTian->show();
     }
     else
     {

         LXliaotian *LiaoTian=new LXliaotian(soc->clientSock,Username,item->text(0));
         LiaoTian->show();
     }

Sleep(100);//
}
void friendlist::closeEvent(QCloseEvent *)
{
    soc->sendData("5");
    Sleep(100);
    char  *ch;
    QByteArray ba = Username.toLatin1();
    ch=ba.data();
    send(soc->clientSock,ch,strlen(ch),0);
}
