#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    connect(ui->loginButton,SIGNAL(clicked()),this,SLOT(on_loginButton_clicked));
}

Login::~Login()
{
    delete ui;
}


void Login::on_loginButton_clicked()
{
    if(ui->userName->text()=="")
    {
        QMessageBox::warning(this,"Warning","请输入用户名","确定");
        return;
    }
    userName = ui->userName->text();
    this->hide();
    emit showmain();
}
