message(STATUS "/src/imgui/ called")

file(GLOB SOURCES "*.cpp")

## Include dir
message(STATUS "including directory: ${CMAKE_SOURCE_DIR}/include/imgui")
include_directories(${CMAKE_SOURCE_DIR}/include/imgui)

add_library(city_imgui.a SHARED 
    ${SOURCES}
)
