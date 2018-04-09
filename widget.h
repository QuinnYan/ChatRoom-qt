#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include "login.h"

class QUdpSocket;


namespace Ui {
class Widget;
}

enum MessageType{//登入/登出/信息
    newlogin,
    logout,
    message
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    Login *p_login;


protected:
    void newLogIn(QString userName, QString IPAddress);
    void logOut(QString userName, QString IPAddress);
    void sendMessage(MessageType type, QString serverAddress="");
    void closeEvent(QCloseEvent *);

    QString getMessage();
    QString getUserName();


private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    int loginFlag;
    //Widget *p_login;


private slots:
    void processDatagrams();
    void on_sendButton_clicked();
    void on_closeButton_clicked();
    void showWindow();
};

#endif // WIDGET_H
