file(GLOB_RECURSE BINET_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/binet/*.c
)

add_library(binet ${BINET_SOURCES})

target_include_directories(binet PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(binet PROPERTIES
        C_STANDARD 11
        CXX_STANDARD 17
)