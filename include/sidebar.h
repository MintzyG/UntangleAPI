#pragma once

#include "project.h"
#include "imgui.h"

enum class ViewMode {
  Projects,
  Orchestrations
};

class Sidebar {
  public:
    Sidebar(ProjectManager& project_manager);

    void render();

    int getCurrentProjectId() const { return current_project_id; }
    int getCurrentOrchestrationId() const { return current_orchestration_id; }
    ViewMode getCurrentViewMode() const { return current_view_mode; }

    bool shouldShowNodeEditor() const;

    static constexpr float SIDEBAR_WIDTH = 250.0f;

  private:
    ProjectManager& project_manager;
    ViewMode current_view_mode = ViewMode::Projects;
    int current_project_id = -1;
    int current_orchestration_id = -1;

    char new_project_name[256] = "";
    char new_orchestration_name[256] = "";
    bool show_create_project_popup = false;
    bool show_create_orchestration_popup = false;

    void renderProjectsView();
    void renderOrchestrationsView();
    void renderCreateProjectPopup();
    void renderCreateOrchestrationPopup();
};
