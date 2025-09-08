#include "node_editor.h"
#include <cstring>

NodeEditor::NodeEditor() {}

NodeEditor::~NodeEditor() {
  if (initialized) shutdown();
}

bool NodeEditor::initialize() {
  ImNodes::CreateContext();
  initialized = true;
  return true;
}

void NodeEditor::shutdown() {
  if (initialized) {
    ImNodes::DestroyContext();
    initialized = false;
  }
}

void NodeEditor::render() {
  ImNodes::BeginNodeEditor();

  // Node Editor Logic

  ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
  ImNodes::EndNodeEditor();
}

void NodeEditor::showEditor(const char* window_title) {
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::SetNextWindowPos(ImVec2(0,0));
  ImGui::SetNextWindowSize(io.DisplaySize);
  ImGui::Begin(window_title, nullptr,
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize   |
      ImGuiWindowFlags_NoMove     |
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoBringToFrontOnFocus);
  render();
  ImGui::End();
}
