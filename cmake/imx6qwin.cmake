# this is required
SET(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# specify the cross compiler
SET(CMAKE_C_COMPILER E:/usr/linaro/gcc/bin/arm-linux-gnueabihf-gcc.exe)
SET(CMAKE_CXX_COMPILER E:/usr/linaro/gcc/bin/arm-linux-gnueabihf-g++.exe)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  E:/usr/linaro/gcc/)
SET(CMAKE_EXT_LIBRARAY_PATH  E:/usr/linaro/imx6q)
# search for programs in the build host directories (not necessary)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# configure Boost and Qt
#SET(QT_QMAKE_EXECUTABLE /opt/qt-embedded/qmake)
#SET(BOOST_ROOT /opt/boost_arm)

if(CMAKE_INCLUDE_PREFIX)
include_directories(${CMAKE_INCLUDE_PREFIX}/include)
LINK_DIRECTORIES (${CMAKE_INCLUDE_PREFIX}/lib)
endif()
include_directories(${CMAKE_EXT_LIBRARAY_PATH}/include/gstreamer-0.10)
include_directories(${CMAKE_EXT_LIBRARAY_PATH}/include/glib-2.0)
include_directories(${CMAKE_EXT_LIBRARAY_PATH}/lib/glib-2.0/include)
include_directories(${CMAKE_EXT_LIBRARAY_PATH}/include/libxml2)
LINK_DIRECTORIES (${CMAKE_EXT_LIBRARAY_PATH}/lib)