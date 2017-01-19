#include <ios>
#include <string>
#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <SDL.h>
#include "cpu.hpp"

typedef unsigned char BYTE;

using namespace std;

const int PIXEL_SIZE =  8;
const int SCREEN_WIDTH = 64 * PIXEL_SIZE;
const int SCREEN_HEIGHT =  32 * PIXEL_SIZE;

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
  return ret;
}

void drawGfx(SDL_Surface *sfc, const BYTE gfx[64][32]) {
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
}

int main(int argc, char* argv[]) {
  #ifndef NDEBUG
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
  #endif
  if (argc != 2) {
    cerr << "Error: Too many or not enough arguments" << endl;
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
  SDL_Surface *sfc = SDL_GetWindowSurface(win);
  if (sfc == nullptr) {
    SDL_DestroyWindow(win);
    logSDLError();
    SDL_Quit();
    return 1;
  }
  //TODO: input
  vector<BYTE> pong = fileToBytes(argv[1]);
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
      cerr << "Error: unknown instruction" << endl;
      return 1;
    }
    if (cpu.drawFlag) {
      drawGfx(sfc, cpu.gfx);
      SDL_UpdateWindowSurface(win);
      cpu.drawFlag = false;
    }
    //SDL_Delay(1000/60);
  }
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}

