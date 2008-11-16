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
      rest/rest.cpp
      rest/requestinterface.cpp
      rest/zoneminderhtmlrequest.cpp
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
		     )

QT4_ADD_RESOURCES ( core_SRCS resources.qrc )
QT4_WRAP_UI( core_SRCS ${core_UIS} )
QT4_AUTOMOC (${core_SRCS})

add_library (zmviewercore SHARED ${core_SRCS} )
target_link_libraries( zmviewercore ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTGUI_LIBRARY} )
install(TARGETS zmviewercore DESTINATION ${ZMVIEWER_LIB_DIR})
