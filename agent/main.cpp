#include "mainwindow.h"
#include <QApplication>

#include "settings.h"
#include "evolution.h"

Settings *Settings::_i = 0;

int main(int argc, char *argv[])
{
    std::string config = "../agent/settings.json";
    if (argc > 1) {
        config = argv[1];
    }
    Settings::inst()->load(config);

    Evolution e;
    e.run();

    std::unique_ptr<Individual> i = e.getLeadingIndividual();

    Settings::inst()->_log.close();

    QApplication a(argc, argv);
    MainWindow w;
    w.initiate(i);
    w.show();

    return a.exec();
}
