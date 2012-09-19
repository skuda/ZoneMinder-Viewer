set ( core_SRCS
      stream.cpp
      camerawidgettoolbar.cpp
      cameraadddialog.cpp
      camerawidget.cpp
      framewidget.cpp
      connectionmanager.cpp
      fullscreencamera.cpp
      databasesession.cpp
      monitors.cpp
      connectionwidget.cpp
      cameraevents.cpp
      auth.cpp
      authdialog.cpp
      qmultipartreader.cpp
      cameralayout.cpp
      cameraviewerpage.cpp
      maincamerawidget.cpp
      doublecamerawidget.cpp
      blackwidget.cpp
      commandlistener.cpp
      models/basemodel.cpp
      models/eventmodel.cpp
      abstractplayer.cpp
      vlcplayer.cpp
      mjpegplayer.cpp
    )

set ( core_MOC_HDRS
      stream.h
      camerawidgettoolbar.h
      camerawidget.h
      framewidget.h
      fullscreencamera.h
      databasesession.h
      monitors.h
      connectionwidget.h
      cameraevents.h
      auth.h
      authdialog.h
      qmultipartreader.h
      cameralayout.h
      cameraviewerpage.h
      maincamerawidget.h
      doublecamerawidget.h
      blackwidget.h
      commandlistener.h
      models/basemodel.h
      models/eventmodel.h
      abstractplayer.h
      vlcplayer.h
      mjpegplayer.h
    )

set ( core_UIS
      ui/cameraadddialog.ui
      ui/authdialogwidget.ui
    )


include_directories ( ${CMAKE_CURRENT_BINARY_DIR}
                      ${CMAKE_CURRENT_SOURCE_DIR} 
                      ${QT_QTGUI_INCLUDE_DIR}
                      ${QT_QTNETWORK_INCLUDE_DIR}
                      ${QT_QTSQL_INCLUDE_DIR}
                      ${LIBVLC_INCLUDE_DIR}
		     )

QT4_ADD_RESOURCES ( core_SRCS resources.qrc )
QT4_WRAP_UI( core_SRCS ${core_UIS} )
QT4_WRAP_CPP( core_MOC_SRCS ${core_MOC_HDRS} )

add_library (zmviewercore SHARED ${core_SRCS} ${core_MOC_SRCS})
target_link_libraries( zmviewercore
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTNETWORK_LIBRARY}
                      ${QT_QTSQL_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${LIBVLC_LIBRARY} )
install(TARGETS zmviewercore DESTINATION ${ZMVIEWER_LIB_DIR})
