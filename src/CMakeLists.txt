message(STATUS "/src make called")

set(EXECUTABLE_NAME "main.exe")
message(STATUS "Name set as ${EXECUTABLE_NAME}")

## Include folder below
add_subdirectory(imgui)

## Include dir
# message(STATUS "including directory: ${CMAKE_SOURCE_DIR}/include")
# include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "*.cpp" "*.c")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

## PRIVATE for only this target
target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
    # ${CMAKE_SOURCE_DIR}/libs/assimp/libassimp.a 
    ${CMAKE_SOURCE_DIR}/libs/GLFW_linux/libglfw3.a
    assimp
    city_imgui.a # Lib specified
    X11 # Unix graphics lib 11th x window system
)
