cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(lox)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SOURCE_FILES
	source/memory.cpp
	source/debug.cpp
	source/chunk.cpp
	source/value.cpp
	source/bits.cpp
	source/virtual_machine.cpp
	source/compiler.cpp
	source/scanner.cpp
	)

add_executable(${CMAKE_PROJECT_NAME}
	${SOURCE_FILES}
	source/main.cpp
	)

add_executable(test_${CMAKE_PROJECT_NAME}
	${SOURCE_FILES}
	tests/test_chunk.cpp
	tests/test_bits.cpp
	tests/test_main.cpp
	)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
target_include_directories(test_${CMAKE_PROJECT_NAME} PRIVATE include)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CONAN_LIBS})
target_link_libraries(test_${CMAKE_PROJECT_NAME} PRIVATE ${CONAN_LIBS})

set(COMPILE_FLAGS
	-std=c++2a
	-Wall
	-Wextra
	-Wpedantic
	)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${COMPILE_FLAGS})
target_compile_options(test_${CMAKE_PROJECT_NAME} PRIVATE ${COMPILE_FLAGS})
