# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion\CLion 2020.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion\CLion 2020.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\CProjects\GC-

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\CProjects\GC-\cmake-build-release

# Include any dependencies generated for this target.
include src\Loader\CMakeFiles\Loader.dir\depend.make

# Include the progress variables for this target.
include src\Loader\CMakeFiles\Loader.dir\progress.make

# Include the compile flags for this target's objects.
include src\Loader\CMakeFiles\Loader.dir\flags.make

src\Loader\CMakeFiles\Loader.dir\loader.c.obj: src\Loader\CMakeFiles\Loader.dir\flags.make
src\Loader\CMakeFiles\Loader.dir\loader.c.obj: ..\src\Loader\loader.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/Loader/CMakeFiles/Loader.dir/loader.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Loader.dir\loader.c.obj /FdCMakeFiles\Loader.dir\Loader.pdb -c D:\CProjects\GC-\src\Loader\loader.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Loader\CMakeFiles\Loader.dir\loader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Loader.dir/loader.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Loader.dir\loader.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Loader\loader.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Loader\CMakeFiles\Loader.dir\loader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Loader.dir/loader.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Loader.dir\loader.c.s /c D:\CProjects\GC-\src\Loader\loader.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Loader\CMakeFiles\Loader.dir\helpers.c.obj: src\Loader\CMakeFiles\Loader.dir\flags.make
src\Loader\CMakeFiles\Loader.dir\helpers.c.obj: ..\src\Loader\helpers.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/Loader/CMakeFiles/Loader.dir/helpers.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Loader.dir\helpers.c.obj /FdCMakeFiles\Loader.dir\Loader.pdb -c D:\CProjects\GC-\src\Loader\helpers.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Loader\CMakeFiles\Loader.dir\helpers.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Loader.dir/helpers.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Loader.dir\helpers.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Loader\helpers.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Loader\CMakeFiles\Loader.dir\helpers.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Loader.dir/helpers.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Loader.dir\helpers.c.s /c D:\CProjects\GC-\src\Loader\helpers.c
<<
	cd D:\CProjects\GC-\cmake-build-release

# Object files for target Loader
Loader_OBJECTS = \
"CMakeFiles\Loader.dir\loader.c.obj" \
"CMakeFiles\Loader.dir\helpers.c.obj"

# External object files for target Loader
Loader_EXTERNAL_OBJECTS =

src\Loader\Loader.lib: src\Loader\CMakeFiles\Loader.dir\loader.c.obj
src\Loader\Loader.lib: src\Loader\CMakeFiles\Loader.dir\helpers.c.obj
src\Loader\Loader.lib: src\Loader\CMakeFiles\Loader.dir\build.make
src\Loader\Loader.lib: src\Loader\CMakeFiles\Loader.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library Loader.lib"
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	$(CMAKE_COMMAND) -P CMakeFiles\Loader.dir\cmake_clean_target.cmake
	cd D:\CProjects\GC-\cmake-build-release
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\lld-link.exe" /lib /nologo /machine:X86 /out:Loader.lib @CMakeFiles\Loader.dir\objects1.rsp 
	cd D:\CProjects\GC-\cmake-build-release

# Rule to build all files generated by this target.
src\Loader\CMakeFiles\Loader.dir\build: src\Loader\Loader.lib

.PHONY : src\Loader\CMakeFiles\Loader.dir\build

src\Loader\CMakeFiles\Loader.dir\clean:
	cd D:\CProjects\GC-\cmake-build-release\src\Loader
	$(CMAKE_COMMAND) -P CMakeFiles\Loader.dir\cmake_clean.cmake
	cd D:\CProjects\GC-\cmake-build-release
.PHONY : src\Loader\CMakeFiles\Loader.dir\clean

src\Loader\CMakeFiles\Loader.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\CProjects\GC- D:\CProjects\GC-\src\Loader D:\CProjects\GC-\cmake-build-release D:\CProjects\GC-\cmake-build-release\src\Loader D:\CProjects\GC-\cmake-build-release\src\Loader\CMakeFiles\Loader.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src\Loader\CMakeFiles\Loader.dir\depend
