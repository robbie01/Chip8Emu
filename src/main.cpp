#include <ios>
#include <map>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include "SDL.h"
#include "cpu.hpp"

const std::map<SDL_Keycode, BYTE> KEYS = {
	{SDLK_1, 1},
	{SDLK_2, 2},
	{SDLK_3, 3},
	{SDLK_4, 0xC},
	{SDLK_q, 4},
	{SDLK_w, 5},
	{SDLK_e, 6},
	{SDLK_r, 0xD},
	{SDLK_a, 7},
	{SDLK_s, 8},
	{SDLK_d, 9},
	{SDLK_f, 0xE},
	{SDLK_z, 0xA},
	{SDLK_x, 0},
	{SDLK_c, 0xB},
	{SDLK_v, 0xF}
};

constexpr int PIXEL_SIZE = 1;
constexpr int SCREEN_WIDTH = 64 * PIXEL_SIZE;
constexpr int SCREEN_HEIGHT =	32 * PIXEL_SIZE;

constexpr int FREQUENCY = 400;

constexpr uint32_t FOREGROUND = 0xFFFFFFFF;
constexpr uint32_t BACKGROUND = 0x000000FF;

bool quit = false;

SDL_Renderer *ren;

void logSDLError() {
	std::cerr << "SDL error: " << SDL_GetError() << std::endl;
}

std::vector<BYTE> fileToBytes(const std::string& filename) {
	std::ifstream fl(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	fl.unsetf(std::ios::skipws);
	std::istream_iterator<BYTE> begin(fl), end;
	std::vector<BYTE> ret(begin, end);
	fl.close();
	return ret;
}

void drawGfx(const array2d<BYTE, 64, 32>& gfx) {
	static SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
	void* pixelsptr;
	int pitch;
	SDL_LockTexture(tex, nullptr, &pixelsptr, &pitch);
	uint32_t* pixels = static_cast<uint32_t*>(pixelsptr);
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 32; y++) {
			pixels[x + y * 64] = gfx[x][y] ? FOREGROUND : BACKGROUND;
		}
	}
	SDL_UnlockTexture(tex);
	SDL_RenderCopy(ren, tex, nullptr, nullptr);
	SDL_RenderPresent(ren);
}

int DecreaseTimers(void* data) {
	timers_t* timers = static_cast<timers_t*>(data);
	while (!quit) {
		if (timers->delayTimer > 0) {
			timers->delayTimer--;
		}
		if (timers->soundTimer > 0) {
			timers->soundTimer--;
		}
		SDL_Delay(1000/60);
	}
	return 0;
}

int main(int argc, char* argv[]) {
#ifndef NDEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
#endif
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <rom>" << std::endl;
		return 1;
	}
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError();
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow("CHIP-8 Emulator", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN/* | INPUT_GRABBED*/);
	if (win == nullptr) {
		logSDLError();
		SDL_Quit();
		return 1;
	}
	ren = SDL_CreateRenderer(win, -1, 0);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		logSDLError();
		SDL_Quit();
		return 1;
	}
	std::vector<BYTE> rom = fileToBytes(argv[1]);
	Chip8_CPU cpu;
	cpu.GfxDraw = drawGfx;
	cpu.init();
	SDL_Thread *timerThread = SDL_CreateThread(
		DecreaseTimers,
		"TimerThread",
		static_cast<void*>(&cpu.timers)
	);
	(void)timerThread;
	cpu.loadProgram(rom);
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: {
					quit = true;
					break;
				}
				case SDL_KEYDOWN: {
					auto key = KEYS.find(e.key.keysym.sym);
					if (key != KEYS.end())
						cpu.OnKey(key->second);
					break;
				}
				case SDL_KEYUP: {
					auto key = KEYS.find(e.key.keysym.sym);
					if (key != KEYS.end())
						cpu.OffKey(key->second);
					break;
				}
			}
		}
		int ok = cpu.doCycle();
		if (ok == 1) {
			std::cerr << "Error: unknown instruction" << std::endl;
			return 1;
		}
		if (cpu.timers.soundTimer > 0) {
			//TODO: buzzer
		}
		SDL_Delay(1000/FREQUENCY);
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

