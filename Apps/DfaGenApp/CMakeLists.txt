add_executable(DfaGenApp main.cpp)

target_link_libraries(DfaGenApp PRIVATE DfaGen Serializer)