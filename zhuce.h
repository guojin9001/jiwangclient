#ifndef ZHUCE_H
#define ZHUCE_H

#include <QDialog>
#include<mysock.h>
namespace Ui {
class zhuce;
}

class zhuce : public QDialog
{
    Q_OBJECT

public:
    mysock *soc;
    explicit zhuce(QWidget *parent = 0);
    ~zhuce();    

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::zhuce *ui;
};

#endif // ZHUCE_H
