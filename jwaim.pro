QT += core gui x11extras
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = jwaim
SOURCES += \
    main.cpp \
    window.cpp \
    hack.cpp \
    remote.cpp \
    helper.cpp \
    monitor.cpp

HEADERS += \
    window.h \
    hack.h \
    remote.h \
    types.h \
    offsets.h \
    helper.h \
    settings.h \
    monitor.h

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

system(cp -n settings_example.cfg settings.cfg)
