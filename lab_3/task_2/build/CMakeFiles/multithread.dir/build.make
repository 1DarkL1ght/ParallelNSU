# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\nsu prog\ParallelNSU\lab_3\task_2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\nsu prog\ParallelNSU\lab_3\task_2\build"

# Include any dependencies generated for this target.
include CMakeFiles/multithread.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/multithread.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/multithread.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/multithread.dir/flags.make

CMakeFiles/multithread.dir/main.cpp.obj: CMakeFiles/multithread.dir/flags.make
CMakeFiles/multithread.dir/main.cpp.obj: C:/nsu\ prog/ParallelNSU/lab_3/task_2/main.cpp
CMakeFiles/multithread.dir/main.cpp.obj: CMakeFiles/multithread.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\nsu prog\ParallelNSU\lab_3\task_2\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/multithread.dir/main.cpp.obj"
	C:\mingw32\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/multithread.dir/main.cpp.obj -MF CMakeFiles\multithread.dir\main.cpp.obj.d -o CMakeFiles\multithread.dir\main.cpp.obj -c "C:\nsu prog\ParallelNSU\lab_3\task_2\main.cpp"

CMakeFiles/multithread.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/multithread.dir/main.cpp.i"
	C:\mingw32\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\nsu prog\ParallelNSU\lab_3\task_2\main.cpp" > CMakeFiles\multithread.dir\main.cpp.i

CMakeFiles/multithread.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/multithread.dir/main.cpp.s"
	C:\mingw32\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\nsu prog\ParallelNSU\lab_3\task_2\main.cpp" -o CMakeFiles\multithread.dir\main.cpp.s

# Object files for target multithread
multithread_OBJECTS = \
"CMakeFiles/multithread.dir/main.cpp.obj"

# External object files for target multithread
multithread_EXTERNAL_OBJECTS =

multithread.exe: CMakeFiles/multithread.dir/main.cpp.obj
multithread.exe: CMakeFiles/multithread.dir/build.make
multithread.exe: CMakeFiles/multithread.dir/linkLibs.rsp
multithread.exe: CMakeFiles/multithread.dir/objects1.rsp
multithread.exe: CMakeFiles/multithread.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="C:\nsu prog\ParallelNSU\lab_3\task_2\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable multithread.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\multithread.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/multithread.dir/build: multithread.exe
.PHONY : CMakeFiles/multithread.dir/build

CMakeFiles/multithread.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\multithread.dir\cmake_clean.cmake
.PHONY : CMakeFiles/multithread.dir/clean

CMakeFiles/multithread.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\nsu prog\ParallelNSU\lab_3\task_2" "C:\nsu prog\ParallelNSU\lab_3\task_2" "C:\nsu prog\ParallelNSU\lab_3\task_2\build" "C:\nsu prog\ParallelNSU\lab_3\task_2\build" "C:\nsu prog\ParallelNSU\lab_3\task_2\build\CMakeFiles\multithread.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/multithread.dir/depend

