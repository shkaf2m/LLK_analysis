cmake_minimum_required(VERSION 3.10)
project(LLK_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
add_subdirectory(pugixml)

add_library(grammarlib STATIC grammar.hpp grammar.cpp)
target_include_directories(grammarlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(parserlib STATIC parser.hpp parser.cpp)
target_include_directories(parserlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(firstlib STATIC first.hpp first.cpp)
target_include_directories(firstlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(firstlib PUBLIC grammarlib)

add_executable(my_app main.cpp)

target_include_directories(my_app PRIVATE pugixml/src)
target_link_libraries(my_app PRIVATE pugixml grammarlib parserlib firstlib)