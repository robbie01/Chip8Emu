#include "cpu.hpp"

using namespace std;

const array<uint8_t, 80> Chip8_FontSet = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8_CPU::init(void) {
  srand(time(NULL));

  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;

  gfx.fill(0);
  stack.fill(0);
  V.fill(0);
  memory.fill(0);

  for (int i = 0; i < 80; ++i)
    memory[i] = Chip8_FontSet[i];
}

void Chip8_CPU::loadProgram(uint8_t* game) {
  for (unsigned long i = 0; i < (sizeof(game)/sizeof(game[0])); ++i)
    memory[i + 512] = game[i];
}

int Chip8_CPU::doCycle(void) {
  opcode = memory[pc] << 8 | memory[pc + 1];
  switch (opcode & 0xF000) {
    case 0x0000:
      switch (opcode & 0x00FF) {
        case 0x00E0: {
          gfx.fill(0);
          drawFlag = true;
          pc += 2;
          break;
        }
        case 0x00EE: {
          --sp;
          pc = stack[sp];
          pc += 2;
          break;
        }/*
        default: {
          return 1;
        }*/
      }
    case 0x1000: {
      pc = opcode & 0x0FFF;
      break;
    }
    case 0x2000: {
      stack[sp] = pc;
      ++sp;
      pc = opcode & 0x0FFF;
      break;
    }
    case 0x3000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      if (V[x] == (opcode & 0x00FF)) pc += 4;
      break;
    }
    case 0x4000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      if (V[x] != (opcode & 0x00FF)) pc += 4;
      else pc += 2;
      break;
    }
    case 0x5000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x00F0) >> 4;
      if (V[x] == V[y]) pc += 4;
      else pc += 2;
      break;
    }
    case 0x6000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      V[x] = (opcode & 0x00FF);
      pc += 2;
      break;
    }
    case 0x7000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      V[x] += (opcode & 0x00FF);
      pc += 2;
      break;
    }
    case 0x8000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x000F) >> 4;
      switch (opcode & 0x000F) {
        case 0: {
          V[x] = V[y];
          break;
        }
        case 1: {
          V[x] |= V[y];
          break;
        }
        case 2: {
          V[x] &= V[y];
          break;
        }
        case 3: {
          V[x] ^= V[y];
          break;
        }
        case 4: {
          if (V[y] > (0xFF - V[x])) V[0xF] = 1;
          else V[0xF] = 0;
          V[x] += V[y];
          break;
        }
        case 5: {
          if (V[y] > V[x]) V[0xF] = 0;
          else V[0xF] = 1;
          V[x] -= V[y];
          break;
        }
        case 6: {
          V[0xF] = V[x] & 1;
          V[x] >>= 1;
          break;
        }
        case 7: {
          if (V[x] > V[y]) V[0xF] = 0;
          else V[0xF] = 1;
          V[x] = V[y] - V[x];
          break;
        }
        case 0xE: {
          V[0xF] = V[x] & 128;
          V[x] <<= 1;
          break;
        }
      }
      pc += 2;
      break;
    }
    case 0x9000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x00F0) >> 4;
      if (V[x] != V[y]) pc += 4;
      else pc += 2;
      break;
    }
    case 0xA000: {
      I = opcode & 0x0FFF;
      pc += 2;
      break;
    }
    case 0xB000: {
      pc = (opcode & 0x0FFF) + V[0];
      break;
    }
    case 0xC000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t val = opcode & 0x00FF;
      V[x] = (rand() % 256) & val;
      pc += 2;
      break;
    }
    case 0xD000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x00F0) >> 4;
      uint8_t h = (opcode & 0x000F);
      uint8_t pixel;
      V[0xF] = 0;
      for (int yline = 0; yline < h; yline++) {
        pixel = memory[I + yline];
        for(int xline = 0; xline < 8; xline++) {
          if((pixel & (0x80 >> xline)) != 0) {
            if(gfx[(x + xline + ((y + yline) * 64))] == 1) V[0xF] = 1;
            gfx[x + xline + ((y + yline) * 64)] ^= 1;
          }
        }
      }
      drawFlag = true;
      pc += 2;
      break;
    }
    case 0xE000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      switch (opcode & 0x00FF) {
        case 0x9E: {
          //TODO: implement
          break;
        }
        case 0xA1: {
          //TODO: implement
          break;
        }
      }
      pc += 2;
      break;
    }
    case 0xF000: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      switch (opcode & 0x00FF) {
        case 0x07: {
          V[x] = delay_timer;
          break;
        }
        case 0x0A: {
          //TODO: implement
          break;
        }
        case 0x15: {
          delay_timer = V[x];
          break;
        }
        case 0x18: {
          sound_timer = V[x];
          break;
        }
        case 0x1E: {
          I += V[x];
          break;
        }
        case 0x29: {
          I = V[x] * 5;
          break;
        }
        case 0x33: {
          memory[I] = V[x] / 100;
          memory[I+1] = (V[x] / 10) % 10;
          memory[I+2] = (V[x] % 100) % 10;
          break;
        }
        case 0x55: {
          for (int i = 0; i <= x; ++i) memory[I+i] = V[i];
          break;
        }
        case 0x65: {
          for (int i = 0; i <= x; ++i) V[i] = memory[I+i];
          break;
        }
      }
      pc += 2;
      break;
    }
  }
  if (delay_timer > 0) --delay_timer;
  if (sound_timer > 0) --sound_timer;
  return 0;
}
