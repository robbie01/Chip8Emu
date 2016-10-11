#ifndef CPU_HPP
#define CPU_HPP

#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>

using namespace std;

class Chip8_CPU {
    uint16_t opcode;
    array<uint8_t, 4096> memory;
    array<uint8_t, 16> V;
    uint16_t I;
    uint16_t pc;
    uint8_t delay_timer;
    uint8_t sound_timer;
    array<uint16_t, 16> stack;
    uint8_t sp;
  public:
    array<uint8_t, 2048> gfx;
    bool drawFlag = false;
    array<uint8_t, 16> key;
    void init(void);
    void loadProgram(uint8_t* game);
    int doCycle(void);
};
#endif
