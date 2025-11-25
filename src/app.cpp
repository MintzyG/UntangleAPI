#include "app.h"
#include <stdio.h>

App::App() : sidebar(project_manager) {}

App::~App() {
  cleanup();
}

void App::saveBaseStyle() {
  base_style = ImGui::GetStyle();
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

  saveBaseStyle();

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

void App::applyUIScale() {
  ImGuiStyle& style = ImGui::GetStyle();
  ImGuiIO& io = ImGui::GetIO();
  
  style = base_style;
  style.ScaleAllSizes(ui_scale);
  
  io.FontGlobalScale = ui_scale;
  
  printf("Applied UI scale: %.1fx\n", ui_scale);
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
      bool ctrl_pressed = (event.key.keysym.mod & KMOD_CTRL);
      
      if (event.key.keysym.sym == SDLK_s && ctrl_pressed) {
        printf("Manual save triggered (Ctrl+S)\n");
        saveData();
      }
      
      if ((event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_EQUALS) && ctrl_pressed) {
        float old_scale = ui_scale;
        ui_scale += 0.1f;
        if (ui_scale > 2.0f) {
          ui_scale = 2.0f;
          printf("Maximum zoom reached: %.1fx\n", ui_scale);
        } else {
          applyUIScale();
          printf("Zoom in: %.1fx (was %.1fx)\n", ui_scale, old_scale);
        }
      }
      
      if (event.key.keysym.sym == SDLK_MINUS && ctrl_pressed) {
        float old_scale = ui_scale;
        ui_scale -= 0.1f;
        if (ui_scale < 0.5f) {
          ui_scale = 0.5f;
          printf("Minimum zoom reached: %.1fx\n", ui_scale);
        } else {
          applyUIScale();
          printf("Zoom out: %.1fx (was %.1fx)\n", ui_scale, old_scale);
        }
      }
      
      if (event.key.keysym.sym == SDLK_0 && ctrl_pressed) {
        ui_scale = 1.0f;
        applyUIScale();
        printf("Zoom reset: 1.0x\n");
      }
      
      if (event.key.keysym.sym == SDLK_b && ctrl_pressed) {
        sidebar_collapsed = !sidebar_collapsed;
        printf("Sidebar %s\n", sidebar_collapsed ? "collapsed" : "expanded");
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
  
  float sidebar_width = sidebar_collapsed ? 0.0f : Sidebar::SIDEBAR_WIDTH;
  
  if (!sidebar_collapsed) {
    sidebar.render();
  }

  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(sidebar_width, 0));
  ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - sidebar_width, io.DisplaySize.y));

  ImGui::Begin("Main Content", nullptr,
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoTitleBar);

  if (sidebar_collapsed) {
    ImGui::SetCursorPos(ImVec2(10, 10));
    if (ImGui::Button("☰", ImVec2(40, 30))) {
      sidebar_collapsed = false;
    }
    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("Show Sidebar (Ctrl+B)");
    }
  }

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
    float available_height = ImGui::GetContentRegionAvail().y - 40; // Reserve space for bottom bar
    float terminal_height = terminal.isVisible() ? terminal.getHeight() : 0;
    float node_editor_height = available_height - terminal_height;
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 120, 10));
    if (terminal.isVisible()) {
      if (ImGui::Button("Hide Terminal", ImVec2(110, 25))) {
        terminal.toggleVisible();
      }
    } else {
      if (ImGui::Button("Show Terminal", ImVec2(110, 25))) {
        terminal.toggleVisible();
      }
    }
    
    ImGui::BeginChild("NodeEditorArea", ImVec2(0, node_editor_height), false, ImGuiWindowFlags_NoScrollbar);
    node_editor.render(sidebar, &terminal);
    ImGui::EndChild();
    
    if (terminal.isVisible()) {
      terminal.render(available_height);
    }
  }

  ImGui::SetCursorPos(ImVec2(10, ImGui::GetWindowSize().y - 30));
  ImGui::TextDisabled("Ctrl+S: Save | Ctrl+B: Toggle Sidebar | Ctrl+/0: Zoom (%.1fx)", ui_scale);

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
