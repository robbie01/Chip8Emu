#pragma once

using BYTE = std::uint8_t;
using SHORT= std::uint16_t;

class Chip8_CPU {
    SHORT opcode;
    BYTE memory[4096];
    BYTE V[16];
    SHORT I;
    SHORT pc;
    SHORT stack[16];
    BYTE sp;
    BYTE gfx[64][32];
    BYTE key[16];
  public:
    BYTE delay_timer;
    BYTE sound_timer;
    void init();
    void loadProgram(std::vector<BYTE> game);
    void (*GfxDraw)(const BYTE[64][32]);
    void OnKey(BYTE index);
    void OffKey(BYTE index);
    int doCycle();
};
