cmake_minimum_required(VERSION 3.16)
project(Verifier LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(Verifier STATIC ${SOURCES})

target_include_directories(Verifier
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Shared
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

target_link_libraries(Verifier
    PRIVATE
        Utils
)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()