cmake_minimum_required(VERSION 3.31)
project(binet LANGUAGES C)

option(BUILD_SHARED_LIBS "Build shared (.dll/.so) or static (.lib/.a)" ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)

install(DIRECTORY include/binet DESTINATION include)

add_executable(binet_test test/main.c)
target_link_libraries(binet_test PRIVATE binet)
target_include_directories(binet_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET binet_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:binet>
            $<TARGET_FILE_DIR:binet_test>
    )
endif ()