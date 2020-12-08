cmake_minimum_required(VERSION 3.16)

project(pyembed	
    VERSION 0.1.0
	DESCRIPTION "An awesome python embedded C++ project"
    LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

enable_testing()

add_subdirectory(pyembed)