TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    population.cpp \
    evolution.cpp \
    individual.cpp

HEADERS += \
    individual.h \
    population.h \
    evolution.h \
    settings.h \
    individual.h

DISTFILES += \
    settings.json
