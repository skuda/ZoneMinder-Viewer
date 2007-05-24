CONFIG += warn_on \
	  thread \
          qt \
	  debug
TARGET = zmviewer

SOURCES += main.cpp \
mainwindow.cpp \
cameraadddialog.cpp \
fullscreencamera.cpp \
multicameraview.cpp \
cameraselectdialog.cpp \
camerawidgettoolbar.cpp \
camerawidget.cpp \
about.cpp
HEADERS += mainwindow.h \
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

INCLUDEPATH += ../core

INSTALLS += target

LIBS += -L../core \
-lzmviewercore
TARGETDEPS += ../core/libzmviewercore.so

