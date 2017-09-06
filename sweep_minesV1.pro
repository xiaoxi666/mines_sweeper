#-------------------------------------------------
#
# Project created by QtCreator 2017-09-03T23:52:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sweep_minesV1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configdialog.cpp \
    fielddata.cpp \
    minesweepscene.cpp \
    cellitem.cpp

HEADERS  += mainwindow.h \
    configdialog.h \
    fielddata.h \
    minesweepscene.h \
    cellitem.h

FORMS    += mainwindow.ui \
    configdialog.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    res.qrc
