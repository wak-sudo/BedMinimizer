if(NOT BUILD_TESTS)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(dfaGen_unit_tests ${TEST_SOURCES})

target_link_libraries(dfaGen_unit_tests
    PRIVATE
        DfaGen
        AugScc
        Utils
        Catch2::Catch2WithMain
)

catch_discover_tests(dfaGen_unit_tests)