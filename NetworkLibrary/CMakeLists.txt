# ~NetworkLibrary/CMakeLists.txt

if(NOT TARGET_NAME)
	get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)	#Takes the name of the current folder as project name.
	set(TARGET_NAME ${TARGET_NAME} PARENT_SCOPE)
endif()

file(GLOB_RECURSE TARGET_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl
	)

file(GLOB_RECURSE TARGET_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/*.c++
	)
	
file(GLOB_RECURSE TARGET_EXTRA_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.txt
	${CMAKE_CURRENT_SOURCE_DIR}/*.md
	)
	
set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES} ${TARGET_EXTRA_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Public/Network)
target_link_libraries(${TARGET_NAME} PRIVATE Ws2_32.lib)

set(${TARGET_NAME}_HEADER_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Public PARENT_SCOPE)

if (MSVC)
	message(STATUS "Using MSVC, target_compile_options for ${TARGET_NAME} set to W4 WX.")
	target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
	message("Not using MSVC, target_compile_options for ${TARGET_NAME} not set.")
endif()