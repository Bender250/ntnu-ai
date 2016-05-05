#include "mainwindow.h"
#include <QApplication>

#include <iostream>

#include "settings.h"
#include "evolution.h"
#include "mainwindow.h"

Settings *Settings::_i = 0;

int main(int argc, char *argv[])
{
    std::string config = "../moea/settings.json";
    if (argc > 1) {
        config = argv[1];
    }
    Settings::inst()->load(config);

    Evolution e;
    e.run();
    Settings::inst()->_log.close();

    QApplication a(argc, argv);
    MainWindow w(e);
    w.show();

    return a.exec();
}
