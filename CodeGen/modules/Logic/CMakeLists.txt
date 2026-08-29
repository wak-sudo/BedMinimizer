cmake_minimum_required(VERSION 3.16)
project(Logic VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_library(Logic STATIC
    src/Clause.cpp
    src/Formula.cpp
    src/Literal.cpp
    src/BitVector.cpp
    src/DimacsWriter.cpp
    src/LogicAst.cpp
    src/Tseytin.cpp
)

target_include_directories(Logic
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()