#-------------------------------------------------
#
# Project created by QtCreator 2017-10-06T11:33:59
#
#-------------------------------------------------

QT       += core gui serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CanBusWorker
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
    anLogger/src/anlogger.cpp \
    CanProtocol/src/canprotocol.cpp \
    shared/commonthings.cpp \
    shared/directtransition.cpp \
    src/canbusworker.cpp \
    src/canbusworkerbasis.cpp \
    shared/abstractstatemachinebasis.cpp \
    src/idlecanbusworker.cpp \
    src/runningcanbusworker.cpp \
    src/uninitiatedcanbusworker.cpp \
    src/frameiswritten.cpp \
    src/errorcanbusworker.cpp \
    src/writeframecanbusworker.cpp

HEADERS += \
        mainwindow.h \
    anLogger/src/anlogger.h \
    CanProtocol/src/canprotocol.h \
    shared/commonthings.h \
    shared/directtransition.h \
    src/canbusworker.h \
    src/canbusworkerbasis.h \
    shared/abstractstatemachinebasis.h \
    src/idlecanbusworker.h \
    src/runningcanbusworker.h \
    src/uninitiatedcanbusworker.h \
    src/frameiswritten.h \
    src/errorcanbusworker.h \
    src/writeframecanbusworker.h

FORMS += \
        mainwindow.ui
