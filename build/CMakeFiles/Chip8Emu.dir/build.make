# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/robbie/code/Chip8Emu

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/robbie/code/Chip8Emu/build

# Include any dependencies generated for this target.
include CMakeFiles/Chip8Emu.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Chip8Emu.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Chip8Emu.dir/flags.make

CMakeFiles/Chip8Emu.dir/src/main.cpp.o: CMakeFiles/Chip8Emu.dir/flags.make
CMakeFiles/Chip8Emu.dir/src/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/robbie/code/Chip8Emu/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Chip8Emu.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Chip8Emu.dir/src/main.cpp.o -c /home/robbie/code/Chip8Emu/src/main.cpp

CMakeFiles/Chip8Emu.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Chip8Emu.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/robbie/code/Chip8Emu/src/main.cpp > CMakeFiles/Chip8Emu.dir/src/main.cpp.i

CMakeFiles/Chip8Emu.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Chip8Emu.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/robbie/code/Chip8Emu/src/main.cpp -o CMakeFiles/Chip8Emu.dir/src/main.cpp.s

CMakeFiles/Chip8Emu.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/Chip8Emu.dir/src/main.cpp.o.requires

CMakeFiles/Chip8Emu.dir/src/main.cpp.o.provides: CMakeFiles/Chip8Emu.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Chip8Emu.dir/build.make CMakeFiles/Chip8Emu.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/Chip8Emu.dir/src/main.cpp.o.provides

CMakeFiles/Chip8Emu.dir/src/main.cpp.o.provides.build: CMakeFiles/Chip8Emu.dir/src/main.cpp.o

CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o: CMakeFiles/Chip8Emu.dir/flags.make
CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o: ../src/cpu.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/robbie/code/Chip8Emu/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o -c /home/robbie/code/Chip8Emu/src/cpu.cpp

CMakeFiles/Chip8Emu.dir/src/cpu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Chip8Emu.dir/src/cpu.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/robbie/code/Chip8Emu/src/cpu.cpp > CMakeFiles/Chip8Emu.dir/src/cpu.cpp.i

CMakeFiles/Chip8Emu.dir/src/cpu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Chip8Emu.dir/src/cpu.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/robbie/code/Chip8Emu/src/cpu.cpp -o CMakeFiles/Chip8Emu.dir/src/cpu.cpp.s

CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o.requires:
.PHONY : CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o.requires

CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o.provides: CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o.requires
	$(MAKE) -f CMakeFiles/Chip8Emu.dir/build.make CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o.provides.build
.PHONY : CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o.provides

CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o.provides.build: CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o

# Object files for target Chip8Emu
Chip8Emu_OBJECTS = \
"CMakeFiles/Chip8Emu.dir/src/main.cpp.o" \
"CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o"

# External object files for target Chip8Emu
Chip8Emu_EXTERNAL_OBJECTS =

Chip8Emu: CMakeFiles/Chip8Emu.dir/src/main.cpp.o
Chip8Emu: CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o
Chip8Emu: CMakeFiles/Chip8Emu.dir/build.make
Chip8Emu: /usr/lib/x86_64-linux-gnu/libSDL2main.a
Chip8Emu: /usr/lib/x86_64-linux-gnu/libSDL2.so
Chip8Emu: CMakeFiles/Chip8Emu.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Chip8Emu"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Chip8Emu.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Chip8Emu.dir/build: Chip8Emu
.PHONY : CMakeFiles/Chip8Emu.dir/build

CMakeFiles/Chip8Emu.dir/requires: CMakeFiles/Chip8Emu.dir/src/main.cpp.o.requires
CMakeFiles/Chip8Emu.dir/requires: CMakeFiles/Chip8Emu.dir/src/cpu.cpp.o.requires
.PHONY : CMakeFiles/Chip8Emu.dir/requires

CMakeFiles/Chip8Emu.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Chip8Emu.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Chip8Emu.dir/clean

CMakeFiles/Chip8Emu.dir/depend:
	cd /home/robbie/code/Chip8Emu/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/robbie/code/Chip8Emu /home/robbie/code/Chip8Emu /home/robbie/code/Chip8Emu/build /home/robbie/code/Chip8Emu/build /home/robbie/code/Chip8Emu/build/CMakeFiles/Chip8Emu.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Chip8Emu.dir/depend

