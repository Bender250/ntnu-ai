#include "mainwindow.h"
#include <QApplication>
#include "swarm.h"

Settings *Settings::instance = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int ret = a.exec();
    Settings::inst()->print_to_file("initial.txt");
    return ret;
}
