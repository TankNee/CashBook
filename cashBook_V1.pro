#-------------------------------------------------
#
# Project created by QtCreator 2019-07-03T10:40:19
#
#-------------------------------------------------


QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cashBook_V1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    addnewitem.cpp \
    itemwidget.cpp \
    userloginwindow.cpp \
    itemmark.cpp

HEADERS += \
        mainwindow.h \
    addnewitem.h \
    itemwidget.h \
    userloginwindow.h \
    itemmark.h

FORMS += \
        mainwindow.ui \
    userloginwindow.ui \
    itemmark.ui

CONFIG += C++11

RESOURCES += \
    icon_asset.qrc

DISTFILES +=

QT    +=charts
