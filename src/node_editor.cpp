#include "node_editor.h"
#include "nodes.h"
#include <cstring>

NodeEditor::NodeEditor() {}

NodeEditor::~NodeEditor() {
  if (initialized) shutdown();
}

bool NodeEditor::initialize() {
  ImNodes::CreateContext();
  initialized = true;
  createNode("Start", ImVec2(0,0));
  createNode("GET", ImVec2(0,0));
  createNode("POST", ImVec2(0,0));
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

  drawNodes();

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

void NodeEditor::createNode(const std::string& nodeType, ImVec2 position) {
  std::unique_ptr<Node> newNode;
  
  if (nodeType == "Start") {
    newNode = std::make_unique<StartNode>(next_node_id);
  } else if (nodeType == "GET") {
    newNode = std::make_unique<GetNode>(next_node_id);
  } else if (nodeType == "POST") {
    newNode = std::make_unique<PostNode>(next_node_id);
  }
  
  if (newNode) {
    ImNodes::SetNodeGridSpacePos(next_node_id, position);
    
    nodes.push_back(std::move(newNode));
    next_node_id += 10;
  }
}

void NodeEditor::drawNodes() const {
  for (auto const& n : nodes) {
    n->draw();
  }
}
