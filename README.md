# GC-

A hobby GameCube emulator. As of right now, I have not tried to run any commerical games yet,
but I don't suspect they would run yet.

#### Packages required:
 - `SDL2`

#### Optional packages:
 - `capstone[ppc]` (disassembly)
 
#### Included packages:
 - `glad` (openGL bindings)
 - `(dear) imgui` (menus/windows)

#### Memos to self:

required settings:
 - CMake options: `-DCMAKE_TOOLCHAIN_FILE=D:\Programs\vcpkg\scripts\buildsystems\vcpkg.cmake`
 
### Todo:
 - Configure depth test/blend equation based on BP registers.
 - All color modes in fragment.glsl
 - Texture wrapping in fragment.glsl
 - `__builtin_bswap32` for memory reads
 
 
### Components:
 - CPU: Quite a lot of features are implemented. Not all instructions are done yet, I have not encountered all of them.
 - Flipper: A lot is implemented, EFB writes, most draw commands. Still needs lighting/dithering and other color effects. 
   Also, not all color formats have been implemented yet, and other texture effects like wrapping/mipmapping.
 - Hardware IO:
    - AI: Done (?)
    - CP: Mostly done (requires DL calling and indexed XF loading)
    - DSP: Not started (stubbed)
    - DI: Not started
    - EXI: Not started (stubbed)
    - GX_FIFO: Done (have not added disconnected mode)
    - MI: Not started
    - PE: Done (?, still need to look at token interrupts)
    - PI: Done (?)
    - SI: Done (only basic controller is supported)
    - VI: Done (?)