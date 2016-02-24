TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    population.cpp \
    evolution.cpp \
    ind/one_max_individual.cpp \
    ind/lolz.cpp \
    ind/sequences.cpp

HEADERS += \
    individual.h \
    population.h \
    evolution.h \
    settings.h \
    ind/one_max_individual.h \
    ind/lolz.h \
    ind/sequences.h

DISTFILES += \
    settings.json
