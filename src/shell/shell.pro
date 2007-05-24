CONFIG += warn_on \
	  thread \
          qt
TARGET = zmviewer

SOURCES += main.cpp \
stream.cpp \
mainwindow.cpp \
cameraadddialog.cpp \
fullscreencamera.cpp \
multicameraview.cpp \
cameraselectdialog.cpp \
camerawidgettoolbar.cpp \
camerawidget.cpp \
about.cpp
HEADERS += stream.h \
mainwindow.h \
cameraadddialog.h \
fullscreencamera.h \
multicameraview.h \
cameraselectdialog.h \
camerawidgettoolbar.h \
camerawidget.h \
about.h \
config.h
QT += core \
network
RESOURCES += resources.qrc

FORMS += ui/cameraadddialog.ui
TEMPLATE = app

