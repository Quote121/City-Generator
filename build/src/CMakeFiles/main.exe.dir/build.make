# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/matt/git/City-Generator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/matt/git/City-Generator/build

# Include any dependencies generated for this target.
include src/CMakeFiles/main.exe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/main.exe.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/main.exe.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/main.exe.dir/flags.make

src/CMakeFiles/main.exe.dir/camera.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/camera.cpp.o: ../src/camera.cpp
src/CMakeFiles/main.exe.dir/camera.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/main.exe.dir/camera.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/camera.cpp.o -MF CMakeFiles/main.exe.dir/camera.cpp.o.d -o CMakeFiles/main.exe.dir/camera.cpp.o -c /home/matt/git/City-Generator/src/camera.cpp

src/CMakeFiles/main.exe.dir/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/camera.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/camera.cpp > CMakeFiles/main.exe.dir/camera.cpp.i

src/CMakeFiles/main.exe.dir/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/camera.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/camera.cpp -o CMakeFiles/main.exe.dir/camera.cpp.s

src/CMakeFiles/main.exe.dir/glad.c.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/glad.c.o: ../src/glad.c
src/CMakeFiles/main.exe.dir/glad.c.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/main.exe.dir/glad.c.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/glad.c.o -MF CMakeFiles/main.exe.dir/glad.c.o.d -o CMakeFiles/main.exe.dir/glad.c.o -c /home/matt/git/City-Generator/src/glad.c

src/CMakeFiles/main.exe.dir/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.exe.dir/glad.c.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/matt/git/City-Generator/src/glad.c > CMakeFiles/main.exe.dir/glad.c.i

src/CMakeFiles/main.exe.dir/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.exe.dir/glad.c.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/matt/git/City-Generator/src/glad.c -o CMakeFiles/main.exe.dir/glad.c.s

src/CMakeFiles/main.exe.dir/inputHandler.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/inputHandler.cpp.o: ../src/inputHandler.cpp
src/CMakeFiles/main.exe.dir/inputHandler.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/main.exe.dir/inputHandler.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/inputHandler.cpp.o -MF CMakeFiles/main.exe.dir/inputHandler.cpp.o.d -o CMakeFiles/main.exe.dir/inputHandler.cpp.o -c /home/matt/git/City-Generator/src/inputHandler.cpp

src/CMakeFiles/main.exe.dir/inputHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/inputHandler.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/inputHandler.cpp > CMakeFiles/main.exe.dir/inputHandler.cpp.i

src/CMakeFiles/main.exe.dir/inputHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/inputHandler.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/inputHandler.cpp -o CMakeFiles/main.exe.dir/inputHandler.cpp.s

src/CMakeFiles/main.exe.dir/main.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/main.cpp.o: ../src/main.cpp
src/CMakeFiles/main.exe.dir/main.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/main.exe.dir/main.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/main.cpp.o -MF CMakeFiles/main.exe.dir/main.cpp.o.d -o CMakeFiles/main.exe.dir/main.cpp.o -c /home/matt/git/City-Generator/src/main.cpp

src/CMakeFiles/main.exe.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/main.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/main.cpp > CMakeFiles/main.exe.dir/main.cpp.i

src/CMakeFiles/main.exe.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/main.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/main.cpp -o CMakeFiles/main.exe.dir/main.cpp.s

src/CMakeFiles/main.exe.dir/menues.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/menues.cpp.o: ../src/menues.cpp
src/CMakeFiles/main.exe.dir/menues.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/main.exe.dir/menues.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/menues.cpp.o -MF CMakeFiles/main.exe.dir/menues.cpp.o.d -o CMakeFiles/main.exe.dir/menues.cpp.o -c /home/matt/git/City-Generator/src/menues.cpp

src/CMakeFiles/main.exe.dir/menues.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/menues.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/menues.cpp > CMakeFiles/main.exe.dir/menues.cpp.i

src/CMakeFiles/main.exe.dir/menues.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/menues.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/menues.cpp -o CMakeFiles/main.exe.dir/menues.cpp.s

src/CMakeFiles/main.exe.dir/mesh.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/mesh.cpp.o: ../src/mesh.cpp
src/CMakeFiles/main.exe.dir/mesh.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/main.exe.dir/mesh.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/mesh.cpp.o -MF CMakeFiles/main.exe.dir/mesh.cpp.o.d -o CMakeFiles/main.exe.dir/mesh.cpp.o -c /home/matt/git/City-Generator/src/mesh.cpp

src/CMakeFiles/main.exe.dir/mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/mesh.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/mesh.cpp > CMakeFiles/main.exe.dir/mesh.cpp.i

src/CMakeFiles/main.exe.dir/mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/mesh.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/mesh.cpp -o CMakeFiles/main.exe.dir/mesh.cpp.s

src/CMakeFiles/main.exe.dir/model.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/model.cpp.o: ../src/model.cpp
src/CMakeFiles/main.exe.dir/model.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/main.exe.dir/model.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/model.cpp.o -MF CMakeFiles/main.exe.dir/model.cpp.o.d -o CMakeFiles/main.exe.dir/model.cpp.o -c /home/matt/git/City-Generator/src/model.cpp

src/CMakeFiles/main.exe.dir/model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/model.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/model.cpp > CMakeFiles/main.exe.dir/model.cpp.i

src/CMakeFiles/main.exe.dir/model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/model.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/model.cpp -o CMakeFiles/main.exe.dir/model.cpp.s

src/CMakeFiles/main.exe.dir/object.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/object.cpp.o: ../src/object.cpp
src/CMakeFiles/main.exe.dir/object.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/main.exe.dir/object.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/object.cpp.o -MF CMakeFiles/main.exe.dir/object.cpp.o.d -o CMakeFiles/main.exe.dir/object.cpp.o -c /home/matt/git/City-Generator/src/object.cpp

src/CMakeFiles/main.exe.dir/object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/object.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/object.cpp > CMakeFiles/main.exe.dir/object.cpp.i

src/CMakeFiles/main.exe.dir/object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/object.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/object.cpp -o CMakeFiles/main.exe.dir/object.cpp.s

src/CMakeFiles/main.exe.dir/scene.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/scene.cpp.o: ../src/scene.cpp
src/CMakeFiles/main.exe.dir/scene.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/CMakeFiles/main.exe.dir/scene.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/scene.cpp.o -MF CMakeFiles/main.exe.dir/scene.cpp.o.d -o CMakeFiles/main.exe.dir/scene.cpp.o -c /home/matt/git/City-Generator/src/scene.cpp

src/CMakeFiles/main.exe.dir/scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/scene.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/scene.cpp > CMakeFiles/main.exe.dir/scene.cpp.i

src/CMakeFiles/main.exe.dir/scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/scene.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/scene.cpp -o CMakeFiles/main.exe.dir/scene.cpp.s

src/CMakeFiles/main.exe.dir/stb_image.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/stb_image.cpp.o: ../src/stb_image.cpp
src/CMakeFiles/main.exe.dir/stb_image.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/CMakeFiles/main.exe.dir/stb_image.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/stb_image.cpp.o -MF CMakeFiles/main.exe.dir/stb_image.cpp.o.d -o CMakeFiles/main.exe.dir/stb_image.cpp.o -c /home/matt/git/City-Generator/src/stb_image.cpp

src/CMakeFiles/main.exe.dir/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/stb_image.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/stb_image.cpp > CMakeFiles/main.exe.dir/stb_image.cpp.i

src/CMakeFiles/main.exe.dir/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/stb_image.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/stb_image.cpp -o CMakeFiles/main.exe.dir/stb_image.cpp.s

src/CMakeFiles/main.exe.dir/texture.cpp.o: src/CMakeFiles/main.exe.dir/flags.make
src/CMakeFiles/main.exe.dir/texture.cpp.o: ../src/texture.cpp
src/CMakeFiles/main.exe.dir/texture.cpp.o: src/CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object src/CMakeFiles/main.exe.dir/texture.cpp.o"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.exe.dir/texture.cpp.o -MF CMakeFiles/main.exe.dir/texture.cpp.o.d -o CMakeFiles/main.exe.dir/texture.cpp.o -c /home/matt/git/City-Generator/src/texture.cpp

src/CMakeFiles/main.exe.dir/texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.exe.dir/texture.cpp.i"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matt/git/City-Generator/src/texture.cpp > CMakeFiles/main.exe.dir/texture.cpp.i

src/CMakeFiles/main.exe.dir/texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.exe.dir/texture.cpp.s"
	cd /home/matt/git/City-Generator/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matt/git/City-Generator/src/texture.cpp -o CMakeFiles/main.exe.dir/texture.cpp.s

# Object files for target main.exe
main_exe_OBJECTS = \
"CMakeFiles/main.exe.dir/camera.cpp.o" \
"CMakeFiles/main.exe.dir/glad.c.o" \
"CMakeFiles/main.exe.dir/inputHandler.cpp.o" \
"CMakeFiles/main.exe.dir/main.cpp.o" \
"CMakeFiles/main.exe.dir/menues.cpp.o" \
"CMakeFiles/main.exe.dir/mesh.cpp.o" \
"CMakeFiles/main.exe.dir/model.cpp.o" \
"CMakeFiles/main.exe.dir/object.cpp.o" \
"CMakeFiles/main.exe.dir/scene.cpp.o" \
"CMakeFiles/main.exe.dir/stb_image.cpp.o" \
"CMakeFiles/main.exe.dir/texture.cpp.o"

# External object files for target main.exe
main_exe_EXTERNAL_OBJECTS =

src/main.exe: src/CMakeFiles/main.exe.dir/camera.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/glad.c.o
src/main.exe: src/CMakeFiles/main.exe.dir/inputHandler.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/main.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/menues.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/mesh.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/model.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/object.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/scene.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/stb_image.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/texture.cpp.o
src/main.exe: src/CMakeFiles/main.exe.dir/build.make
src/main.exe: ../libs/GLFW_linux/libglfw3.a
src/main.exe: src/imgui/libcity_imgui.a.so
src/main.exe: src/CMakeFiles/main.exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/matt/git/City-Generator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable main.exe"
	cd /home/matt/git/City-Generator/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/main.exe.dir/build: src/main.exe
.PHONY : src/CMakeFiles/main.exe.dir/build

src/CMakeFiles/main.exe.dir/clean:
	cd /home/matt/git/City-Generator/build/src && $(CMAKE_COMMAND) -P CMakeFiles/main.exe.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/main.exe.dir/clean

src/CMakeFiles/main.exe.dir/depend:
	cd /home/matt/git/City-Generator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/matt/git/City-Generator /home/matt/git/City-Generator/src /home/matt/git/City-Generator/build /home/matt/git/City-Generator/build/src /home/matt/git/City-Generator/build/src/CMakeFiles/main.exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/main.exe.dir/depend
