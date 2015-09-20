#-------------------------------------------------
#
# Project created by QtCreator 2015-07-05T13:14:30
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rainMaking
TEMPLATE = app
target.path = /home/pi
INSTALLS += target

SOURCES += main.cpp\
        mainwindow.cpp \
    abstractmeter.cpp \
    functions.cpp \
    manometer.cpp \
    widgetwithbackground.cpp

HEADERS  += mainwindow.h \
    abstractmeter.h \
    functions.h \
    manometer.h \
    widgetwithbackground.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
