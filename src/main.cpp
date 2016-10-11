/*
 * Errors:
 * 0: ok
 * 1: arguments (too many or not enough)
 * 2: SDL
 * 3: RCA 1802 hybrid game
 */

#include <ios>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include "cpu.hpp"

using namespace std;

const int PIXEL_SIZE =  8;
const int SCREEN_WIDTH = 64 * PIXEL_SIZE;
const int SCREEN_HEIGHT =  32 * PIXEL_SIZE;

void logSDLError(void) {
  cerr << "SDL error: " << SDL_GetError() << endl;
}

uint8_t* fileToBytes(string filename) {
  ifstream fl(filename.c_str(), ios_base::in | ios_base::binary);
  fl.seekg(0, ios::end);
  size_t len = fl.tellg();
  vector<uint8_t> retV(len);
  uint8_t* ret = &ret[0];
  fl.seekg(0, ios::beg);
  fl.read((char*)ret, len);
  fl.close();
  return ret;
}

void drawGfx(SDL_Renderer *ren, array<uint8_t, 2048>& gfx) {
  SDL_Rect *square;
  square->w = square->h = PIXEL_SIZE;
  SDL_RenderClear(ren);
  for (int yline = 0; yline < 32; yline++) {
    for (int xline = 0; xline < 64; xline++) {
      if(gfx[xline + (yline * 64)] == 1)
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
      else
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
      square.x = xline * PIXEL_SIZE;
      square.y = yline * PIXEL_SIZE;
      SDL_RenderDrawRect(ren, &square);
    }
  }
  SDL_RenderPresent(ren);
}

int main(int argc, char** argv) {
  #ifndef NDEBUG
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
  #endif
  if (argc != 2) {
    cerr << "Error: Too many or not enough arguments" << endl;
    return 1;
  }
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logSDLError();
    return 2;
  }
  SDL_Window *win = SDL_CreateWindow("CHIP-8 Emulator", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN/*INPUT_GRABBED*/);
  if (win == nullptr) {
    logSDLError();
    SDL_Quit();
    return 2;
  }
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr) {
    SDL_DestroyWindow(win);
    logSDLError();
    SDL_Quit();
    return 2;
  }
  //TODO: input
  uint8_t* pong = fileToBytes(argv[1]);
  Chip8_CPU cpu;
  cpu.init();
  cpu.loadProgram(pong);
  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT: {
          quit = true;
        }
      }
    }
    int ok = cpu.doCycle();
    if (ok == 1) {
      cerr << "Error: RCA 1802 hybrid program." << endl;
      return 3;
    }
    if (cpu.drawFlag) {
      drawGfx(ren, cpu.gfx);
      cpu.drawFlag = false;
    }
    //SDL_Delay(1000/60);
  }
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}

