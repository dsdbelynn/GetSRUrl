#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QTime>
void delayMs(int ms)
{
    QTime t = QTime::currentTime().addMSecs(ms);
    while(t > QTime::currentTime())
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
