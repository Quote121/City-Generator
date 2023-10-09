cmake_minimum_required(VERSION 3.0.0)

project(city-gen VERSION 1.0 DESCRIPTION "A city generator")

message(STATUS "including directory: ${CMAKE_SOURCE_DIR}/include")
## For project specific headers
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/city_gen)
include_directories(${CMAKE_SOURCE_DIR}/include/city_gen/renderer)


## Set debug
set(CMAKE_BUILD_TYPE Debug)
## Wall
set(CMAKE_CXX_FLAGS "-Wall")

## Set c++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)
