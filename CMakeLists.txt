cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(lox)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${CMAKE_PROJECT_NAME}
	source/main.cpp
	)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CONAN_LIBS})

set(COMPILE_FLAGS
	-std=c++2a
	-Wall
	-Wextra
	-Wpedantic
	)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${COMPILE_FLAGS})
