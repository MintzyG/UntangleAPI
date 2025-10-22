#include "node_editor.h"
#include "nodes.h"
#include "link.h"
#include "sidebar.h"
#include "database.h"
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
  return true;
}

void NodeEditor::shutdown() {
  if (initialized) {
    ImNodes::DestroyContext();
    initialized = false;
  }
}

OrchestrationData& NodeEditor::getOrchestrationData(int orchestration_id) {
  auto it = orchestration_data.find(orchestration_id);
  if (it == orchestration_data.end()) {
    auto data = std::make_unique<OrchestrationData>();
    
    OrchestrationData* data_ptr = data.get();
    orchestration_data[orchestration_id] = std::move(data);
    return *data_ptr;
  } else {
    for(auto& node : it->second->nodes) {
      ImNodes::SetNodeGridSpacePos(node->getId(), node->getPosition());
    }
  }
  return *it->second;
}

void NodeEditor::render(const Sidebar& sidebar) {
  if (!sidebar.shouldShowNodeEditor()) {
    return;
  }

  int orchestration_id = sidebar.getCurrentOrchestrationId();
  OrchestrationData& data = getOrchestrationData(orchestration_id);

  ImNodes::BeginNodeEditor();

  drawNodes(data);
  drawLinks(data);

  handleRightClick();

  ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
  ImNodes::EndNodeEditor();

  for(auto& node : data.nodes) {
    node->updatePosition();
  }

  createLinks(data);
  handleContextMenu(data);
  deleteNodes(data);
  deleteLinks(data);
}

void NodeEditor::createNode(const std::string& nodeType, ImVec2 position, OrchestrationData& data) {
  std::unique_ptr<Node> newNode;

  if (nodeType == "Start") {
    newNode = std::make_unique<StartNode>(data.next_node_id);
  } else if (nodeType == "GET") {
    newNode = std::make_unique<GetNode>(data.next_node_id);
  } else if (nodeType == "POST") {
    newNode = std::make_unique<PostNode>(data.next_node_id);
  }

  if (newNode) {
    newNode->setPosition(position);

    data.nodes.push_back(std::move(newNode));
    data.next_node_id += 10;
  }
}

void NodeEditor::createNodeWithId(int node_id, const std::string& nodeType, ImVec2 position, OrchestrationData& data) {
  std::unique_ptr<Node> newNode;

  if (nodeType == "Start") {
    newNode = std::make_unique<StartNode>(node_id);
  } else if (nodeType == "GET") {
    newNode = std::make_unique<GetNode>(node_id);
  } else if (nodeType == "POST") {
    newNode = std::make_unique<PostNode>(node_id);
  }

  if (newNode) {
    newNode->setPosition(position);
    data.nodes.push_back(std::move(newNode));
    
    if (node_id >= data.next_node_id) {
      data.next_node_id = node_id + 10;
    }
  }
}

void NodeEditor::drawNodes(const OrchestrationData& data) const {
  for (auto const& n : data.nodes) {
    n->draw();
  }
}

void NodeEditor::createLinks(OrchestrationData& data) {
  int start_attr, end_attr;
  if (ImNodes::IsLinkCreated(&start_attr, &end_attr)) {
    data.links.push_back(std::make_unique<Link>(data.next_link_id++, start_attr, end_attr));
  }
}

void NodeEditor::drawLinks(const OrchestrationData& data) const {
  for (const auto& link : data.links) {
    ImNodes::Link(link->id, link->start_attr, link->end_attr);
  }
}

void NodeEditor::handleRightClick() {
  if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    right_clicked_in_editor = true;
    context_menu_pos = ImGui::GetMousePos();
  }
}

void NodeEditor::handleContextMenu(OrchestrationData& data) {
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
      createNode("Start", context_menu_pos, data);
    }

    if (ImGui::MenuItem("GET Node")) {
      createNode("GET", context_menu_pos, data);
    }

    if (ImGui::MenuItem("POST Node")) {
      createNode("POST", context_menu_pos, data);
    }

    ImGui::EndPopup();
  }
}

void NodeEditor::deleteLinks(OrchestrationData& data) {
  if (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
    std::vector<int> selected_links;
    selected_links.resize(ImNodes::NumSelectedLinks());
    ImNodes::GetSelectedLinks(selected_links.data());

    for (int selected_id : selected_links) {
      data.links.erase(
          std::remove_if(data.links.begin(), data.links.end(),
            [selected_id](const std::unique_ptr<Link>& l) {
            return l->id == selected_id;
            }),
          data.links.end()
          );
    }
  }

  int link_id;
  if (ImNodes::IsLinkDestroyed(&link_id)) {
    data.links.erase(
        std::remove_if(data.links.begin(), data.links.end(),
          [link_id](const std::unique_ptr<Link>& l) {
          return l->id == link_id;
          }),
        data.links.end()
        );
  }
}

void NodeEditor::deleteNodes(OrchestrationData& data) {
  if (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
    data.nodes.erase(
        std::remove_if(data.nodes.begin(), data.nodes.end(),
          [&](const std::unique_ptr<Node>& n) {
          if (ImNodes::IsNodeSelected(n->getId())) {
          auto nodeAttributes = n->getAttributeIds();
          data.links.erase(
              std::remove_if(data.links.begin(), data.links.end(),
                [&](const std::unique_ptr<Link>& l) {
                return std::find(nodeAttributes.begin(), nodeAttributes.end(), l->start_attr) != nodeAttributes.end() ||
                std::find(nodeAttributes.begin(), nodeAttributes.end(), l->end_attr) != nodeAttributes.end();
                }),
              data.links.end()
              );
          return true;
          }
          return false;
          }),
        data.nodes.end()
        );
  }
}

std::vector<NodeData> NodeEditor::getAllNodesData() const {
  std::vector<NodeData> result;
  
  for (const auto& [orch_id, data] : orchestration_data) {
    for (const auto& node : data->nodes) {
      NodeData node_data;
      node_data.id = node->getId();
      node_data.orchestration_id = orch_id;
      node_data.pos_x = node->getPosition().x;
      node_data.pos_y = node->getPosition().y;
      
      if (dynamic_cast<StartNode*>(node.get())) {
        node_data.type = "Start";
      } else if (dynamic_cast<GetNode*>(node.get())) {
        node_data.type = "GET";
      } else if (dynamic_cast<PostNode*>(node.get())) {
        node_data.type = "POST";
      }
      
      result.push_back(node_data);
    }
  }
  
  return result;
}

std::vector<LinkData> NodeEditor::getAllLinksData() const {
  std::vector<LinkData> result;
  
  for (const auto& [orch_id, data] : orchestration_data) {
    for (const auto& link : data->links) {
      LinkData link_data;
      link_data.id = link->id;
      link_data.orchestration_id = orch_id;
      link_data.start_attr = link->start_attr;
      link_data.end_attr = link->end_attr;
      
      result.push_back(link_data);
    }
  }
  
  return result;
}

void NodeEditor::loadNodesData(const std::vector<NodeData>& nodes_data) {
  for (const auto& node_data : nodes_data) {
    auto& data = getOrchestrationData(node_data.orchestration_id);
    
    ImVec2 position(node_data.pos_x, node_data.pos_y);
    createNodeWithId(node_data.id, node_data.type, position, data);
  }
}

void NodeEditor::loadLinksData(const std::vector<LinkData>& links_data) {
  for (const auto& link_data : links_data) {
    auto it = orchestration_data.find(link_data.orchestration_id);
    if (it != orchestration_data.end()) {
      auto& data = *it->second;
      data.links.push_back(std::make_unique<Link>(link_data.id, link_data.start_attr, link_data.end_attr));
      
      if (link_data.id >= data.next_link_id) {
        data.next_link_id = link_data.id + 1;
      }
    }
  }
}
