set ( core_SRCS
      stream.cpp
    )

include_directories ( ${CMAKE_CURRENT_BINARY_DIR}
                      ${CMAKE_CURRENT_SOURCE_DIR} 
                      ${QT_QTNETWORK_INCLUDE_DIR}
		     )

QT4_AUTOMOC (${core_SRCS})
add_library (zmviewercore SHARED ${core_SRCS})
target_link_libraries( zmviewercore ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY})
install(TARGETS zmviewercore DESTINATION lib)
