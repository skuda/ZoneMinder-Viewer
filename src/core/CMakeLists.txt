set ( core_SRCS
      stream.cpp
      connectionmanager.cpp
      databasesession.cpp
    )

#set ( core_UIS
    #  ui/sessionwidget.ui
    #)

include_directories ( ${CMAKE_CURRENT_BINARY_DIR}
                      ${CMAKE_CURRENT_SOURCE_DIR} 
                      ${QT_QTNETWORK_INCLUDE_DIR}
                      ${QT_QTSQL_INCLUDE_DIR}
		     )

QT4_ADD_RESOURCES ( core_SRCS_RES resources.qrc )
QT4_AUTOMOC (${core_SRCS})
QT4_WRAP_UI( core_SRCS ${core_UIS} )

add_library (zmviewercore SHARED ${core_SRCS} ${core_SRCS_RES} )
target_link_libraries( zmviewercore ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTGUI_LIBRARY} )
install(TARGETS zmviewercore DESTINATION lib)
