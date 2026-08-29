cmake_minimum_required(VERSION 3.16)
project(CodeGen VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(CodeGen STATIC ${SOURCES})

add_subdirectory(modules/Logic)

target_include_directories(CodeGen
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../Contract
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(CodeGen
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Shared
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

target_link_libraries(CodeGen
    PRIVATE
        Logic
        Utils
)

