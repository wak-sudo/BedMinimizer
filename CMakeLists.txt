cmake_minimum_required(VERSION 3.16)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(Utils)
add_subdirectory(Serializer)
add_subdirectory(ResParser)
add_subdirectory(AugScc)
add_subdirectory(CodeGen)
add_subdirectory(Verifier)
add_subdirectory(BedMinimizer)
add_subdirectory(DfaGen)
add_subdirectory(Apps)