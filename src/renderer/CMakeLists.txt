message(STATUS "/src/renderer/ called")

file(GLOB RENDERER_SOURCES "*.cpp")

# Export the variable to the parent scope
set(RENDERER_SOURCES ${RENDERER_SOURCES} PARENT_SCOPE)
