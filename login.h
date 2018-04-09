#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
//#include <widget.h>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    QString userName;
    ~Login();

private slots:
    void on_loginButton_clicked();

signals:
    void showmain();

private:
    Ui::Login *ui;

};

#endif // LOGIN_H
