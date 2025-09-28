#include "node_editor.h"
#include "nodes.h"
#include "link.h"
#include "sidebar.h"
#include <cstring>
#include <algorithm>
#include <memory>

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

void NodeEditor::render(const Sidebar& sidebar) {
  if (!sidebar.shouldShowNodeEditor()) {
    return;
  }

  int orchestration_id = sidebar.getCurrentOrchestrationId();

  ImNodes::BeginNodeEditor();

  drawNodes();
  drawLinks();

  handleRightClick();

  ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
  ImNodes::EndNodeEditor();

  createLinks();
  handleContextMenu();
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

void NodeEditor::createLinks() {
  int start_attr, end_attr;
  if (ImNodes::IsLinkCreated(&start_attr, &end_attr)) {
    links.push_back(std::make_unique<Link>(next_link_id++, start_attr, end_attr));
  }
}

void NodeEditor::drawLinks() const {
  for (const auto& link : links) {
    ImNodes::Link(link->id, link->start_attr, link->end_attr);
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

void NodeEditor::deleteLinks() {
  if (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
    std::vector<int> selected_links;
    selected_links.resize(ImNodes::NumSelectedLinks());
    ImNodes::GetSelectedLinks(selected_links.data());

    for (int selected_id : selected_links) {
      links.erase(
          std::remove_if(links.begin(), links.end(),
            [selected_id](const std::unique_ptr<Link>& l) {
            return l->id == selected_id;
            }),
          links.end()
          );
    }
  }

  int link_id;
  if (ImNodes::IsLinkDestroyed(&link_id)) {
    links.erase(
        std::remove_if(links.begin(), links.end(),
          [link_id](const std::unique_ptr<Link>& l) {
          return l->id == link_id;
          }),
        links.end()
        );
  }
}

void NodeEditor::deleteNodes() {
  if (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
    nodes.erase(
        std::remove_if(nodes.begin(), nodes.end(),
          [&](const std::unique_ptr<Node>& n) {
          if (ImNodes::IsNodeSelected(n->getId())) {
          auto nodeAttributes = n->getAttributeIds();
          links.erase(
              std::remove_if(links.begin(), links.end(),
                [&](const std::unique_ptr<Link>& l) {
                return std::find(nodeAttributes.begin(), nodeAttributes.end(), l->start_attr) != nodeAttributes.end() ||
                std::find(nodeAttributes.begin(), nodeAttributes.end(), l->end_attr) != nodeAttributes.end();
                }),
              links.end()
              );
          return true;
          }
          return false;
          }),
        nodes.end()
        );
  }
}
