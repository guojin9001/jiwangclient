#ifndef LIAOTIAN_H
#define LIAOTIAN_H
#include <QDialog>
#include<mysock.h>
#include<Windows.h>
#define server_port 6666
static DWORD WINAPI recvda(LPARAM lparam);
static DWORD WINAPI Frecv(LPARAM lparam);
static DWORD WINAPI Fsend(LPARAM lparam);
//static DWORD WINAPI acceptForP2P(LPARAM lparam);
namespace Ui {
class liaotian;
}
class liaotian : public QDialog
{
    Q_OBJECT
public:
    explicit liaotian(SOCKET sock, sockaddr_in ipadd, QString user, QWidget *parent = 0);
    ~liaotian();
sockaddr_in ipaddr;
QString username;
private slots:
    void uichange(QString str);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void filerecv();
    void filesend();
signals:
    void valuechanged(QString str);
    void filerecvok();
    void filesendok();
private:
    Ui::liaotian *ui;
    SOCKET soc;
};
struct par
{
    liaotian *l;
    SOCKET s;
};

#endif // LIAOTIAN_H
