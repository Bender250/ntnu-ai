#include <iostream>

#include "settings.h"
#include "evolution.h"

Settings *Settings::_i = 0;

int main(int argc, char *argv[])
{
    std::string config = "config.js";
    if (argc > 1) {
        config = argv[1];
    }
    Settings::inst()->load(config);

    Evolution e;
    e.run();
    return 0;
}
