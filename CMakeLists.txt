cmake_minimum_required(VERSION 3.30)

project(vole VERSION 0.1.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 23)

include(import_fmt)
include(import_json)

add_library(vole
    src/datamodel.cpp
    src/node_printer.cpp
    src/exception.cpp
    src/datamodel_parsers.cpp
)
target_sources(vole
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include/
            FILES
                include/vole/datamodel.hpp
                include/vole/datamodel_parsers.hpp
                include/vole/exception.hpp
                include/vole/node_printer.hpp
)
target_link_libraries(vole PUBLIC
    nlohmann_json::nlohmann_json
    fmt::fmt
)

add_executable(vole_renderer src/main.cpp)
target_link_libraries(vole_renderer PUBLIC vole)

if (PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(tests)
endif()
