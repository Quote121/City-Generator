cmake_minimum_required(VERSION 3.0.0)

project(city-gen VERSION 1.0 DESCRIPTION "A city generator")

message(STATUS "including directory: ${CMAKE_SOURCE_DIR}/include")
## For project specific headers
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/city_gen)
include_directories(${CMAKE_SOURCE_DIR}/include/city_gen/renderer)
include_directories(${CMAKE_SOURCE_DIR}/include/city_gen/objects)



## Set debug
set(CMAKE_BUILD_TYPE Debug)
## Wall
set(CMAKE_CXX_FLAGS "-Wall")

## Set c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

## To generate the compile_commands.json file for LSP clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

