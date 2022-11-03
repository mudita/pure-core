cmake_minimum_required(VERSION 3.23)
project(pure-core C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(third-party)

add_library(pure-core STATIC
        src/boot_control.c
        src/boot_reason.c
        src/common/json.c
        )
add_library(pure::core ALIAS pure-core)


target_include_directories(pure-core
        PRIVATE
        include/hal
        src
        PUBLIC
        include
        )

target_link_libraries(pure-core PRIVATE cjson)

# Enable unit tests only if configured as a standalone module
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(tests)
    endif ()
endif ()
