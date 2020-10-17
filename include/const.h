#ifndef GC__CONST_H
#define GC__CONST_H

#define BASE_DSP_FILE_PATH "D:\\Data\\CProjects\\GC-\\src\\Core\\HW\\DSP\\files\\"
#define DSP_ROM_PATH BASE_DSP_FILE_PATH "dsp_rom.bin"
#define DSP_COEF_PATH BASE_DSP_FILE_PATH "dsp_coef.bin"


#ifdef _WIN32
#define ROOT_DIR "D:/Data/CProjects/GameCubeResources/Tests/"
#else
#define ROOT_DIR "/mnt/d/"
#endif

// #define TEST_DOL ROOT_DIR "Cube/Cube.dol"
// #define TEST_DOL ROOT_DIR "Textures/Textures.dol"
// #define TEST_DOL ROOT_DIR "Sprites/Sprites.dol"
// #define TEST_DOL ROOT_DIR "Particles/Particles.dol"
// #define TEST_DOL ROOT_DIR "HelloWorld/HelloWorld.dol"
// #define TEST_DOL ROOT_DIR "Console/Console.dol"
#define TEST_DOL ROOT_DIR "AudioPlayer/AudioPlayer.dol"

#endif //GC__CONST_H
