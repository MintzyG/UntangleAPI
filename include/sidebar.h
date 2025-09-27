#pragma once

#include "imgui.h"

class Sidebar {
  public:
    Sidebar();

    void render();
    static constexpr float SIDEBAR_WIDTH = 250.0f;
};
