#pragma once
#include "imgui.h"
#include "imnodes.h"
#include <SDL.h>

class App {
public:
  App();
  ~App();

  int run();
private:
  SDL_Window* window = nullptr;
  SDL_GLContext gl_context = nullptr;

  void initSDL();
  void initImGui();
  void cleanup();
};


