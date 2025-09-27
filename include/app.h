#pragma once
#include "renderer.h"
#include "ui_manager.h"
#include "node_editor.h"
#include "sidebar.h"
#include "project.h"
#include <SDL.h>

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

    bool initialize();
    void cleanup();
    void handleEvents(bool& done);
    void update();
    void render();
};
