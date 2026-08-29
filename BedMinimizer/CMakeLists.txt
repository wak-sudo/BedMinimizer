cmake_minimum_required(VERSION 3.16)
project(BedMinimizer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_library(BedMinimizer STATIC ${SOURCES})

target_include_directories(BedMinimizer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Shared
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(BedMinimizer
    PRIVATE
        Serializer
        CodeGen
        ResParser
        Verifier
        AugScc
)