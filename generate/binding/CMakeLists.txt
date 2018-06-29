#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目

if(MSVC)
add_library(audio SHARED
				${PROJECT_SOURCE_DIR}/binding/index.cpp
				${PROJECT_SOURCE_DIR}/binding/jsLoop.cpp
				${PROJECT_SOURCE_DIR}/binding/jsAudio.cpp
				${PROJECT_SOURCE_DIR}/binding/export.def
				${PROJECT_SOURCE_DIR}/binding/duk/duktape.c
				${PROJECT_SOURCE_DIR}/src/audio.cpp
				)
target_link_libraries(audio
             portaudio_x86
             )
set(CMAKE_SHARED_LIBRARY_SUFFIX ".passoa")
install(TARGETS audio
	RUNTIME DESTINATION x86windows)
add_custom_target(install_script
	COMMAND echo "sync script files!!!"
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/modules ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(ConfigurationName)/
    COMMENT "Generate project output file" 
)
else()

endif()
