#include <ios>
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include "SDL.h"
#include "cpu.hpp"

typedef unsigned char BYTE;

using namespace std;

const map<SDL_Keycode, BYTE> KEYS = { {SDLK_1, 1},
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
                                 {SDLK_v, 0xF} };

const int PIXEL_SIZE =  8;
const int SCREEN_WIDTH = 64 * PIXEL_SIZE;
const int SCREEN_HEIGHT =  32 * PIXEL_SIZE;

const int FREQUENCY = 400;

bool quit = false;

SDL_Window *win;
SDL_Surface *sfc;

void logSDLError(void) {
  cerr << "SDL error: " << SDL_GetError() << endl;
}

vector<BYTE> fileToBytes(char const* filename) {
  ifstream fl(filename, ios_base::in | ios_base::binary);
  fl.unsetf(ios::skipws);
  fl.seekg(0, ios::end);
  ios::streampos pos = fl.tellg();
  fl.seekg(0, ios::beg);
  vector<BYTE> ret;
  ret.reserve(pos);
  ret.insert(ret.begin(),
             istream_iterator<BYTE>(fl),
             istream_iterator<BYTE>());
  fl.close();
  return ret;
}

void drawGfx(const BYTE gfx[64][32]) {
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 32; y++) {
      SDL_Rect square = {x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
      if(gfx[x][y] == 1) {
        SDL_FillRect(sfc, &square, SDL_MapRGB(sfc->format, 0xFF, 0xFF, 0xFF));
      }
      else {
        SDL_FillRect(sfc, &square, SDL_MapRGB(sfc->format, 0x00, 0x00, 0x00));
      }
    }
  }
  SDL_UpdateWindowSurface(win);
}

int DecreaseTimers(void* data) {
  BYTE* timers[2];
  memcpy(timers, (BYTE**)data, sizeof timers);
  while (!quit) {
    for (int i = 0; i < 2; i++) {
      if (*timers[i] > 0) {
        (*timers[i])--;
      }
      SDL_Delay(1000/60);
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  #ifndef NDEBUG
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
  #endif
  if (argc != 2) {
    cerr << "Usage: Chip8Emu <rom>" << endl;
    return 1;
  }
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logSDLError();
    return 1;
  }
  win = SDL_CreateWindow("CHIP-8 Emulator", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN/* | INPUT_GRABBED*/);
  if (win == nullptr) {
    logSDLError();
    SDL_Quit();
    return 1;
  }
  sfc = SDL_GetWindowSurface(win);
  if (sfc == nullptr) {
    SDL_DestroyWindow(win);
    logSDLError();
    SDL_Quit();
    return 1;
  }
  vector<BYTE> pong = fileToBytes(argv[1]);
  Chip8_CPU cpu;
  cpu.GfxDraw = drawGfx;
  cpu.init();
  BYTE* timers[] = {&cpu.delay_timer, &cpu.sound_timer};
  SDL_Thread *timerThread = SDL_CreateThread(DecreaseTimers, "TimerThread", timers);
  cpu.loadProgram(pong);
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
            cpu.OnKey(key->first);
          break;
        }
        case SDL_KEYUP: {
          auto key = KEYS.find(e.key.keysym.sym);
          if (key != KEYS.end())
            cpu.OffKey(key->first);
          break;
        }
      }
    }
    int ok = cpu.doCycle();
    if (ok == 1) {
      cerr << "Error: unknown instruction" << endl;
      return 1;
    }
    if (cpu.sound_timer > 0) {
      //TODO: buzzer
    }
    SDL_Delay(1000/FREQUENCY);
  }
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}

