# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hw01.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hw01.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hw01.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw01.dir/flags.make

CMakeFiles/hw01.dir/src/main.cpp.o: CMakeFiles/hw01.dir/flags.make
CMakeFiles/hw01.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/hw01.dir/src/main.cpp.o: CMakeFiles/hw01.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hw01.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hw01.dir/src/main.cpp.o -MF CMakeFiles/hw01.dir/src/main.cpp.o.d -o CMakeFiles/hw01.dir/src/main.cpp.o -c /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/src/main.cpp

CMakeFiles/hw01.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw01.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/src/main.cpp > CMakeFiles/hw01.dir/src/main.cpp.i

CMakeFiles/hw01.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw01.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/src/main.cpp -o CMakeFiles/hw01.dir/src/main.cpp.s

# Object files for target hw01
hw01_OBJECTS = \
"CMakeFiles/hw01.dir/src/main.cpp.o"

# External object files for target hw01
hw01_EXTERNAL_OBJECTS =

hw01: CMakeFiles/hw01.dir/src/main.cpp.o
hw01: CMakeFiles/hw01.dir/build.make
hw01: CMakeFiles/hw01.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hw01"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw01.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw01.dir/build: hw01
.PHONY : CMakeFiles/hw01.dir/build

CMakeFiles/hw01.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw01.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw01.dir/clean

CMakeFiles/hw01.dir/depend:
	cd /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/cmake-build-debug /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/cmake-build-debug /mnt/c/Users/pro/Desktop/Geomatics/Q3/Modelling/hw01/Solution/cpp/cmake-build-debug/CMakeFiles/hw01.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw01.dir/depend

