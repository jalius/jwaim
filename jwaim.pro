QT += core gui
<<<<<<< HEAD
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

=======
greaterThan(QT_MAJOR_VERSION, 5): QT += widgets
CONFIG += c++11
>>>>>>> 5888b19d35369a8f86122ee4312c3cce83f5b550
TARGET = jwaim
SOURCES += \
    main.cpp \
    window.cpp \
    hack.cpp \
    remote.cpp \
    helper.cpp

HEADERS += \
    window.h \
    hack.h \
    remote.h \
    types.h \
    offsets.h \
    helper.h \
    settings.h

INCLUDEPATH += /usr/include/boost

LIBS += \
    -lX11 \
    -lconfig++ \
    -lXtst \
    -lpthread \
    -lboost_system \
    -lboost_thread

DISTFILES += \
    settings.cfg


