cmake_minimum_required(VERSION 3.30)

project(vole VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    URL_HASH MD5=c23a33f04786d85c29fda8d16b5f0efd
)
FetchContent_MakeAvailable(json)

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
)

add_executable(vole_renderer src/main.cpp)
target_link_libraries(vole_renderer PUBLIC vole)

if (PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(tests)
endif()
