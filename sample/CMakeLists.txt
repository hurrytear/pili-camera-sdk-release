cmake_minimum_required(VERSION 3.5)
project(sample)

message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

set(SOURCE_FILES 
	src/demo.c 
	src/flv-parser.c
	)

include_directories(
	"${PROJECT_SOURCE_DIR}" 
	"${PROJECT_SOURCE_DIR}/../inc/" 
	"/usr/local/include"
	"/usr/include"
	)

if(BUILD_ARM)
link_directories(	
	"${PROJECT_SOURCE_DIR}/../lib/arm_linux/"
	)
else()
link_directories(
	"${PROJECT_SOURCE_DIR}/../lib/linux_x86-64/"
	)
endif()

link_directories(${link_directories}
	"/usr/local/lib" 
	"/usr/lib"
	)

add_executable(sample ${SOURCE_FILES})

target_link_libraries(sample "libpili_push.so" ${CMAKE_DL_LIBS})
