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

  handleRightClick();

  ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
  ImNodes::EndNodeEditor();

  handleContextMenu();
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

void NodeEditor::handleRightClick() {
  if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    right_clicked_in_editor = true;
    context_menu_pos = ImGui::GetMousePos();
  }
}

void NodeEditor::handleContextMenu() {
  if (right_clicked_in_editor) {
    int hovered_node_id, hovered_link_id, hovered_pin_id;
    bool node_hovered = ImNodes::IsNodeHovered(&hovered_node_id);
    bool link_hovered = ImNodes::IsLinkHovered(&hovered_link_id);
    bool pin_hovered = ImNodes::IsPinHovered(&hovered_pin_id);

    if (!node_hovered && !link_hovered && !pin_hovered) {
      ImGui::OpenPopup("ContextMenu");
    }

    right_clicked_in_editor = false;
  }

  ImGui::SetNextWindowPos(context_menu_pos, ImGuiCond_Appearing);

  if (ImGui::BeginPopup("ContextMenu")) {
    ImGui::Text("Add Node:");
    ImGui::Separator();

    if (ImGui::MenuItem("Start Node")) {
      createNode("Start", context_menu_pos);
    }

    if (ImGui::MenuItem("GET Node")) {
      createNode("GET", context_menu_pos);
    }

    if (ImGui::MenuItem("POST Node")) {
      createNode("POST", context_menu_pos);
    }

    ImGui::EndPopup();
  }
}
