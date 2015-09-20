#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   // w.setStyleSheet("QMainWindow {background: 'cyan';}");
    w.show();

    return a.exec();
}
