#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目

if(MSVC)
add_library(passoa_audio SHARED
				${PROJECT_SOURCE_DIR}/js/binding.cpp
				${PROJECT_SOURCE_DIR}/js/export.def
				${PROJECT_SOURCE_DIR}/js/duk/duktape.c
				${PROJECT_SOURCE_DIR}/src/audio.cpp
				)
target_link_libraries(passoa_audio
             portaudio_x86
             )
set(CMAKE_SHARED_LIBRARY_SUFFIX ".passoa")
install(TARGETS passoa_audio
	RUNTIME DESTINATION x86windows)
else()

endif()
