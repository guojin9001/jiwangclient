#include "zhaohui.h"
#include "ui_zhaohui.h"
#include<QMessageBox>
zhaohui::zhaohui(QWidget *parent) :
    QDialog(parent),

    ui(new Ui::zhaohui)
{
    ui->setupUi(this);
    soc=new mysock;
    soc->conToSever(5050,"222.20.104.92");
}

zhaohui::~zhaohui()
{
    delete ui;
    soc->~mysock();
}

void zhaohui::on_pushButton_clicked()
{
    QString str;
    QString account=ui->lineEdit->text();
    QString telenum=ui->lineEdit_2->text();
    if(account==""||telenum=="")
    {
       QMessageBox::warning(this,tr("waring"),("输入用户名和电话"),QMessageBox::Yes);
    }
    else
    {
            char recvbuf[20]="";
            char *ch;
            QString panduan="2";
            QByteArray ba = (panduan).toLatin1();
            ch=ba.data();
            soc->sendData(ch);
Sleep(1000);//
            ba = (account).toLatin1();
            ch=ba.data();
            soc->sendData(ch);
Sleep(1000);//
            ba = (telenum).toLatin1();
            ch=ba.data();
            soc->sendData(ch);


            recv(soc->clientSock,recvbuf,20,0);
            str=QString(QLatin1String(recvbuf));
            ui->lineEdit->setText(str);
            if(str!="-1") {
                ui->lineEdit_3->setText(str);
                QMessageBox::warning(this,tr("waring"),("找回成功"),QMessageBox::Yes);
            }
            else {
                QMessageBox::warning(this,tr("waring"),("找回失败"),QMessageBox::Yes);
                delete this;
            }
        }
}
void zhaohui::on_pushButton_2_clicked()
{
    delete this;
}
