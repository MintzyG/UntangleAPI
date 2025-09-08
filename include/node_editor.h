#pragma once
#include "imgui.h"
#include "imnodes.h"

class NodeEditor {
  public:
    NodeEditor();
    ~NodeEditor();

    bool initialize();
    void shutdown();
    void render();

    void showEditor(const char* window_title);

  private:
    bool initialized = false;
};
