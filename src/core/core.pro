TEMPLATE = lib

CONFIG += dll \
debug \
qt \
warn_on
TARGET = zmviewercore

SOURCES += stream.cpp

HEADERS += stream.h

QT += network

INSTALLS += target

