TEMPLATE = lib

CONFIG += dll \
debug \
qt \
warn_on
TARGET = zmviewercore

SOURCES += stream.cpp \
monitors.cpp \
database.cpp
HEADERS += stream.h \
monitors.h \
database.h
QT += network

INSTALLS += target

