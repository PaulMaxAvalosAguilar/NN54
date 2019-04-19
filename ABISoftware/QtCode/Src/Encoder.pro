#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T20:09:14
#
#-------------------------------------------------

QT       += core gui serialport bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CFLAGS += -fno-keep-inline-dllexport -std=c++11

TARGET = Encoder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
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
    Connection/Bluetooth/Classic/bluetoothconnection.cpp \
    Connection/Serial/serialconnection.cpp \
    Connection/connection.cpp \
    scanner.cpp \
    Connection/Bluetooth/Classic/device.cpp \
    Connection/Bluetooth/Classic/devices.cpp



HEADERS += \
        mainwindow.h \
    Connection/Bluetooth/Classic/bluetoothconnection.h \
    Connection/Serial/serialconnection.h \
    Connection/connection.h \
    scanner.h \
    Connection/Bluetooth/Classic/device.h \
    Connection/Bluetooth/Classic/devices.h



FORMS += \
        mainwindow.ui \
    scanner.ui

