#include "app.h"
#include <stdio.h>

App::App() : sidebar(project_manager) {}

App::~App() {
  cleanup();
}

bool App::initialize() {
  if (!renderer.initialize()) {
    printf("Failed to initialize renderer\n");
    return false;
  }

  if (!ui_manager.initialize(renderer.getWindow(), renderer.getGLContext())) {
    printf("Failed to initialize UI manager\n");
    return false;
  }

  if (!node_editor.initialize()) {
    printf("Failed to initialize node editor\n");
    return false;
  }

  if (!database.initialize("untangle.db")) {
    printf("Failed to initialize database\n");
    return false;
  }

  printf("Loading data from database...\n");
  if (!database.loadAll(project_manager, node_editor)) {
    printf("No existing data found or failed to load, starting fresh\n");
    
    if (project_manager.getProjects().empty()) {
      project_manager.addProject("Test project 1");
      auto p = project_manager.getProject(1);
      if (p) {
        p->addOrchestration("Get Users");
      }
    }
  }

  return true;
}

void App::cleanup() {
  printf("Saving data before exit...\n");
  saveData();
  
  node_editor.shutdown();
  ui_manager.shutdown();
  database.close();
}

void App::saveData() {
  if (!database.saveAll(project_manager, node_editor)) {
    printf("Failed to save data!\n");
  }
}

void App::handleEvents(bool& done) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ui_manager.processEvent(event);

    if (event.type == SDL_QUIT) {
      done = true;
    }

    if (event.type == SDL_WINDOWEVENT) {
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        done = true;
      }
    }

    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_s && (event.key.keysym.mod & KMOD_CTRL)) {
        printf("Manual save triggered (Ctrl+S)\n");
        saveData();
      }
    }
  }
}

void App::update() {
  // update application state
}

void App::render() {
  renderer.beginFrame();
  renderer.clear();

  ui_manager.newFrame();
  sidebar.render();

  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(Sidebar::SIDEBAR_WIDTH, 0));
  ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - Sidebar::SIDEBAR_WIDTH, io.DisplaySize.y));

  ImGui::Begin("Main Content", nullptr,
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoTitleBar);

  if (!sidebar.shouldShowNodeEditor()) {
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 text_size;

    if (sidebar.getCurrentViewMode() == ViewMode::Projects || sidebar.getCurrentProjectId() == -1) {
      const char* message = "Open a project to start";
      text_size = ImGui::CalcTextSize(message);
      ImGui::SetCursorPos(ImVec2((window_size.x - text_size.x) * 0.5f, (window_size.y - text_size.y) * 0.5f));
      ImGui::Text("%s", message);
    } else {
      const char* message = "Select or Create an orchestration to start";
      text_size = ImGui::CalcTextSize(message);
      ImGui::SetCursorPos(ImVec2((window_size.x - text_size.x) * 0.5f, (window_size.y - text_size.y) * 0.5f));
      ImGui::Text("%s", message);
    }
  } else {
    node_editor.render(sidebar);
  }

  ImGui::SetCursorPos(ImVec2(10, ImGui::GetWindowSize().y - 30));
  ImGui::TextDisabled("Press Ctrl+S to save");

  ImGui::End();

  ui_manager.render();
  renderer.endFrame();
}

int App::run() {
  if (!initialize()) {
    printf("Failed to initialize application\n");
    return -1;
  }

  bool done = false;

  while (!done) {
    handleEvents(done);
    update();
    render();
  }

  return 0;
}
