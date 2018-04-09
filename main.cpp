#include "widget.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("ChatRoom");
    //Login log;
    //log.show();
    //w.show();
    return a.exec();
}
