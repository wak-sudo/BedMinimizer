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

file(GLOB TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(aug_scc_unit_tests
    ${TEST_SOURCES}
)

target_link_libraries(aug_scc_unit_tests
    PRIVATE
        AugScc
        Catch2::Catch2WithMain
)

include(Catch)

catch_discover_tests(aug_scc_unit_tests)