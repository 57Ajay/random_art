# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.31

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\msys64\mingw64\bin\cmake.exe

# The command to remove a file.
RM = C:\msys64\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\projects\cProjects\ran-art

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\projects\cProjects\ran-art\build

# Include any dependencies generated for this target.
include CMakeFiles/ran-art.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ran-art.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ran-art.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ran-art.dir/flags.make

CMakeFiles/ran-art.dir/codegen:
.PHONY : CMakeFiles/ran-art.dir/codegen

CMakeFiles/ran-art.dir/src/main.c.obj: CMakeFiles/ran-art.dir/flags.make
CMakeFiles/ran-art.dir/src/main.c.obj: CMakeFiles/ran-art.dir/includes_C.rsp
CMakeFiles/ran-art.dir/src/main.c.obj: C:/projects/cProjects/ran-art/src/main.c
CMakeFiles/ran-art.dir/src/main.c.obj: CMakeFiles/ran-art.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\projects\cProjects\ran-art\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ran-art.dir/src/main.c.obj"
	C:\msys64\mingw64\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/ran-art.dir/src/main.c.obj -MF CMakeFiles\ran-art.dir\src\main.c.obj.d -o CMakeFiles\ran-art.dir\src\main.c.obj -c C:\projects\cProjects\ran-art\src\main.c

CMakeFiles/ran-art.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ran-art.dir/src/main.c.i"
	C:\msys64\mingw64\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\projects\cProjects\ran-art\src\main.c > CMakeFiles\ran-art.dir\src\main.c.i

CMakeFiles/ran-art.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ran-art.dir/src/main.c.s"
	C:\msys64\mingw64\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\projects\cProjects\ran-art\src\main.c -o CMakeFiles\ran-art.dir\src\main.c.s

# Object files for target ran-art
ran__art_OBJECTS = \
"CMakeFiles/ran-art.dir/src/main.c.obj"

# External object files for target ran-art
ran__art_EXTERNAL_OBJECTS =

bin/ran-art.exe: CMakeFiles/ran-art.dir/src/main.c.obj
bin/ran-art.exe: CMakeFiles/ran-art.dir/build.make
bin/ran-art.exe: CMakeFiles/ran-art.dir/linkLibs.rsp
bin/ran-art.exe: CMakeFiles/ran-art.dir/objects1.rsp
bin/ran-art.exe: CMakeFiles/ran-art.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\projects\cProjects\ran-art\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin\ran-art.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ran-art.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ran-art.dir/build: bin/ran-art.exe
.PHONY : CMakeFiles/ran-art.dir/build

CMakeFiles/ran-art.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ran-art.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ran-art.dir/clean

CMakeFiles/ran-art.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\projects\cProjects\ran-art C:\projects\cProjects\ran-art C:\projects\cProjects\ran-art\build C:\projects\cProjects\ran-art\build C:\projects\cProjects\ran-art\build\CMakeFiles\ran-art.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ran-art.dir/depend

