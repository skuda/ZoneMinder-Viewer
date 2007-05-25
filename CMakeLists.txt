project (zmviewer)

find_package ( Qt4 REQUIRED )
 
include ( ${QT_USE_FILE} )
 
include_directories ( QT_INCLUDE_DIR ) 
 
#uncomment if you don't need debug outputs
#set (ZMVIEWERDEF -DQT_NO_DEBUG_OUTPUT )
 
add_definitions(${QT_DEFINITIONS} ${ZMVIEWERDEF}  )

add_subdirectory ( src )
