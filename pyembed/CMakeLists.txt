cmake_minimum_required(VERSION 3.16)

find_package(Python3 COMPONENTS Development)

add_executable(${PROJECT_NAME}
    source/main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE pybind11::embed)

set(DOWNLOADED_PYTHON_EMBED_ZIP ${CMAKE_BINARY_DIR}/python-${PYTHON_VERSION}-embed-amd64.zip)

file(DOWNLOAD https://www.python.org/ftp/python/${PYTHON_VERSION}/python-${PYTHON_VERSION}-embed-amd64.zip ${CMAKE_BINARY_DIR}/python-${PYTHON_VERSION}-embed-amd64.zip
    TIMEOUT 60  # seconds
    TLS_VERIFY ON)

if(MSVC)
    file(ARCHIVE_EXTRACT INPUT ${DOWNLOADED_PYTHON_EMBED_ZIP}
        DESTINATION ${CMAKE_BINARY_DIR}/bin
        PATTERNS python39.zip python39.dll vcruntime140.dll
    )
else()
    file(ARCHIVE_EXTRACT INPUT ${DOWNLOADED_PYTHON_EMBED_ZIP}
        DESTINATION ${CMAKE_BINARY_DIR}/bin
        PATTERNS python39.zip
    )
endif()