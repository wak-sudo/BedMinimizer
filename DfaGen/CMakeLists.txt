cmake_minimum_required(VERSION 3.16)
project(DfaGen VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_library(DfaGen STATIC ${SOURCES})

target_include_directories(DfaGen
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Shared
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

target_link_libraries(DfaGen
    PRIVATE
        AugScc
        Utils
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()