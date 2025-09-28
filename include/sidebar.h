#pragma once

#include "imgui.h"

enum class ViewMode {
  Projects,
  Orchestrations
};

class Sidebar {
  public:
    Sidebar();

    void render();

    int getCurrentProjectId() const { return current_project_id; }
    int getCurrentOrchestrationId() const { return current_orchestration_id; }
    ViewMode getCurrentViewMode() const { return current_view_mode; }

    bool shouldShowNodeEditor() const;

    static constexpr float SIDEBAR_WIDTH = 250.0f;
  private:
    ViewMode current_view_mode = ViewMode::Projects;
    int current_project_id = -1;
    int current_orchestration_id = -1;
};
