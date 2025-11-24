#pragma once
#include "renderer.h"
#include "ui_manager.h"
#include "node_editor.h"
#include "sidebar.h"
#include "project.h"
#include "database.h"
#include <SDL.h>
#include "terminal.h"

class App {
  public:
    App();
    ~App();

    int run();

  private:
    Renderer renderer;
    UIManager ui_manager;
    NodeEditor node_editor;
    ProjectManager project_manager;
    Sidebar sidebar;
    Database database;
    Terminal terminal;
    
    float ui_scale = 1.0f;
    bool sidebar_collapsed = false;
    ImGuiStyle base_style;

    bool initialize();
    void cleanup();
    void handleEvents(bool& done);
    void update();
    void render();
    void saveData();
    void applyUIScale();
    void saveBaseStyle();
};
