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

if(NOT EXISTS ${DOWNLOADED_PYTHON_EMBED_ZIP})
    set(DOWNLOAD_URL https://www.python.org/ftp/python/${PYTHON_VERSION}/python-${PYTHON_VERSION}-embed-amd64.zip)
    file(DOWNLOAD ${DOWNLOAD_URL} ${DOWNLOADED_PYTHON_EMBED_ZIP}
        TIMEOUT 60  # seconds
        TLS_VERIFY ON
    )
endif()

set(PYTHON_BUILTIN_ZIP_FILENAME python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}.zip)

if(MSVC)
    set(PYTHON_RUNTIME_FILENAME python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}.dll)
    file(ARCHIVE_EXTRACT INPUT ${DOWNLOADED_PYTHON_EMBED_ZIP}
        DESTINATION ${CMAKE_BINARY_DIR}/bin
        PATTERNS ${PYTHON_RUNTIME_FILENAME} vcruntime140.dll
    )

    file(ARCHIVE_EXTRACT INPUT ${DOWNLOADED_PYTHON_EMBED_ZIP}
        DESTINATION ${CMAKE_BINARY_DIR}/bin/builtins
        PATTERNS ${PYTHON_BUILTIN_ZIP_FILENAME}
    )
else()
    file(ARCHIVE_EXTRACT INPUT ${DOWNLOADED_PYTHON_EMBED_ZIP}
        DESTINATION ${CMAKE_BINARY_DIR}/bin
        PATTERNS PYTHON_BUILTIN_ZIP_FILENAME
    )
endif()

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#             ${CMAKE_BINARY_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
#             ../../$<TARGET_FILE_NAME:${PROJECT_NAME}>
# )
