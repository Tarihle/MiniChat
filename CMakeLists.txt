# ~Network/CMakeLists.txt

cmake_minimum_required(VERSION 3.24 FATAL_ERROR)	#PATH_EQUAL

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)	#Takes the name of the current folder as project name.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(${PROJECT_NAME})

set(TARGET_NAME Network)
add_subdirectory(NetworkLibrary)
set(ADDITIONAL_INCLUDE_DIRECTORY ${ADDITIONAL_INCLUDE_DIRECTORY} ${${TARGET_NAME}_HEADER_DIRECTORY})
set(ADDITIONAL_LINK_LIBRARY ${ADDITIONAL_LINK_LIBRARY} ${TARGET_NAME})
unset(TARGET_NAME)

add_subdirectory(MiniChatClient)
add_subdirectory(MiniChatServer)

message(STATUS ${Network_HEADER_DIRECTORY})

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MiniChatServer)
endif()
