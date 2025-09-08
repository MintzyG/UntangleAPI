#include "renderer.h"
#include "imgui.h"
#include <stdio.h>

Renderer::Renderer() {}

Renderer::~Renderer() {
  if (gl_context) SDL_GL_DeleteContext(gl_context);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
}

bool Renderer::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    return false;
  }

  window = SDL_CreateWindow("Untangle API",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    printf("Error creating window: %s\n", SDL_GetError());
    return false;
  }

  gl_context = SDL_GL_CreateContext(window);
  if (!gl_context) {
    printf("Error creating GL context: %s\n", SDL_GetError());
    return false;
  }

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1);

  return true;
}

void Renderer::beginFrame() {
  ImGuiIO& io = ImGui::GetIO();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
}

void Renderer::endFrame() {
  SDL_GL_SwapWindow(window);
}

void Renderer::clear(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}
