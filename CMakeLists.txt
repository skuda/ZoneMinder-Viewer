project (zmviewer)

SET(QT_MIN_VERSION "4.3.0")

find_package ( Qt4 REQUIRED )
 
include ( ${QT_USE_FILE} )
 
include_directories ( QT_INCLUDE_DIR ) 
 
#uncomment if you don't need debug outputs
#set (ZMVIEWERDEF -DQT_NO_DEBUG_OUTPUT )

set(ZMVIEWER_INSTALL_DIR  ${CMAKE_INSTALL_PREFIX} )
set(ZMVIEWER_BIN_DIR       ${ZMVIEWER_INSTALL_DIR}/bin )
set(ZMVIEWER_DATA_DIR     ${ZMVIEWER_INSTALL_DIR}/share/zmviewer )
set(ZMVIEWER_STYLE_DIR    ${ZMVIEWER_DATA_DIR}/styles )
set(ZMVIEWER_LIB_DIR      ${ZMVIEWER_INSTALL_DIR}/lib )
set(ZMVIEWER_BIN_DIR      ${ZMVIEWER_INSTALL_DIR}/bin )

configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-zmviewer.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-zmviewer.h
)

 
add_definitions(${QT_DEFINITIONS} ${ZMVIEWERDEF}  )

add_subdirectory ( src )
