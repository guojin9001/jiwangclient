#include "liaotian.h"
#include "ui_liaotian.h"
#include<mysock.h>
#include<QDateTime>
#include<QMessageBox>
#include<QFileDialog>
#include<stdio.h>
QString filpath;
QString filname;
typedef struct {
    int id;
    int buf_size;
    int end;
}PackInfo;
struct SendPack
{
    PackInfo head;
    char buf[1024];
}sendata;
struct RecvPack
{
  PackInfo head;
  char buf[1024];
}recdata;
liaotian::liaotian(SOCKET sock, sockaddr_in ipadd, QString user, QWidget *parent) :
    soc(sock),
    ipaddr(ipadd),
    username(user),
    QDialog(parent),
    ui(new Ui::liaotian)
{
    ui->setupUi(this);
    connect(this,SIGNAL(valuechanged(QString)),this,SLOT(uichange(QString)));
    connect(this,SIGNAL(filerecvok()),this,SLOT(filerecv()));
    connect(this,SIGNAL(filesendok()),this,SLOT(filesend()));
    par *pa=new par;
    pa->l=this;
    pa->s=sock;
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recvda,(LPVOID*)pa, 0, 0);    
}
liaotian::~liaotian()
{
    delete ui;
}
void liaotian::uichange(QString str)
{
    ui->textBrowser->append(str);
    ui->textBrowser->setAlignment(Qt::AlignLeft);
}
void liaotian::on_pushButton_clicked()
{
    QString CONOK="connect ok";
    QString NeiRong=ui->lineEdit->text();
    if(NeiRong=="")
    {
       QMessageBox::warning(this,tr("waring"),("输入消息"),QMessageBox::Yes);
    }
    else
    {
            NeiRong.prepend(":");
            NeiRong.prepend(username);
            char *ch;
            QByteArray ba = NeiRong.toLatin1();
            ch=ba.data();
            if(send(soc,ch,strlen(ch),0)) {
                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("ddd yyyy.MM.dd hh:mm:ss.zzz ");                
                ui->textBrowser->append(current_date+NeiRong);
                ui->textBrowser->setAlignment(Qt::AlignRight);
            }
            ui->lineEdit->setText("");
    }
}

void liaotian::on_pushButton_2_clicked()
{
   QFileDialog *fileDia=new QFileDialog(this);
   fileDia->setWindowTitle("选择文件");
   fileDia->setFileMode(QFileDialog::AnyFile);
   fileDia->setViewMode(QFileDialog::Detail);
   fileDia->show();
   if(fileDia->exec()==QFileDialog::Accepted)
   {
       filname=fileDia->selectedFiles()[0];
       QFileInfo file_info=QFileInfo(filname);
       filpath=file_info.fileName();
       ui->textBrowser->setText(filpath);
       QString chuan="#file#";
       char *ch;
       QByteArray ba = chuan.toLatin1();
       ch=ba.data();
       if(send(soc,ch,strlen(ch),0))
       {
           ui->textBrowser->append("waiting for acknowledgement");              
       }
   }
}
DWORD WINAPI recvda(LPARAM lparam)
{
    par *q=new par;
    q->l=((par *)lparam)->l;
    q->s=((par *)lparam)->s;
    SOCKET sockClient=q->s;
    char buf[512];
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        // 接收客户端的一条数据
        int ret = recv(sockClient, buf, sizeof(buf), 0);
        //检查是否接收失败
        if (SOCKET_ERROR == ret)
        {
            qDebug("socket recv failed\n");
            closesocket(sockClient);
            return -1;
        }
        QString str= QString(QLatin1String(buf));
        if(str=="#file#"){
           emit q->l->filerecvok();
           recv(sockClient,buf,sizeof(buf),0);
           filpath=QString(QLatin1String(buf));
        }
        else if(str=="#fileok#")
        {
            char *ch;
            QByteArray ba = filpath.toLatin1();
            ch=ba.data();
            send(sockClient,ch,strlen(ch),0);
            Sleep(1000);
            emit q->l->filesendok(); 
        }
        else{
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("ddd yyyy.MM.dd hh:mm:ss.zzz ");
            emit q->l->valuechanged(current_date+str);
        }

        // 0 代表客户端主动断开连接
        if (ret == 0)
        {
            qDebug("client close connection\n");
            closesocket(sockClient);
            return -1;
        }
    }
    closesocket(sockClient);
    return 0;
}
void liaotian::filerecv()
{
    char buf[30];
    //QMessageBox::warning(this,tr("waring"),("get a file"),QMessageBox::Yes);
    QString chuan="#fileok#";
    char *ch;
    QByteArray ba = chuan.toLatin1();
    ch=ba.data();
    send(soc,ch,strlen(ch),0);

    ui->textBrowser->append(filpath);
    HANDLE Thead= CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Frecv,(LPVOID*)&ipaddr, 0, 0);//创建接收文件线程
    CloseHandle(Thead);
    ui->textBrowser->append("start");
}
void liaotian::filesend()
{
    //创建发送文件线程
    HANDLE Thead=CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Fsend,(LPVOID*)&ipaddr, 0, 0);
    CloseHandle(Thead);
    ui->textBrowser->append("start");
   // QMessageBox::warning(this,tr("waring"),("start"),QMessageBox::Yes);
}
DWORD WINAPI Frecv(LPARAM lparam)
{
    int id = 1;
    char buffer[1024];
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(1, 1);
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        qDebug("WSAStartup failed with error: %d\n", err);
        return 1;
    }
    sockaddr_in ser_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(server_port);
    int ser_addr_len = sizeof(ser_addr);
    memset(ser_addr.sin_zero, 0, 8);
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1)
    {
        qDebug("create socket failed");
        return -1;
    }
    else qDebug("create socket success");//建立套接字

    if (bind(server_socket, (sockaddr *)&ser_addr, sizeof(ser_addr)) == -1)
    {
        qDebug("bind socket failed");
        return -1;
    }
    else qDebug("bind socket success");//绑定套接字
    filpath="D:\\"+filpath+"\0";
    char *fil;
    QByteArray ba = filpath.toLatin1();
    fil=ba.data();
    FILE *file = fopen(fil, "wb+");
    if (file == NULL)
    {
        qDebug("open file error");
        return -1;
    }
    else
    {
        int len = 0;
        PackInfo pack_info;
        pack_info.id = 0;
        pack_info.buf_size = 0;
        while (1)
        {
            if ((len = recvfrom(server_socket, (char*)&recdata, sizeof(recdata), 0, (sockaddr*)&ser_addr, &ser_addr_len))>0)
            {
                qDebug("recv some file");
                if (recdata.head.id == id)
                {
                    id++;
                    pack_info.id = recdata.head.id;
                    pack_info.buf_size = recdata.head.buf_size;
                    if (sendto(server_socket, (char*)&pack_info, sizeof(pack_info), 0, (sockaddr*)&ser_addr, ser_addr_len)<0)
                    {
                        qDebug("send confirm failed");
                    }
                    if (fwrite(recdata.buf, sizeof(char), recdata.head.buf_size, file)<0)
                    {
                        qDebug("file write failed");
                        break;
                    }
                    if (recdata.head.end == 1)break;
                }
                else
                {
                    if (sendto(server_socket, (char*)&pack_info, sizeof(pack_info), 0, (sockaddr*)&ser_addr, ser_addr_len)<0)
                    {
                        qDebug("send confirm failed");
                    }
                }
            }
        }
        qDebug("recv file success");
        fclose(file);
    }
    closesocket(server_socket);
    return 0;
}
DWORD WINAPI Fsend(LPARAM lparam)
{
    int recv_id = 0;
    int send_id = 0;
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(1, 1);
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        qDebug("WSAStartup failed with error: %d\n", err);
        return 1;
    }
    sockaddr_in temp=*(sockaddr_in *)lparam;
    char* ch=inet_ntoa(temp.sin_addr);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr(ch);//inet_addr("222.20.104.92");
    server_addr.sin_port = htons(server_port);
    int server_addr_length = sizeof(server_addr);
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket<0)
    {
        qDebug("create socket error");
    }
    else qDebug("create socket success");
    char *fil;
    QByteArray ba = filname.toLatin1();
    fil=ba.data();
    FILE *file = fopen(fil, "rb+");
    if (file == NULL)
    {
        qDebug("open file error");
    }
    else
    {
        int len;
        PackInfo pack_info;
        pack_info.buf_size = 0;
        pack_info.id = 0;
        pack_info.end = 0;
        while (1) {
            if (recv_id == send_id)
            {
                ++send_id;
                if ((len = fread(sendata.buf, sizeof(char), 1024, file))>0)
                {
                    if (feof(file)) sendata.head.end = 1;
                    else sendata.head.end = 0;
                    sendata.head.id = send_id;
                    sendata.head.buf_size = len;
                    if (sendto(client_socket, (char*)&sendata, sizeof(sendata), 0, (sockaddr *)&server_addr, server_addr_length)<0)
                    {
                        qDebug("send error");
                    }
                    int timeout = 1000;
                    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
                    if (recvfrom(client_socket, (char*)&pack_info, sizeof(pack_info), 0, (sockaddr*)&server_addr, &server_addr_length) == -1) continue;
                    recv_id = pack_info.id;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (sendto(client_socket, (char*)&sendata, sizeof(sendata), 0, (sockaddr *)&server_addr, server_addr_length)<0)
                {
                    qDebug("send error");
                }
                int timeout = 1000;
                setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
                if (recvfrom(client_socket, (char*)&pack_info, sizeof(pack_info), 0, (sockaddr*)&server_addr, &server_addr_length) == -1) continue;
                recv_id = pack_info.id;
            }
        }
        fclose(file);
        qDebug("success");
    }
    closesocket(client_socket);
    return 0;
}
