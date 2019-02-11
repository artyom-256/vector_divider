TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++ -lstdc++

DEFINES += STATIC

SOURCES += \
        main.cpp
