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
include src\Core\CMakeFiles\Core.dir\depend.make

# Include the progress variables for this target.
include src\Core\CMakeFiles\Core.dir\progress.make

# Include the compile flags for this target's objects.
include src\Core\CMakeFiles\Core.dir\flags.make

src\Core\CMakeFiles\Core.dir\PPC\Gekko.c.obj: src\Core\CMakeFiles\Core.dir\flags.make
src\Core\CMakeFiles\Core.dir\PPC\Gekko.c.obj: ..\src\Core\PPC\Gekko.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/Core/CMakeFiles/Core.dir/PPC/Gekko.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Core.dir\PPC\Gekko.c.obj /FdCMakeFiles\Core.dir\Core.pdb -c D:\CProjects\GC-\src\Core\PPC\Gekko.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\Gekko.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Core.dir/PPC/Gekko.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Core.dir\PPC\Gekko.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Core\PPC\Gekko.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\Gekko.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Core.dir/PPC/Gekko.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Core.dir\PPC\Gekko.c.s /c D:\CProjects\GC-\src\Core\PPC\Gekko.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\MMU.c.obj: src\Core\CMakeFiles\Core.dir\flags.make
src\Core\CMakeFiles\Core.dir\PPC\MMU.c.obj: ..\src\Core\PPC\MMU.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/Core/CMakeFiles/Core.dir/PPC/MMU.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Core.dir\PPC\MMU.c.obj /FdCMakeFiles\Core.dir\Core.pdb -c D:\CProjects\GC-\src\Core\PPC\MMU.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\MMU.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Core.dir/PPC/MMU.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Core.dir\PPC\MMU.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Core\PPC\MMU.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\MMU.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Core.dir/PPC/MMU.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Core.dir\PPC\MMU.c.s /c D:\CProjects\GC-\src\Core\PPC\MMU.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\system.c.obj: src\Core\CMakeFiles\Core.dir\flags.make
src\Core\CMakeFiles\Core.dir\system.c.obj: ..\src\Core\system.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/Core/CMakeFiles/Core.dir/system.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Core.dir\system.c.obj /FdCMakeFiles\Core.dir\Core.pdb -c D:\CProjects\GC-\src\Core\system.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\system.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Core.dir/system.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Core.dir\system.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Core\system.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\system.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Core.dir/system.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Core.dir\system.c.s /c D:\CProjects\GC-\src\Core\system.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\instructions.c.obj: src\Core\CMakeFiles\Core.dir\flags.make
src\Core\CMakeFiles\Core.dir\PPC\instructions.c.obj: ..\src\Core\PPC\instructions.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/Core/CMakeFiles/Core.dir/PPC/instructions.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Core.dir\PPC\instructions.c.obj /FdCMakeFiles\Core.dir\Core.pdb -c D:\CProjects\GC-\src\Core\PPC\instructions.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\instructions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Core.dir/PPC/instructions.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Core.dir\PPC\instructions.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Core\PPC\instructions.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\instructions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Core.dir/PPC/instructions.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Core.dir\PPC\instructions.c.s /c D:\CProjects\GC-\src\Core\PPC\instructions.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\Gekko_init.c.obj: src\Core\CMakeFiles\Core.dir\flags.make
src\Core\CMakeFiles\Core.dir\PPC\Gekko_init.c.obj: ..\src\Core\PPC\Gekko_init.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object src/Core/CMakeFiles/Core.dir/PPC/Gekko_init.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Core.dir\PPC\Gekko_init.c.obj /FdCMakeFiles\Core.dir\Core.pdb -c D:\CProjects\GC-\src\Core\PPC\Gekko_init.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\Gekko_init.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Core.dir/PPC/Gekko_init.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Core.dir\PPC\Gekko_init.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Core\PPC\Gekko_init.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\Gekko_init.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Core.dir/PPC/Gekko_init.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Core.dir\PPC\Gekko_init.c.s /c D:\CProjects\GC-\src\Core\PPC\Gekko_init.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\Breakpoints\breakpoints.c.obj: src\Core\CMakeFiles\Core.dir\flags.make
src\Core\CMakeFiles\Core.dir\Breakpoints\breakpoints.c.obj: ..\src\Core\Breakpoints\breakpoints.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object src/Core/CMakeFiles/Core.dir/Breakpoints/breakpoints.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Core.dir\Breakpoints\breakpoints.c.obj /FdCMakeFiles\Core.dir\Core.pdb -c D:\CProjects\GC-\src\Core\Breakpoints\breakpoints.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\Breakpoints\breakpoints.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Core.dir/Breakpoints/breakpoints.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Core.dir\Breakpoints\breakpoints.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Core\Breakpoints\breakpoints.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\Breakpoints\breakpoints.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Core.dir/Breakpoints/breakpoints.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Core.dir\Breakpoints\breakpoints.c.s /c D:\CProjects\GC-\src\Core\Breakpoints\breakpoints.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\registers\hardware_registers.c.obj: src\Core\CMakeFiles\Core.dir\flags.make
src\Core\CMakeFiles\Core.dir\PPC\registers\hardware_registers.c.obj: ..\src\Core\PPC\registers\hardware_registers.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object src/Core/CMakeFiles/Core.dir/PPC/registers/hardware_registers.c.obj"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\Core.dir\PPC\registers\hardware_registers.c.obj /FdCMakeFiles\Core.dir\Core.pdb -c D:\CProjects\GC-\src\Core\PPC\registers\hardware_registers.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\registers\hardware_registers.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Core.dir/PPC/registers/hardware_registers.c.i"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" > CMakeFiles\Core.dir\PPC\registers\hardware_registers.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\CProjects\GC-\src\Core\PPC\registers\hardware_registers.c
<<
	cd D:\CProjects\GC-\cmake-build-release

src\Core\CMakeFiles\Core.dir\PPC\registers\hardware_registers.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Core.dir/PPC/registers/hardware_registers.c.s"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-cl.exe" @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\Core.dir\PPC\registers\hardware_registers.c.s /c D:\CProjects\GC-\src\Core\PPC\registers\hardware_registers.c
<<
	cd D:\CProjects\GC-\cmake-build-release

# Object files for target Core
Core_OBJECTS = \
"CMakeFiles\Core.dir\PPC\Gekko.c.obj" \
"CMakeFiles\Core.dir\PPC\MMU.c.obj" \
"CMakeFiles\Core.dir\system.c.obj" \
"CMakeFiles\Core.dir\PPC\instructions.c.obj" \
"CMakeFiles\Core.dir\PPC\Gekko_init.c.obj" \
"CMakeFiles\Core.dir\Breakpoints\breakpoints.c.obj" \
"CMakeFiles\Core.dir\PPC\registers\hardware_registers.c.obj"

# External object files for target Core
Core_EXTERNAL_OBJECTS =

src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\PPC\Gekko.c.obj
src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\PPC\MMU.c.obj
src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\system.c.obj
src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\PPC\instructions.c.obj
src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\PPC\Gekko_init.c.obj
src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\Breakpoints\breakpoints.c.obj
src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\PPC\registers\hardware_registers.c.obj
src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\build.make
src\Core\Core.lib: src\Core\CMakeFiles\Core.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\CProjects\GC-\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C static library Core.lib"
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	$(CMAKE_COMMAND) -P CMakeFiles\Core.dir\cmake_clean_target.cmake
	cd D:\CProjects\GC-\cmake-build-release
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	"C:\PROGRA~2\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\lld-link.exe" /lib /nologo /machine:X86 /out:Core.lib @CMakeFiles\Core.dir\objects1.rsp 
	cd D:\CProjects\GC-\cmake-build-release

# Rule to build all files generated by this target.
src\Core\CMakeFiles\Core.dir\build: src\Core\Core.lib

.PHONY : src\Core\CMakeFiles\Core.dir\build

src\Core\CMakeFiles\Core.dir\clean:
	cd D:\CProjects\GC-\cmake-build-release\src\Core
	$(CMAKE_COMMAND) -P CMakeFiles\Core.dir\cmake_clean.cmake
	cd D:\CProjects\GC-\cmake-build-release
.PHONY : src\Core\CMakeFiles\Core.dir\clean

src\Core\CMakeFiles\Core.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\CProjects\GC- D:\CProjects\GC-\src\Core D:\CProjects\GC-\cmake-build-release D:\CProjects\GC-\cmake-build-release\src\Core D:\CProjects\GC-\cmake-build-release\src\Core\CMakeFiles\Core.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src\Core\CMakeFiles\Core.dir\depend

