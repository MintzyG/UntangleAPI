#pragma once
#include "imgui.h"
#include <SDL.h>

class UIManager {
  public:
    UIManager();
    ~UIManager();

    bool initialize(SDL_Window* window, SDL_GLContext gl_context);
    void shutdown();

    void newFrame();
    void render();
    bool processEvent(const SDL_Event& event);
  private:
    bool initialized = false;
};
