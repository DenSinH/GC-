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
include CMakeFiles\GC_.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\GC_.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\GC_.dir\flags.make

CMakeFiles\GC_.dir\main.c.obj: CMakeFiles\GC_.dir\flags.make
CMakeFiles\GC_.dir\main.c.obj: ..\main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/GC_.dir/main.c.obj"
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\GC_.dir\main.c.obj /FdCMakeFiles\GC_.dir\ -c D:\CProjects\GC-\main.c
<<

CMakeFiles\GC_.dir\main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/GC_.dir/main.c.i"
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\GC_.dir\main.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\main.c
<<

CMakeFiles\GC_.dir\main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/GC_.dir/main.c.s"
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\GC_.dir\main.c.s /c D:\CProjects\GC-\main.c
<<

CMakeFiles\GC_.dir\UIthread.c.obj: CMakeFiles\GC_.dir\flags.make
CMakeFiles\GC_.dir\UIthread.c.obj: ..\UIthread.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/GC_.dir/UIthread.c.obj"
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\GC_.dir\UIthread.c.obj /FdCMakeFiles\GC_.dir\ -c D:\CProjects\GC-\UIthread.c
<<

CMakeFiles\GC_.dir\UIthread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/GC_.dir/UIthread.c.i"
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\GC_.dir\UIthread.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\UIthread.c
<<

CMakeFiles\GC_.dir\UIthread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/GC_.dir/UIthread.c.s"
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\GC_.dir\UIthread.c.s /c D:\CProjects\GC-\UIthread.c
<<

# Object files for target GC_
GC__OBJECTS = \
"CMakeFiles\GC_.dir\main.c.obj" \
"CMakeFiles\GC_.dir\UIthread.c.obj"

# External object files for target GC_
GC__EXTERNAL_OBJECTS =

GC_.exe: CMakeFiles\GC_.dir\main.c.obj
GC_.exe: CMakeFiles\GC_.dir\UIthread.c.obj
GC_.exe: CMakeFiles\GC_.dir\build.make
GC_.exe: src\Core\Core.lib
GC_.exe: src\Frontend\Frontend.lib
GC_.exe: src\Loader\Loader.lib
GC_.exe: src\Core\HWregs\Registers.lib
GC_.exe: D:\vcpkg\vcpkg\installed\x86-windows\lib\imgui.lib
GC_.exe: D:\vcpkg\vcpkg\installed\x86-windows\lib\SDL2.lib
GC_.exe: D:\vcpkg\vcpkg\installed\x86-windows\lib\manual-link\SDL2main.lib
GC_.exe: D:\vcpkg\vcpkg\installed\x86-windows\lib\capstone_dll.lib
GC_.exe: src\Frontend\Renderer\Renderer.lib
GC_.exe: D:\vcpkg\vcpkg\installed\x86-windows\lib\glad.lib
GC_.exe: D:\vcpkg\vcpkg\installed\x86-windows\lib\manual-link\SDL2main.lib
GC_.exe: D:\vcpkg\vcpkg\installed\x86-windows\lib\SDL2.lib
GC_.exe: CMakeFiles\GC_.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable GC_.exe"
	"D:\CLion\CLion 2020.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\GC_.dir --rc="C:\PROGRA~2\Windows Kits\10\bin\10.0.18362.0\x86\rc.exe" --mt="C:\PROGRA~2\Windows Kits\10\bin\10.0.18362.0\x86\mt.exe" --manifests  -- "C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\lld-link.exe" /nologo @CMakeFiles\GC_.dir\objects1.rsp @<<
 /out:GC_.exe /implib:GC_.lib /pdb:D:\CProjects\GC-\cmake-build-release\GC_.pdb /version:0.0  /machine:X86 /INCREMENTAL:NO /subsystem:console  src\Core\Core.lib src\Frontend\Frontend.lib src\Loader\Loader.lib src\Core\HWregs\Registers.lib D:\vcpkg\vcpkg\installed\x86-windows\lib\imgui.lib D:\vcpkg\vcpkg\installed\x86-windows\lib\SDL2.lib user32.lib gdi32.lib winmm.lib imm32.lib ole32.lib oleaut32.lib version.lib uuid.lib advapi32.lib setupapi.lib shell32.lib dinput8.lib D:\vcpkg\vcpkg\installed\x86-windows\lib\manual-link\SDL2main.lib D:\vcpkg\vcpkg\installed\x86-windows\lib\capstone_dll.lib src\Frontend\Renderer\Renderer.lib D:\vcpkg\vcpkg\installed\x86-windows\lib\glad.lib D:\vcpkg\vcpkg\installed\x86-windows\lib\manual-link\SDL2main.lib D:\vcpkg\vcpkg\installed\x86-windows\lib\SDL2.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<
	powershell -noprofile -executionpolicy Bypass -file D:/vcpkg/vcpkg/scripts/buildsystems/msbuild/applocal.ps1 -targetBinary D:/CProjects/GC-/cmake-build-release/GC_.exe -installedDir D:/vcpkg/vcpkg/installed/x86-windows/bin -OutVariable out

# Rule to build all files generated by this target.
CMakeFiles\GC_.dir\build: GC_.exe

.PHONY : CMakeFiles\GC_.dir\build

CMakeFiles\GC_.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\GC_.dir\cmake_clean.cmake
.PHONY : CMakeFiles\GC_.dir\clean

CMakeFiles\GC_.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\CProjects\GC- D:\CProjects\GC- D:\CProjects\GC-\cmake-build-release D:\CProjects\GC-\cmake-build-release D:\CProjects\GC-\cmake-build-release\CMakeFiles\GC_.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\GC_.dir\depend

