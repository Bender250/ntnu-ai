QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wshadow

SOURCES += main.cpp \
    population.cpp \
    evolution.cpp \
    individual.cpp \
    ../qcustomplot/qcustomplot.cpp \
    mainwindow.cpp

HEADERS += \
    individual.h \
    population.h \
    evolution.h \
    settings.h \
    individual.h \
    ../qcustomplot/qcustomplot.h \
    mainwindow.h

FORMS    += mainwindow.ui

DISTFILES += \
    settings.json
