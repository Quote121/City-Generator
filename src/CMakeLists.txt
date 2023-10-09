message(STATUS "/src make called")

set(EXECUTABLE_NAME "main.exe")
message(STATUS "Name set as ${EXECUTABLE_NAME}")

set(EXECUTABLE_DIR "${CMAKE_SOURCE_DIR}/bin")
message(STATUS "Dir set as ${EXECUTABLE_DIR}")

set(CITYLIBS_DIR "${CMAKE_SOURCE_DIR}/libs/city_gen")
message(STATUS "Name imgui dir as ${CITYLIBS_DIR}")

## Include folder below
add_subdirectory(imgui)
add_subdirectory(renderer)

file(GLOB SRC_SOURCES "*.cpp" "*.c")
add_executable(${EXECUTABLE_NAME}            
            ${SRC_SOURCES}
            ${RENDERER_SOURCES}
)

## Set RPATH for loader to find the imgui library
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    INSTALL_RPATH "${CITYLIBS_DIR}"
)


## PRIVATE for only this target
target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
    # ${CMAKE_SOURCE_DIR}/libs/assimp/libassimp.a 
    ${CMAKE_SOURCE_DIR}/libs/GLFW_linux/libglfw3.a
    assimp
    city_imgui.a # Lib specified
    X11 # Unix graphics lib 11th x window system
)


message(STATUS "Install directory: ${CMAKE_SOURCE_DIR}/bin")

## Check if directories exist
## If they dont, create them
if (EXISTS "${EXECUTABLE_DIR}")
    message("Folder '${EXECUTABLE_DIR}' exists.")
else()
    message("Folder '${EXECUTABLE_DIR}' does not exist. Creating...")
    file(MAKE_DIRECTORY ${EXECUTABLE_DIR})
endif()

if (EXISTS "${CITYLIBS_DIR}")
    message("Folder '${CITYLIBS_DIR}' exists.")
else()
    message("Folder '${CITYLIBS_DIR}' does not exist. Creating...")
    file(MAKE_DIRECTORY ${CITYLIBS_DIR})
endif()


## Install the executable and library
INSTALL(TARGETS ${EXECUTABLE_NAME} city_imgui.a
    DESTINATION ${EXECUTABLE_DIR}
    LIBRARY DESTINATION ${CITYLIBS_DIR}
)


## Run argument
add_custom_target(run
    COMMAND ${EXECUTABLE_NAME}
    DEPENDS ${EXECUTABLE_NAME}
    WORKING_DIRECTORY ${EXECUTABLE_DIR}  # Set the working directory to where your executable is located
    COMMENT "Running ${EXECUTABLE_NAME}"
)
