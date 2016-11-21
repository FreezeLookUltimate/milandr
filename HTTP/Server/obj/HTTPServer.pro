QT += core network
QT -= gui

CONFIG += c++11

TARGET = HTTPServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += ./../src/main.cpp \
    ./../src/qsimpleserver.cpp \
    ./../src/requests.cpp

HEADERS += \
    ./../src/qsimpleserver.h \
    ./../src/requests.h
