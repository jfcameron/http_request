# © 2020 Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(null)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

option(JFC_BUILD_DEMO "Build the demo" ON)
option(JFC_BUILD_DOCS "Build documentation" OFF)
option(JFC_BUILD_TESTS "Build unit tests" ON)

#add_subdirectory(thirdparty)

FIND_PACKAGE(CURL REQUIRED)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(Threads REQUIRED)
endif()

jfc_project(library
    NAME "jfchttp"
    VERSION 0.0
    DESCRIPTION "concurrent, object oriented http lib"
    C++_STANDARD 17
    C_STANDARD 90
    TYPE STATIC

    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CURL_INCLUDE_DIR}

    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/curl/include

    SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/src/context.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/curl/src/curl_context.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/curl/src/curl_get.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/curl/src/curl_request.cpp

    LIBRARIES
        ${CURL_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}

#    DEPENDENCIES
#        "example"
)

if (JFC_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if (JFC_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JFC_BUILD_DOCS)
    add_subdirectory(docs)
endif()

