#ifndef LXLIAOTIAN_H
#define LXLIAOTIAN_H
#include <QDialog>
#include<mysock.h>
#include<Windows.h>
namespace Ui {
class LXliaotian;
}

class LXliaotian : public QDialog
{
    Q_OBJECT

public:
    explicit LXliaotian(SOCKET sock,QString user,QString lixianname, QWidget *parent = 0);
    ~LXliaotian();
QString username;
QString Lixianname;
private slots:
    void on_pushButton_clicked();
private:
    Ui::LXliaotian *ui;
    SOCKET soc;
};
#endif // LXLIAOTIAN_H
