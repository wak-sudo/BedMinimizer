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

add_executable(verifier_unit_tests ${TEST_SOURCES})

target_include_directories(verifier_unit_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(verifier_unit_tests
    PRIVATE
        Verifier
        Catch2::Catch2WithMain
)

catch_discover_tests(verifier_unit_tests)