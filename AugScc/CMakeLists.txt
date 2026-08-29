cmake_minimum_required(VERSION 3.16)
project(AugScc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" ON)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(json)

file(GLOB SOURCES "src/*.cpp")

add_library(AugScc STATIC ${SOURCES})

target_include_directories(AugScc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Shared
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

target_link_libraries(AugScc
    PRIVATE
        Utils
)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()