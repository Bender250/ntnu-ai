#include <iostream>

#include "settings.h"
#include "evolution.h"

Settings *Settings::_i = 0;

int main(int argc, char *argv[])
{
    std::string config = "../ea/settings.json";
    if (argc > 1) {
        config = argv[1];
    }
    Settings::inst()->load(config);

    Evolution e;
    e.run();
    Settings::inst()->_log.close();
    return 0;
}
