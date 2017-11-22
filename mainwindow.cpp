#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include"zhuce.h"
#include<zhaohui.h>
#include<mysock.h>
#include<friendlist.h>
#include<Windows.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    soc=new mysock;
    conNectOk=soc->conToSever(5050,"222.20.104.92");
    ui->lineEdit->setPlaceholderText("account");
    ui->lineEdit_2->setPlaceholderText("password");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(conNectOk!=1)
    {   delete soc;
        soc=new mysock;
        if((conNectOk=soc->conToSever(5050,"222.20.104.92"))!=1)
            QMessageBox::warning(this,tr("waring"),("connect to server error"),QMessageBox::Yes);
    }
    QString CONOK="connect ok";
    QString account=ui->lineEdit->text();
    QString UserName=ui->lineEdit_2->text();
    QString str,str1;
    if(account==""||UserName=="")
    {
       QMessageBox::warning(this,tr("waring"),("输入用户名和密码"),QMessageBox::Yes);
    }
    else
    {
         {
            char recvbuf[1024]="";
            char *ch;
            QString panduan="0";

           QByteArray ba = (panduan).toLatin1();
            ch=ba.data();
            soc->sendData(ch);
Sleep(100);//
            ba = (account).toLatin1();
            ch=ba.data();
            soc->sendData(ch);
Sleep(100);//
            ba = (UserName).toLatin1();
            ch=ba.data();
            soc->sendData(ch);
            recv(soc->clientSock,recvbuf,sizeof(recvbuf),0);
            str=QString(QLatin1String(recvbuf));
            ui->lineEdit_2->setText(str);
            if(str=="1") {
                recv(soc->clientSock,recvbuf,sizeof(recvbuf),0);
                str=QString(QLatin1String(recvbuf));
                ui->lineEdit_2->setText(str);

                recv(soc->clientSock,recvbuf,sizeof(recvbuf),0);
                str1=QString(QLatin1String(recvbuf));
                ui->lineEdit_2->setText(str1);
                friendlist *FriendFist=new friendlist(str,str1,soc,account);
                this->hide();
                FriendFist->show();
            }
            else
            {
               QMessageBox::warning(this,tr("waring"),("password wrong or account doesn't exist"),QMessageBox::Yes);
            }
        }
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    zhuce * z=new zhuce();
    this->hide();
    z->show();
    z->exec();
    this->show();

}

void MainWindow::on_pushButton_3_clicked()
{
    zhaohui *z=new zhaohui();
    this->hide();
    z->exec();
    this->show();
}
