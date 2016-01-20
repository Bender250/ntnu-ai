#-------------------------------------------------
#
# Project created by QtCreator 2016-01-20T12:43:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = swarm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    boid.cpp \
    swarm.cpp

HEADERS  += mainwindow.h \
    boid.h \
    swarm.h \
    settings.h

FORMS    += mainwindow.ui
