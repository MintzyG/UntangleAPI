#include "sidebar.h"
#include <cstring>

Sidebar::Sidebar(ProjectManager& project_manager)
  : project_manager(project_manager) {}

void Sidebar::render() {
  ImGuiIO& io = ImGui::GetIO();

  ImGui::SetNextWindowPos(ImVec2(0,0));
  ImGui::SetNextWindowSize(ImVec2(SIDEBAR_WIDTH, io.DisplaySize.y));

  ImGui::Begin("Sidebar", nullptr,
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoTitleBar);

  if (current_view_mode == ViewMode::Projects) {
    renderProjectsView();
  } else {
    renderOrchestrationsView();
  }

  ImGui::End();

  renderCreateProjectPopup();
  renderCreateOrchestrationPopup();
}

bool Sidebar::shouldShowNodeEditor() const {
  return current_view_mode == ViewMode::Orchestrations &&
    current_project_id != -1 &&
    current_orchestration_id != -1;
}

void Sidebar::renderProjectsView() {
  ImGui::Text("Projects");
  ImGui::Separator();

  if (ImGui::Button("Create New Project", ImVec2(-1, 0))) {
    show_create_project_popup = true;
    strcpy(new_project_name, "");
  }

  ImGui::Spacing();

  for (const auto& project : project_manager.getProjects()) {
    ImGui::PushID(project->id);

    float button_width = 20.0f;
    float available_width = ImGui::GetContentRegionAvail().x;

    if (ImGui::Selectable(project->name.c_str(), current_project_id == project->id, 0 ,
          ImVec2(available_width - button_width - ImGui::GetStyle().ItemSpacing.x, 0 ))) {
      current_project_id = project->id;
      current_orchestration_id = -1;
      current_view_mode = ViewMode::Orchestrations;
    }

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    if (ImGui::Button("X", ImVec2(button_width, 0))) { }
    ImGui::PopStyleColor();
    ImGui::PopID();
  }
}

void Sidebar::renderOrchestrationsView() {
  Project* current_project = project_manager.getProject(current_project_id);
  if (!current_project) return;

  if (ImGui::Button("<- Back to Projetcs")) {
    current_view_mode = ViewMode::Projects;
    current_project_id = -1;
    current_orchestration_id = -1;
    return;
  }

  ImGui::Spacing();
  ImGui::Text("Project: %s", current_project->name.c_str());
  ImGui::Separator();

  ImGui::Text("Orchestrations");
  ImGui::Separator();

  if (ImGui::Button("Create Orchestration", ImVec2(-1, 0))) {
    show_create_orchestration_popup = true;
    strcpy(new_orchestration_name, "");
  }

  ImGui::Spacing();

  for (const auto& orchestration : current_project->orchestrations) {
    ImGui::PushID(orchestration->id);

    float button_width = 20.0f;
    float available_width = ImGui::GetContentRegionAvail().x;

    bool is_selected = current_orchestration_id == orchestration->id;

    if (ImGui::Selectable(orchestration->name.c_str(), is_selected, 0 ,
          ImVec2(available_width - button_width - ImGui::GetStyle().ItemSpacing.x, 0 ))) {
      current_orchestration_id = orchestration->id;
    }

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    if (ImGui::Button("X", ImVec2(button_width, 0))) { }
    ImGui::PopStyleColor();
    ImGui::PopID();
  }
}

void Sidebar::renderCreateProjectPopup() {
  if (show_create_project_popup) {
    ImGui::OpenPopup("Create Project");
    show_create_project_popup = false;
  }

  if (ImGui::BeginPopupModal("Create Project", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Project Name:");
    ImGui::InputText("##project_name", new_project_name, sizeof(new_project_name));

    ImGui::Spacing();

    if (ImGui::Button("Create")) {
      if (strlen(new_project_name) > 0) {
        project_manager.addProject(new_project_name);
        ImGui::CloseCurrentPopup();
      }
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void Sidebar::renderCreateOrchestrationPopup() {
  if (show_create_orchestration_popup) {
    ImGui::OpenPopup("Create Orchestration");
    show_create_orchestration_popup = false;
  }

  if (ImGui::BeginPopupModal("Create Orchestration", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Orchestration Name:");
    ImGui::InputText("##orchestration_name", new_orchestration_name, sizeof(new_orchestration_name));

    ImGui::Spacing();

    if (ImGui::Button("Create")) {
      if (strlen(new_orchestration_name) > 0) {
        Project* current_project = project_manager.getProject(current_project_id);
        if (current_project) {
          current_project->addOrchestration(new_orchestration_name);
        }
        ImGui::CloseCurrentPopup();
      }
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}
