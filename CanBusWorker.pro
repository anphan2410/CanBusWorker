#-------------------------------------------------
#
# Project created by QtCreator 2017-06-15T06:57:28
#
#-------------------------------------------------

QT       += core gui serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += -std=c++11 console
TARGET = CanBusWorkerExecutableFile
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


SOURCES += main.cpp\
        mainwindow.cpp \
    CanBusWorker/canbusworker.cpp \
    CanBusWorker/canbusworkerdb.cpp \
    CanBusWorker/waitforpluginandinterface.cpp \
    CanBusWorker/createdevice.cpp \
    CanBusWorker/directtransition.cpp \
    CanBusWorker/connectdevice.cpp \
    CanBusWorker/deviceconnected.cpp \
    CanBusWorker/readframe.cpp \
    CanBusWorker/writeframe.cpp \
    CanBusWorker/framesent.cpp \
    CanBusWorker/waitforerrorhandler.cpp \
    canprotocol.cpp \
    CanBusWorker/writeaframe.cpp

HEADERS  += mainwindow.h \
    CanBusWorker/canbusworker.h \
    CanBusWorker/canbusworkerdb.h \
    CanBusWorker/waitforpluginandinterface.h \
    CanBusWorker/createdevice.h \
    CanBusWorker/directtransition.h \
    CanBusWorker/connectdevice.h \
    CanBusWorker/deviceconnected.h \
    CanBusWorker/readframe.h \
    CanBusWorker/writeframe.h \
    CanBusWorker/framesent.h \
    CanBusWorker/waitforerrorhandler.h \
    anlogger.h \
    canprotocol.h \
    commonthings.h \
    CanBusWorker/writeaframe.h

FORMS    += mainwindow.ui
