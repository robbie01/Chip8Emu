#ifndef _CPU_HPP
#define _CPU_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

typedef unsigned char BYTE;

class Chip8_CPU {
    unsigned short opcode;
    BYTE memory[4096];
    BYTE V[16];
    unsigned short I;
    unsigned short pc;
    unsigned short stack[16];
    BYTE sp;
    BYTE gfx[64][32];
    BYTE key[16];
  public:
    BYTE delay_timer;
    BYTE sound_timer;
    void init(void);
    void loadProgram(std::vector<BYTE> game);
    void (*GfxDraw)(const BYTE[64][32]);
    void OnKey(BYTE index);
    void OffKey(BYTE index);
    int doCycle(void);
};
#endif
