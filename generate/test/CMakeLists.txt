#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
if(PASSOA_COMPILER_MSVC)
#add_library(poseidon SHARED
ADD_EXECUTABLE(cpptest
				${PROJECT_SOURCE_DIR}/test/cpptest/index.cpp
				${PROJECT_SOURCE_DIR}/src/filesystem/path.cpp
				${PROJECT_SOURCE_DIR}/src/filesystem/msvc_filesystem.cpp
				)
target_link_libraries(cpptest
				debug gtestd
				optimized gtest)
add_custom_target(jstest
                  COMMAND 
				  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(ConfigurationName)/passoa ${PROJECT_SOURCE_DIR}/test/module/index.js
				  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(ConfigurationName)/passoa ${PROJECT_SOURCE_DIR}/test/addon/index.js)
elseif(PASSOA_COMPILER_NDK)
include_directories(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES (${Boost_LIB_DIR})
ADD_EXECUTABLE(duktest
				${PROJECT_SOURCE_DIR}/test/cpptest/arm.gcc.index.cpp
				${PROJECT_SOURCE_DIR}/src/duk/duktape.c
				${PROJECT_SOURCE_DIR}/src/duk/duk_module_node.c
				${PROJECT_SOURCE_DIR}/src/process/child_process.cpp
				${PROJECT_SOURCE_DIR}/src/filesystem/gcc_filesystem.cpp
				${PROJECT_SOURCE_DIR}/src/module.cpp
				${PROJECT_SOURCE_DIR}/src/filesystem/path.cpp
				${PROJECT_SOURCE_DIR}/src/script.cpp
				${PROJECT_SOURCE_DIR}/src/console.cpp
				${PROJECT_SOURCE_DIR}/src/process/loop.cpp
				${PROJECT_SOURCE_DIR}/src/net/socket.cpp
				)
target_link_libraries(duktest
				${Boost_LIBs})
endif()
