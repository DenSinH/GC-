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
include src\Debugger\CMakeFiles\Debugger.dir\depend.make

# Include the progress variables for this target.
include src\Debugger\CMakeFiles\Debugger.dir\progress.make

# Include the compile flags for this target's objects.
include src\Debugger\CMakeFiles\Debugger.dir\flags.make

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui.cpp.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui.cpp.obj: ..\src\Debugger\imgui\imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/Debugger/CMakeFiles/Debugger.dir/imgui/imgui.cpp.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Debugger.dir\imgui\imgui.cpp.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\imgui\imgui.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Debugger.dir/imgui/imgui.cpp.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\imgui\imgui.cpp.i @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CProjects\GC-\src\Debugger\imgui\imgui.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Debugger.dir/imgui/imgui.cpp.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\imgui\imgui.cpp.s /c D:\CProjects\GC-\src\Debugger\imgui\imgui.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.obj: ..\src\Debugger\imgui\imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/Debugger/CMakeFiles/Debugger.dir/imgui/imgui_demo.cpp.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\imgui\imgui_demo.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Debugger.dir/imgui/imgui_demo.cpp.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.i @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CProjects\GC-\src\Debugger\imgui\imgui_demo.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Debugger.dir/imgui/imgui_demo.cpp.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.s /c D:\CProjects\GC-\src\Debugger\imgui\imgui_demo.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.obj: ..\src\Debugger\imgui\imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/Debugger/CMakeFiles/Debugger.dir/imgui/imgui_draw.cpp.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\imgui\imgui_draw.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Debugger.dir/imgui/imgui_draw.cpp.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.i @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CProjects\GC-\src\Debugger\imgui\imgui_draw.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Debugger.dir/imgui/imgui_draw.cpp.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.s /c D:\CProjects\GC-\src\Debugger\imgui\imgui_draw.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.obj: ..\src\Debugger\imgui\imgui_impl_opengl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/Debugger/CMakeFiles/Debugger.dir/imgui/imgui_impl_opengl3.cpp.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\imgui\imgui_impl_opengl3.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Debugger.dir/imgui/imgui_impl_opengl3.cpp.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.i @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CProjects\GC-\src\Debugger\imgui\imgui_impl_opengl3.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Debugger.dir/imgui/imgui_impl_opengl3.cpp.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.s /c D:\CProjects\GC-\src\Debugger\imgui\imgui_impl_opengl3.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.obj: ..\src\Debugger\imgui\imgui_impl_sdl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/Debugger/CMakeFiles/Debugger.dir/imgui/imgui_impl_sdl.cpp.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\imgui\imgui_impl_sdl.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Debugger.dir/imgui/imgui_impl_sdl.cpp.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.i @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CProjects\GC-\src\Debugger\imgui\imgui_impl_sdl.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Debugger.dir/imgui/imgui_impl_sdl.cpp.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.s /c D:\CProjects\GC-\src\Debugger\imgui\imgui_impl_sdl.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.obj: ..\src\Debugger\imgui\imgui_widgets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/Debugger/CMakeFiles/Debugger.dir/imgui/imgui_widgets.cpp.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\imgui\imgui_widgets.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Debugger.dir/imgui/imgui_widgets.cpp.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.i @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CProjects\GC-\src\Debugger\imgui\imgui_widgets.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Debugger.dir/imgui/imgui_widgets.cpp.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.s /c D:\CProjects\GC-\src\Debugger\imgui\imgui_widgets.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\debugger.cpp.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\debugger.cpp.obj: ..\src\Debugger\debugger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/Debugger/CMakeFiles/Debugger.dir/debugger.cpp.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Debugger.dir\debugger.cpp.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\debugger.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\debugger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Debugger.dir/debugger.cpp.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\debugger.cpp.i @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CProjects\GC-\src\Debugger\debugger.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\debugger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Debugger.dir/debugger.cpp.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\debugger.cpp.s /c D:\CProjects\GC-\src\Debugger\debugger.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\widgets\menubar.cpp.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\widgets\menubar.cpp.obj: ..\src\Debugger\widgets\menubar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/Debugger/CMakeFiles/Debugger.dir/widgets/menubar.cpp.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Debugger.dir\widgets\menubar.cpp.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\widgets\menubar.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\widgets\menubar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Debugger.dir/widgets/menubar.cpp.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\widgets\menubar.cpp.i @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CProjects\GC-\src\Debugger\widgets\menubar.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\widgets\menubar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Debugger.dir/widgets/menubar.cpp.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo -TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\widgets\menubar.cpp.s /c D:\CProjects\GC-\src\Debugger\widgets\menubar.cpp
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\widgets\disassemble.c.obj: src\Debugger\CMakeFiles\Debugger.dir\flags.make
src\Debugger\CMakeFiles\Debugger.dir\widgets\disassemble.c.obj: ..\src\Debugger\widgets\disassemble.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object src/Debugger/CMakeFiles/Debugger.dir/widgets/disassemble.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Debugger.dir\widgets\disassemble.c.obj /FdCMakeFiles\Debugger.dir\Debugger.pdb -c D:\CProjects\GC-\src\Debugger\widgets\disassemble.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\widgets\disassemble.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Debugger.dir/widgets/disassemble.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Debugger.dir\widgets\disassemble.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Debugger\widgets\disassemble.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Debugger\CMakeFiles\Debugger.dir\widgets\disassemble.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Debugger.dir/widgets/disassemble.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Debugger.dir\widgets\disassemble.c.s /c D:\CProjects\GC-\src\Debugger\widgets\disassemble.c
<<
	cd D:\CProjects\GC-\cmake-build-release

# Object files for target Debugger
Debugger_OBJECTS = \
"CMakeFiles\Debugger.dir\imgui\imgui.cpp.obj" \
"CMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.obj" \
"CMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.obj" \
"CMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.obj" \
"CMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.obj" \
"CMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.obj" \
"CMakeFiles\Debugger.dir\debugger.cpp.obj" \
"CMakeFiles\Debugger.dir\widgets\menubar.cpp.obj" \
"CMakeFiles\Debugger.dir\widgets\disassemble.c.obj"

# External object files for target Debugger
Debugger_EXTERNAL_OBJECTS =

src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui.cpp.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_demo.cpp.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_draw.cpp.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_opengl3.cpp.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_impl_sdl.cpp.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\imgui\imgui_widgets.cpp.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\debugger.cpp.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\widgets\menubar.cpp.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\widgets\disassemble.c.obj
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\build.make
src\Debugger\Debugger.lib: src\Debugger\CMakeFiles\Debugger.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX static library Debugger.lib"
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	$(CMAKE_COMMAND) -P CMakeFiles\Debugger.dir\cmake_clean_target.cmake
	cd D:\CProjects\GC-\cmake-build-release
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\lld-link.exe" /lib /nologo /machine:X86 /out:Debugger.lib @CMakeFiles\Debugger.dir\objects1.rsp 
	cd D:\CProjects\GC-\cmake-build-release

# Rule to build all files generated by this target.
src\Debugger\CMakeFiles\Debugger.dir\build: src\Debugger\Debugger.lib

.PHONY : src\Debugger\CMakeFiles\Debugger.dir\build

src\Debugger\CMakeFiles\Debugger.dir\clean:
	cd D:\CProjects\GC-\cmake-build-release\src\Debugger
	$(CMAKE_COMMAND) -P CMakeFiles\Debugger.dir\cmake_clean.cmake
	cd D:\CProjects\GC-\cmake-build-release
.PHONY : src\Debugger\CMakeFiles\Debugger.dir\clean

src\Debugger\CMakeFiles\Debugger.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\CProjects\GC- D:\CProjects\GC-\src\Debugger D:\CProjects\GC-\cmake-build-release D:\CProjects\GC-\cmake-build-release\src\Debugger D:\CProjects\GC-\cmake-build-release\src\Debugger\CMakeFiles\Debugger.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src\Debugger\CMakeFiles\Debugger.dir\depend
