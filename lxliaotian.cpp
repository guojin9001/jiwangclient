#include "lxliaotian.h"
#include "ui_lxliaotian.h"
#include<QDateTime>
#include<QMessageBox>
#include<QFileDialog>
LXliaotian:: LXliaotian(SOCKET sock,QString user,QString lixianname, QWidget *parent):
    soc(sock),
    username(user),
    Lixianname(lixianname),
    QDialog(parent),
    ui(new Ui::LXliaotian)
{
    ui->setupUi(this);
    setWindowTitle(lixianname);
}
LXliaotian::~LXliaotian()
{
    delete ui;
}
void LXliaotian::on_pushButton_clicked()
{
    char recvbuf[1024]={0};
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

            QString panduan="4";
            QByteArray ba = (panduan).toLatin1();
            ch=ba.data();

            send(soc,ch,strlen(ch),0);
            ba = Lixianname.toLatin1();
            ch=ba.data();
Sleep(100);
            if(send(soc,ch,strlen(ch),0)) {
                char *ch;
                QByteArray ba = NeiRong.toLatin1();
                ch=ba.data();
Sleep(100);
                if(send(soc,ch,strlen(ch),0)){
                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("ddd yyyy.MM.dd hh:mm:ss ");
                ui->textBrowser->append(current_date+NeiRong);
                ui->textBrowser->setAlignment(Qt::AlignRight);
                }
Sleep(100);
                recv(soc,recvbuf,sizeof(recvbuf),0);
                QString str=QString(QLatin1String(recvbuf));
                if(str=="1")
                {
                   QMessageBox::warning(this,tr("waring"),("success"),QMessageBox::Yes);
                }

            }
            ui->lineEdit->setText("");
    }
}


