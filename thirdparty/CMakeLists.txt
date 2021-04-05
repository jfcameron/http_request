# © Joseph Cameron - All Rights Reserved

#set(dependencies
#    "example_dependency"
#)
#jfc_add_dependencies(${dependencies})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/curl")

    set (CURL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/curl/include" PARENT_SCOPE)
    set (CURL_LIBRARIES "${PROJECT_BINARY_DIR}/thirdparty/curl/libcurl-d${CMAKE_STATIC_LIBRARY_SUFFIX};ws2_32.lib;wldap32.lib;advapi32.lib;kernel32.lib;comdlg32.lib;crypt32.lib;normaliz.lib" PARENT_SCOPE)
    set (CURL_LIBRARY_NAME "libcurl" PARENT_SCOPE)
else()
    FIND_PACKAGE(CURL REQUIRED)

    set (CURL_INCLUDE_DIR "${CURL_INCLUDE_DIR}" PARENT_SCOPE)
    set (CURL_LIBRARIES "${CURL_LIBRARIES}" PARENT_SCOPE)
endif()

