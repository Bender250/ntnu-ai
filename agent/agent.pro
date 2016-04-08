#-------------------------------------------------
#
# Project created by QtCreator 2016-03-30T11:28:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = agent
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wshadow

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
    flatland.h \
    visualiser.h

FORMS    += mainwindow.ui

DISTFILES += \
        settings.json
