#ifndef FRIENDLIST_H
#define FRIENDLIST_H
#include <QDialog>
#include<mysock.h>
#include <QMetaType>
#include<liaotian.h>
#include<lxliaotian.h>
#include<QTreeWidget>
static DWORD WINAPI acceptForP2P(LPARAM lparam);
namespace Ui {
class friendlist;
}
class friendlist : public QDialog
{
    Q_OBJECT

public:
    explicit friendlist(QString str1, QString str3, mysock* sock, QString str2, QWidget *parent = 0);
    ~friendlist();
    mysock *soc;
    QString Username;
    QString Friendname;
    QString lixianXX;
    void friendlist::closeEvent(QCloseEvent *);
signals:
    void valuechanged(QString str);
signals:
    void p2pconnect(SOCKET sock,sockaddr_in ipaddr);
private slots:
    void uichange(QString str);
    void newliaotian(SOCKET sock,sockaddr_in ipaddr);
    //void on_treeWidget_clicked(const QModelIndex &index);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::friendlist *ui;
};
struct param
{
    friendlist *l;
    SOCKET s;
};
Q_DECLARE_METATYPE(SOCKET)

#endif // FRIENDLIST_H
