#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

typedef unsigned char BYTE;

using namespace std;

class Chip8_CPU {
    unsigned short opcode;
    BYTE memory[4096];
    BYTE V[16];
    unsigned short I;
    unsigned short pc;
    BYTE delay_timer;
    BYTE sound_timer;
    unsigned short stack[16];
    BYTE sp;
  public:
    BYTE gfx[64][32];
    bool drawFlag = false;
    BYTE key[16];
    void init(void);
    void loadProgram(vector<BYTE> game);
    int doCycle(void);
};
#endif
