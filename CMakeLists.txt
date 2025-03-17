cmake_minimum_required(VERSION 3.30)

project(vole VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_library(vole
    src/datamodel.cpp
    src/render.cpp
    src/exception.cpp
)

target_sources(vole
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include/
            FILES
                include/vole/datamodel.hpp
                include/vole/exception.hpp
                include/vole/render.hpp
)

add_executable(vole_renderer src/main.cpp)
target_link_libraries(vole_renderer PUBLIC vole)

if (PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(tests)
endif()
