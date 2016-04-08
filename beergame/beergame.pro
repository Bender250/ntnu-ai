#-------------------------------------------------
#
# Project created by QtCreator 2016-04-08T17:30:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = beergame
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp\
        evolution.cpp\
        population.cpp \
        individual.cpp \
        neuron.cpp \
        visualiser.cpp

HEADERS += mainwindow.h \
        settings.h\
        evolution.h\
        population.h \
        individual.h \
        neuron.h \
        visualiser.h \
        land.h

FORMS    += mainwindow.ui

DISTFILES += \
        settings.json
