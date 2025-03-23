
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
    URL_HASH MD5=92a5fd39c0952595b0ceea41805dd79d
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(vole_units
    datamodel_tests.cpp
    datamodel_parser_tests.cpp
    helpers/visualizers.hpp
)
target_sources(vole_units
    PUBLIC
        FILE_SET HEADERS
        FILES
            helpers/assertions.h
)
target_link_libraries(vole_units
    PUBLIC
        vole
        GTest::gtest_main
)

gtest_discover_tests(vole_units)