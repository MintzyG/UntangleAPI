#pragma once
#include <SDL.h>
#include <GL/gl.h>

class Renderer {
  public:
    Renderer();
    ~Renderer();

    bool initialize();
    void beginFrame();
    void endFrame();
    void clear(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 1.0f);

    SDL_Window* getWindow() const { return window; }
    SDL_GLContext getGLContext() const { return gl_context; }

  private:
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;
};;
