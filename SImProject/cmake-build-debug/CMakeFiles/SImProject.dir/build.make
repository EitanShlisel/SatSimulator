# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.2.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.2.5\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\97250\CLionProjects\SImProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\97250\CLionProjects\SImProject\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SImProject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SImProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SImProject.dir/flags.make

CMakeFiles/SImProject.dir/main.c.obj: CMakeFiles/SImProject.dir/flags.make
CMakeFiles/SImProject.dir/main.c.obj: main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/SImProject.dir/main.c.obj"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\SImProject.dir\main.c.obj   -c C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\main.c

CMakeFiles/SImProject.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SImProject.dir/main.c.i"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\main.c > CMakeFiles\SImProject.dir\main.c.i

CMakeFiles/SImProject.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SImProject.dir/main.c.s"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\main.c -o CMakeFiles\SImProject.dir\main.c.s

CMakeFiles/SImProject.dir/RTC.c.obj: CMakeFiles/SImProject.dir/flags.make
CMakeFiles/SImProject.dir/RTC.c.obj: RTC.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/SImProject.dir/RTC.c.obj"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\SImProject.dir\RTC.c.obj   -c C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\RTC.c

CMakeFiles/SImProject.dir/RTC.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SImProject.dir/RTC.c.i"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\RTC.c > CMakeFiles\SImProject.dir\RTC.c.i

CMakeFiles/SImProject.dir/RTC.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SImProject.dir/RTC.c.s"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\RTC.c -o CMakeFiles\SImProject.dir\RTC.c.s

CMakeFiles/SImProject.dir/Time.c.obj: CMakeFiles/SImProject.dir/flags.make
CMakeFiles/SImProject.dir/Time.c.obj: Time.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/SImProject.dir/Time.c.obj"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\SImProject.dir\Time.c.obj   -c C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\Time.c

CMakeFiles/SImProject.dir/Time.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SImProject.dir/Time.c.i"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\Time.c > CMakeFiles\SImProject.dir\Time.c.i

CMakeFiles/SImProject.dir/Time.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SImProject.dir/Time.c.s"
	C:\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\Time.c -o CMakeFiles\SImProject.dir\Time.c.s

# Object files for target SImProject
SImProject_OBJECTS = \
"CMakeFiles/SImProject.dir/main.c.obj" \
"CMakeFiles/SImProject.dir/RTC.c.obj" \
"CMakeFiles/SImProject.dir/Time.c.obj"

# External object files for target SImProject
SImProject_EXTERNAL_OBJECTS =

SImProject.exe: CMakeFiles/SImProject.dir/main.c.obj
SImProject.exe: CMakeFiles/SImProject.dir/RTC.c.obj
SImProject.exe: CMakeFiles/SImProject.dir/Time.c.obj
SImProject.exe: CMakeFiles/SImProject.dir/build.make
SImProject.exe: CMakeFiles/SImProject.dir/linklibs.rsp
SImProject.exe: CMakeFiles/SImProject.dir/objects1.rsp
SImProject.exe: CMakeFiles/SImProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable SImProject.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\SImProject.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SImProject.dir/build: SImProject.exe

.PHONY : CMakeFiles/SImProject.dir/build

CMakeFiles/SImProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SImProject.dir\cmake_clean.cmake
.PHONY : CMakeFiles/SImProject.dir/clean

CMakeFiles/SImProject.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\97250\CLionProjects\SImProject C:\Users\97250\CLionProjects\SImProject C:\Users\97250\CLionProjects\SImProject\cmake-build-debug C:\Users\97250\CLionProjects\SImProject\cmake-build-debug C:\Users\97250\CLionProjects\SImProject\cmake-build-debug\CMakeFiles\SImProject.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SImProject.dir/depend
