
include(import_gtest)

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