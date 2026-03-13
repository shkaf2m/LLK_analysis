cmake_minimum_required(VERSION 3.10)
project(LLK_project)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(pugixml)
add_library(grammarlib INTERFACE grammar.hpp)
add_library(parserlib STATIC parser.hpp parser.cpp)
target_include_directories(parserlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(my_exe main.cpp)

target_include_directories(my_exe PRIVATE pugixml/src)
target_link_libraries(my_exe PRIVATE pugixml grammarlib parserlib)