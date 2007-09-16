set ( shell_SRCS
      about.cpp
      cameraselectdialog.cpp
      main.cpp
      multicameraview.cpp
      mainwindow.cpp
      styles/styledialog.cpp
      styles/style.cpp
    )

QT4_ADD_RESOURCES ( shell_SRCS shell_resources.qrc )


include_directories ( ${CMAKE_CURRENT_BINARY_DIR}
                      ${CMAKE_CURRENT_SOURCE_DIR} 
                      ${CMAKE_SOURCE_DIR}/src/core
                      ${CMAKE_BINARY_DIR}/src/core
                      ${CMAKE_BINARY_DIR}
                      ${QT_QTXML_INCLUDE_DIR}
             )



QT4_AUTOMOC (${shell_SRCS})
add_executable (zmviewer ${shell_SRCS} )
target_link_libraries( zmviewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} zmviewercore )
install(TARGETS zmviewer DESTINATION ${ZMVIEWER_BIN_DIR})

install ( DIRECTORY styles/examples/blackstyle DESTINATION ${ZMVIEWER_STYLE_DIR}  PATTERN ".svn" EXCLUDE )
install ( DIRECTORY styles/examples/whitestyle DESTINATION ${ZMVIEWER_STYLE_DIR} PATTERN ".svn" EXCLUDE )
