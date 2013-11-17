# this one is important
SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/bin/i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/i686-w64-mingw32-c++)
SET(CMAKE_RC_COMPILER /usr/bin/i686-w64-mingw32-windres)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/i686-w64-mingw32)
