#ifndef ZHAOHUI_H
#define ZHAOHUI_H
#include"mysock.h"
#include <QDialog>

namespace Ui {
class zhaohui;
}

class zhaohui : public QDialog
{
    Q_OBJECT

public:
    mysock * soc;
    explicit zhaohui(QWidget *parent = 0);
    ~zhaohui();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::zhaohui *ui;
};

#endif // ZHAOHUI_H
