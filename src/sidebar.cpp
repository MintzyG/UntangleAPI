#include "sidebar.h"

Sidebar::Sidebar() {}

void Sidebar::render() {
  ImGuiIO& io = ImGui::GetIO();

  ImGui::SetNextWindowPos(ImVec2(0,0));
  ImGui::SetNextWindowSize(ImVec2(SIDEBAR_WIDTH, io.DisplaySize.y));

  ImGui::Begin("Sidebar", nullptr,
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoTitleBar);

  ImGui::End();
}
