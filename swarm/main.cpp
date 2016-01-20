#include "mainwindow.h"
#include <QApplication>
#include "swarm.h"

Settings *Settings::instance = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
