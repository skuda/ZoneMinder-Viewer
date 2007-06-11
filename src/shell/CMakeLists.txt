set ( shell_SRCS
      about.cpp
      cameraselectdialog.cpp
      main.cpp
      multicameraview.cpp
      mainwindow.cpp
    )

QT4_ADD_RESOURCES ( shell_SRCS shell_resources.qrc )
QT4_WRAP_UI( shell_SRCS ${shell_UIS} )


include_directories ( ${CMAKE_CURRENT_BINARY_DIR}
                      ${CMAKE_CURRENT_SOURCE_DIR} 
                      ${CMAKE_SOURCE_DIR}/src/core
                      ${CMAKE_BINARY_DIR}/src/core
             )

QT4_AUTOMOC (${shell_SRCS})
add_executable (zmviewer ${shell_SRCS} )
target_link_libraries( zmviewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} zmviewercore )
install(TARGETS zmviewer DESTINATION bin)
