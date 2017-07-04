#pragma once

#ifndef _CPU_HPP
#define _CPU_HPP

#include <array>

using BYTE = std::uint8_t;
using SHORT = std::uint16_t;

template <typename T, std::size_t X, std::size_t Y>
using array2d = std::array<std::array<T, Y>, X>;

typedef struct {
  BYTE delayTimer;
  BYTE soundTimer;
} timers_t;

class Chip8_CPU {
		SHORT opcode;
		std::array<BYTE, 4096> memory;
		std::array<BYTE, 16> V;
		SHORT I;
		SHORT pc;
		std::array<SHORT, 16> stack;
		BYTE sp;
		array2d<BYTE, 64, 32> gfx;
		std::array<BYTE, 16> key;
	public:
		timers_t timers;
		void init();
		void loadProgram(std::vector<BYTE>);
		void (*GfxDraw)(const array2d<BYTE, 64, 32>);
		void OnKey(std::size_t);
		void OffKey(std::size_t);
		int doCycle();
};

#endif
