message(STATUS "/src/objects/ called")

file(GLOB OBJECTS_SOURCES "*.cpp")

# Export the variable to the parent scope
set(OBJECTS_SOURCES ${OBJECTS_SOURCES} PARENT_SCOPE)