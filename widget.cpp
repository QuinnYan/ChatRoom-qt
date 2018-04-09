#include "widget.h"
#include "ui_widget.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QProcess>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    p_login = new Login;
    p_login->setWindowTitle("Login");
    p_login->show();
    loginFlag = 0;

    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    port = 45454;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);//?
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processDatagrams()));
    connect(p_login,SIGNAL(showmain()),this,SLOT(showWindow()));


}
Widget::~Widget()
{
    delete ui;
}

void Widget::showWindow()
{
    this->show();
    sendMessage(newlogin);

    ui->messageTextEdit->setFocus();

}

QString Widget::getUserName()
{
    return p_login->userName;
}

QString Widget::getMessage()
{
    QString mes = ui->messageTextEdit->toPlainText();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return mes;
}


void Widget::sendMessage(MessageType type, QString serverAddress)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);//QDataStream读写二进制文件需要QIODevice
    QString address;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress add, list) {
        if(add.protocol() == QAbstractSocket::IPv4Protocol)
            address = add.toString();

    }

    out << type << getUserName();

    switch(type)
    {
    case newlogin:
        out << address;
        break;
    case logout:
        break;
    case message:
        if(ui->messageTextEdit->toPlainText()=="")
        {
            QMessageBox::warning(this,"Warning","消息不能为空","确定");
            return;
        }
        out << address << getMessage();
        ui->messageBrowser->verticalScrollBar()->setValue(ui->messageBrowser->verticalScrollBar()->maximum());

        break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);

}

//接受处理广播数据
void Widget::processDatagrams()
{

    while(udpSocket->hasPendingDatagrams())
    {
        qDebug() << "send 广播信息";
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);

        int messageType;
        in >> messageType;
        QString userName,IPAddress,_message;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString space = " ";
        switch (messageType)
        {
        case message:
            in >> userName >> IPAddress >> _message;

            if(_message[0] == '@')
            {

                if(_message.mid(1,_message.indexOf(space) - 1) == getUserName())
                {
                    //你是被@的对象
                    ui->messageBrowser->append("<font color=red> 有人@你 ["+userName+"]"+time+"</font>");
                    //QMessageBox::information(this,"提示","收到小伙伴@你的消息，快查看吧","确认","取消");
                }
                else
                    ui->messageBrowser->append("<font color=blue>["+userName+"]"+time+"</font>");
            }
            else if(userName == getUserName())
            {
                ui->messageBrowser->append("<font color=green>["+userName+"]"+time+"</font>");
            }
            else
            {
                ui->messageBrowser->append("<font color=blue>["+userName+"]"+time+"</font>");
            }
            ui->messageBrowser->append(_message);
            break;

        case newlogin:
            in >> userName >> IPAddress;
            qDebug() << userName <<IPAddress;
            newLogIn(userName,IPAddress);
            break;

        case logout:
            in >> userName ;
            logOut(userName,IPAddress);
            break;
        default:
            break;
        }
    }
}


void Widget::newLogIn(QString userName, QString IPAddress)
{
    if(ui->userTableWidget->findItems(userName,Qt::MatchExactly).isEmpty()
            || ui->userTableWidget->findItems(IPAddress,Qt::MatchExactly).isEmpty())
    {
        QTableWidgetItem *user = new QTableWidgetItem(userName);
        QTableWidgetItem *ip = new QTableWidgetItem(IPAddress);
        int rowCount = ui->userTableWidget->rowCount();

        ui->userTableWidget->insertRow(rowCount);
        ui->userTableWidget->setItem(rowCount,0,user);
        ui->userTableWidget->setItem(rowCount,1,ip);

        ui->messageBrowser->append(tr("%1上线").arg(userName));
        ui->numLabel->setText(QString::number(rowCount+1));

        sendMessage(newlogin);
        loginFlag++;
    }
    else if (loginFlag == 1) {

    }
    else
    {
        QMessageBox mess;
        mess.setFont(QFont("Sans Serif", 12, 50));
        mess.warning(this, "warning","invalide input");
        this->hide();
    }
}


void Widget::logOut(QString userName, QString IPAddress)
{
    int num = ui->userTableWidget->findItems(IPAddress,Qt::MatchExactly).at(1)->row();
    ui->userTableWidget->removeRow(num);

    ui->messageBrowser->append(tr("%1下线").arg(userName));

    int rowCount = ui->userTableWidget->rowCount();
    ui->numLabel->setText(QString::number(rowCount));
}


void Widget::on_sendButton_clicked()
{
    sendMessage(message);
}

void Widget::on_closeButton_clicked()
{
    sendMessage(logout);
    close();
}
void Widget::closeEvent(QCloseEvent * e)
{
    sendMessage(logout);
    QWidget::closeEvent(e);
}
