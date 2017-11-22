#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<mysock.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    friend class liaotian;
protected:
    mysock *soc;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
private:
    int conNectOk;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
