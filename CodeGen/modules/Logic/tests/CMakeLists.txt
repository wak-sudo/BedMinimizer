if(NOT BUILD_TESTS)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_Declare(
    TogaSAT
    GIT_REPOSITORY https://github.com/togatoga/togasat.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(Catch2 TogaSAT)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(logic_unit_tests ${TEST_SOURCES})

target_include_directories(logic_unit_tests
    PRIVATE
        ${togasat_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/internal
)

target_link_libraries(logic_unit_tests
    PRIVATE
        Logic
        Catch2::Catch2WithMain
)

catch_discover_tests(logic_unit_tests)