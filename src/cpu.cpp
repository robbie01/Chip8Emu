#include <array>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <ctime>
#include <string>
#include <cstring>
#include "cpu.hpp"

constexpr std::array<BYTE, 80> Chip8_FontSet = {
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
	0xF0, 0x80, 0xF0, 0x80, 0x80	// F
};

std::mt19937 mt_rand(time(NULL));

void Chip8_CPU::OnKey(BYTE index) {
	key[index] = 1;
}

void Chip8_CPU::OffKey(BYTE index) {
	key[index] = 0;
}

void Chip8_CPU::init() {
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	std::fill(&gfx[0][0], &gfx[0][0] + sizeof(gfx), 0); //2d array stuff
	std::fill(key.begin(), key.end(), 0);
	std::fill(V.begin(), V.end(), 0);
	std::fill(stack.begin(), stack.end(), 0);
	std::fill(memory.begin(), memory.end(), 0);

	std::copy(Chip8_FontSet.begin(), Chip8_FontSet.end(), memory.begin());

	delay_timer = 0;
	sound_timer = 0;

	GfxDraw(gfx);
}

void Chip8_CPU::loadProgram(std::vector<BYTE> game) {
	for (unsigned long i = 0; i < game.size(); i++)
		memory[i + 512] = game[i];
}

int Chip8_CPU::doCycle() {
	opcode = memory[pc] << 8 | memory[pc + 1];
	switch (opcode & 0xF000) {
		case 0x0000: {
			switch (opcode) {
				case 0x00E0: {
					std::fill(&gfx[0][0], &gfx[0][0] + sizeof(gfx), 0);
					GfxDraw(gfx);
					pc += 2;
					break;
				}
				case 0x00EE: {
					sp--;
					pc = stack[sp];
					pc += 2;
					break;
				}
				default: {
					return 1;
				}
			}
			break;
		}
		case 0x1000: {
			pc = opcode & 0x0FFF;
			break;
		}
		case 0x2000: {
			stack[sp] = pc;
			sp++;
			pc = opcode & 0x0FFF;
			break;
		}
		case 0x3000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			if (V[x] == (opcode & 0x00FF)) pc += 4;
			else pc += 2;
			break;
		}
		case 0x4000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			if (V[x] != (opcode & 0x00FF)) pc += 4;
			else pc += 2;
			break;
		}
		case 0x5000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			BYTE y = (opcode & 0x00F0) >> 4;
			if (V[x] == V[y]) pc += 4;
			else pc += 2;
			break;
		}
		case 0x6000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			V[x] = (opcode & 0x00FF);
			pc += 2;
			break;
		}
		case 0x7000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			V[x] += (opcode & 0x00FF);
			pc += 2;
			break;
		}
		case 0x8000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			BYTE y = (opcode & 0x00F0) >> 4;
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
			BYTE x = (opcode & 0x0F00) >> 8;
			BYTE y = (opcode & 0x00F0) >> 4;
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
			BYTE x = (opcode & 0x0F00) >> 8;
			BYTE val = opcode & 0x00FF;
			V[x] = (mt_rand() % 256) & val;
			pc += 2;
			break;
		}
		case 0xD000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			BYTE y = (opcode & 0x00F0) >> 4;
			BYTE n = (opcode & 0x000F);
			BYTE pixel;
			V[0xF] = 0;
			for (int yline = 0; yline < n; yline++) {
				pixel = memory[I + yline];
				for (int xpix = 0; xpix < 8; xpix++) {
					if ((pixel & (0x80 >> xpix)) != 0) {
						if (gfx[V[x]+xpix][V[y]+yline] == 1) V[0xF] = 1;
						gfx[V[x]+xpix][V[y]+yline] ^= 1;
					}
				}
			}
			GfxDraw(gfx);
			pc += 2;
			break;
		}
		case 0xE000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			switch (opcode & 0x00FF) {
				case 0x9E: {
					if (key[V[x]] != 0) pc += 2;
					break;
				}
				case 0xA1: {
					if (key[V[x]] == 0) pc += 2;
					break;
				}
			}
			pc += 2;
			break;
		}
		case 0xF000: {
			BYTE x = (opcode & 0x0F00) >> 8;
			switch (opcode & 0x00FF) {
				case 0x07: {
					V[x] = delay_timer;
					break;
				}
				case 0x0A: {
					bool pressed = false;
					for (int i = 0; i < 16; i++) {
						if (key[i] != 0) {
							V[x] = i;
							pressed = true;
							break;
						}
					}
					if (!pressed) pc -= 2;
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
	return 0;
}
