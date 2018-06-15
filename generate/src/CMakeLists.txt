#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目

set(APP_SRCS
	${PROJECT_SOURCE_DIR}/src/index.cpp
	${PROJECT_SOURCE_DIR}/src/fingerprint.cpp
	${PROJECT_SOURCE_DIR}/src/hash.cpp
	${PROJECT_SOURCE_DIR}/src/recog.cpp
	${PROJECT_SOURCE_DIR}/src/wav.cpp
	)
if(PASSOA_COMPILER_MSVC)
ADD_EXECUTABLE(audio
				${APP_SRCS}
				)
endif()

if(PASSOA_COMPILER_NDK)

include_directories(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES (${Boost_LIB_DIR})
ADD_EXECUTABLE(audio
				${APP_SRCS}
				${MODULE_SRCS}
				${DUK_SRCS}
				${PROJECT_SOURCE_DIR}/src/filesystem/gcc_filesystem.cpp
				${PROJECT_SOURCE_DIR}/src/net/ndk_serialport.cpp
				)

target_link_libraries(passoa
             ${Boost_LIBs}
             )
endif()
if(PASSOA_COMPILER_GCC)

include_directories(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES (${Boost_LIB_DIR})
ADD_EXECUTABLE(passoa
				${APP_SRCS}
				${MODULE_SRCS}
				${DUK_SRCS}
				${PROJECT_SOURCE_DIR}/src/filesystem/gcc_filesystem.cpp
				${PROJECT_SOURCE_DIR}/src/net/serialport.cpp
				)

target_link_libraries(passoa
             ${Boost_LIBs}
             )
endif()

add_custom_target(install_script
		COMMAND echo "sync script files!!!"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/modules ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(ConfigurationName)/node_modules
        COMMENT "Generate project output file" 
)

message(${PROJECT_SOURCE_DIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/ DESTINATION node_modules)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/test/ DESTINATION test
         FILES_MATCHING
		  PATTERN "*.js"
		  PATTERN "*.bat"
		  PATTERN "*.json")
install(TARGETS audio
	RUNTIME DESTINATION ./
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
