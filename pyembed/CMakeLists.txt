cmake_minimum_required(VERSION 3.16)

list(APPEND BUILD_FILES 
    source/main.cpp
)

find_package(Python3 COMPONENTS Development)

add_executable(${PROJECT_NAME} ${BUILD_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE pybind11::embed)

if(MSVC)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
            ../../$<TARGET_FILE_NAME:${PROJECT_NAME}>
)
endif()