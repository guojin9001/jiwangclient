#include "zhuce.h"
#include "ui_zhuce.h"
#include<QMessageBox>

zhuce::zhuce(QWidget *parent) :
    QDialog(parent),

    ui(new Ui::zhuce)
{
    ui->setupUi(this);
    soc=new mysock;
    soc->conToSever(5050,"222.20.104.92");
}

zhuce::~zhuce()
{
    delete ui;
}


void zhuce::on_pushButton_clicked()
{
    QString str;
    QString account=ui->lineEdit->text();
    QString password1=ui->lineEdit_2->text();
    QString password2=ui->lineEdit_3->text();
    QString telephone=ui->lineEdit_4->text();
    if(password1==""||password2==""||account=="")
    {
       QMessageBox::warning(this,tr("waring"),tr("重新输入"),QMessageBox::Yes);
    }
    if(password1!=password2)
    {
       QMessageBox::warning(this,tr("waring"),tr("重新输入"),QMessageBox::Yes);
    }
    else
    {
         {
            char recvbuf[20]="";
            char *ch;
            QString panduan="1";

            QByteArray ba = (panduan).toLatin1();
            ch=ba.data();
            soc->sendData(ch);
Sleep(100);//
            ba = (account).toLatin1();
            ch=ba.data();
            soc->sendData(ch);
Sleep(100);//
            ba = (password1).toLatin1();
            ch=ba.data();
            soc->sendData(ch);

Sleep(100);//
            ba = (telephone).toLatin1();
            ch=ba.data();
            soc->sendData(ch);

            recv(soc->clientSock,recvbuf,20,0);
            str=QString(QLatin1String(recvbuf));

            if(str=="1") {
                QMessageBox::warning(this,tr("waring"),tr("create account success"),QMessageBox::Yes);
                delete this;
            }
            else if(str=="-1")
            {
                   QMessageBox::warning(this,tr("waring"),tr("the account has been used"),QMessageBox::Yes);
            }
        }
    }
}

void zhuce::on_pushButton_2_clicked()
{
    delete this;
}
