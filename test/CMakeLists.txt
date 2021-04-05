# © 2020 Joseph Cameron - All Rights Reserved

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(Threads REQUIRED)
endif()

jfc_add_tests(
    C++_STANDARD 17
    C_STANDARD 90

    TEST_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/context_test.cpp"

    INCLUDE_DIRECTORIES
        "${${PROJECT_NAME}_INCLUDE_DIRECTORIES}"

    LIBRARIES
        "${${PROJECT_NAME}_LIBRARIES}"
        ${CMAKE_THREAD_LIBS_INIT}

    DEPENDENCIES
        "jfchttp"
)
