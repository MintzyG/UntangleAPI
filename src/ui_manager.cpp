#include "ui_manager.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

UIManager::UIManager() {}

UIManager::~UIManager() {
  if (initialized) shutdown();
}

bool UIManager::initialize(SDL_Window* window, SDL_GLContext gl_context) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  bool success = ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  success &= ImGui_ImplOpenGL3_Init("#version 150");

  if (success) {
    initialized = true;
  }

  return success;
}

void UIManager::shutdown() {
  if (initialized) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    initialized = false;
  }
}

void UIManager::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void UIManager::render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UIManager::processEvent(const SDL_Event& event) {
  return ImGui_ImplSDL2_ProcessEvent(&event);
}
