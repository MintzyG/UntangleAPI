#include "node_editor.h"
#include "nodes.h"
#include "link.h"
#include "sidebar.h"
#include "executor.h"
#include "database.h"
#include <cstring>
#include <algorithm>
#include <memory>
#include <map>

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

  // Play button in top right
  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetCursorPos(ImVec2(io.DisplaySize.x - Sidebar::SIDEBAR_WIDTH - 240, 10));
  
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));
  
  if (ImGui::Button("Execute", ImVec2(100, 30))) {
    printf("Executing orchestration %d...\n", orchestration_id);
    executeOrchestration(orchestration_id);
  }
  
  ImGui::PopStyleColor(3);
  
  // Execute selected node button
  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.5f, 0.2f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.6f, 0.3f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.4f, 0.1f, 1.0f));
  
  if (ImGui::Button("Execute Selected", ImVec2(120, 30))) {
    executeSelectedNode();
  }
  
  ImGui::PopStyleColor(3);
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
  } else if (nodeType == "HTTP_GET") {
    newNode = std::make_unique<HttpGetNode>(data.next_node_id);
  } else if (nodeType == "HTTP_POST") {
    newNode = std::make_unique<HttpPostNode>(data.next_node_id);
  } else if (nodeType == "HTTP_PUT") {
    newNode = std::make_unique<HttpPutNode>(data.next_node_id);
  } else if (nodeType == "HTTP_DELETE") {
    newNode = std::make_unique<HttpDeleteNode>(data.next_node_id);
  } else if (nodeType == "JSON_EXTRACT") {
    newNode = std::make_unique<JsonExtractNode>(data.next_node_id);
  } else if (nodeType == "SET_VARIABLE") {
    newNode = std::make_unique<SetVariableNode>(data.next_node_id);
  } else if (nodeType == "GET_VARIABLE") {
    newNode = std::make_unique<GetVariableNode>(data.next_node_id);
  } else if (nodeType == "IF_CONDITION") {
    newNode = std::make_unique<IfConditionNode>(data.next_node_id);
  } else if (nodeType == "DELAY") {
    newNode = std::make_unique<DelayNode>(data.next_node_id);
  } else if (nodeType == "ASSERT") {
    newNode = std::make_unique<AssertNode>(data.next_node_id);
  } else if (nodeType == "LOG") {
    newNode = std::make_unique<LogNode>(data.next_node_id);
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
  } else if (nodeType == "HTTP_GET") {
    newNode = std::make_unique<HttpGetNode>(node_id);
  } else if (nodeType == "HTTP_POST") {
    newNode = std::make_unique<HttpPostNode>(node_id);
  } else if (nodeType == "HTTP_PUT") {
    newNode = std::make_unique<HttpPutNode>(node_id);
  } else if (nodeType == "HTTP_DELETE") {
    newNode = std::make_unique<HttpDeleteNode>(node_id);
  } else if (nodeType == "JSON_EXTRACT") {
    newNode = std::make_unique<JsonExtractNode>(node_id);
  } else if (nodeType == "SET_VARIABLE") {
    newNode = std::make_unique<SetVariableNode>(node_id);
  } else if (nodeType == "GET_VARIABLE") {
    newNode = std::make_unique<GetVariableNode>(node_id);
  } else if (nodeType == "IF_CONDITION") {
    newNode = std::make_unique<IfConditionNode>(node_id);
  } else if (nodeType == "DELAY") {
    newNode = std::make_unique<DelayNode>(node_id);
  } else if (nodeType == "ASSERT") {
    newNode = std::make_unique<AssertNode>(node_id);
  } else if (nodeType == "LOG") {
    newNode = std::make_unique<LogNode>(node_id);
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
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Add Node");
    ImGui::Separator();

    if (ImGui::MenuItem("Start")) {
      createNode("Start", context_menu_pos, data);
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.5f, 0.7f, 1.0f, 1.0f), "HTTP Requests");
    ImGui::Separator();

    if (ImGui::MenuItem("GET Request")) {
      createNode("HTTP_GET", context_menu_pos, data);
    }

    if (ImGui::MenuItem("POST Request")) {
      createNode("HTTP_POST", context_menu_pos, data);
    }

    if (ImGui::MenuItem("PUT Request")) {
      createNode("HTTP_PUT", context_menu_pos, data);
    }

    if (ImGui::MenuItem("DELETE Request")) {
      createNode("HTTP_DELETE", context_menu_pos, data);
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.7f, 0.5f, 1.0f, 1.0f), "Data Processing");
    ImGui::Separator();

    if (ImGui::MenuItem("JSON Extract")) {
      createNode("JSON_EXTRACT", context_menu_pos, data);
    }

    if (ImGui::MenuItem("Set Variable")) {
      createNode("SET_VARIABLE", context_menu_pos, data);
    }

    if (ImGui::MenuItem("Get Variable")) {
      createNode("GET_VARIABLE", context_menu_pos, data);
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 0.9f, 0.4f, 1.0f), "Logic & Control");
    ImGui::Separator();

    if (ImGui::MenuItem("If Condition")) {
      createNode("IF_CONDITION", context_menu_pos, data);
    }

    if (ImGui::MenuItem("Delay")) {
      createNode("DELAY", context_menu_pos, data);
    }

    if (ImGui::MenuItem("Assert")) {
      createNode("ASSERT", context_menu_pos, data);
    }

    if (ImGui::MenuItem("Log")) {
      createNode("LOG", context_menu_pos, data);
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
      node_data.type = node->getType();
      
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

void NodeEditor::executeSelectedNode() {
  std::vector<int> selected_nodes;
  selected_nodes.resize(ImNodes::NumSelectedNodes());
  ImNodes::GetSelectedNodes(selected_nodes.data());
  
  if (selected_nodes.empty()) {
    printf("No node selected\n");
    return;
  }
  
  int selected_node_id = selected_nodes[0];
  
  for (auto& [orch_id, data] : orchestration_data) {
    for (auto& node : data->nodes) {
      if (node->getId() == selected_node_id) {
        printf("\n=== Executing Single Node ===\n");
        execution_context.execution_log.clear();
        bool success = NodeExecutor::execute(node.get(), execution_context);
        printf("Execution %s\n", success ? "SUCCESS" : "FAILED");
        printf("=== End Execution ===\n\n");
        return;
      }
    }
  }
  
  printf("Selected node not found\n");
}

void NodeEditor::executeOrchestration(int orchestration_id) {
  auto it = orchestration_data.find(orchestration_id);
  if (it == orchestration_data.end()) {
    printf("Orchestration not found\n");
    return;
  }
  
  auto& data = *it->second;
  
  printf("\n=== Executing Full Orchestration ===\n");
  execution_context.execution_log.clear();
  execution_context.variables.clear();
  
  Node* start_node = nullptr;
  for (auto& node : data.nodes) {
    if (node->getType() == "Start") {
      start_node = node.get();
      break;
    }
  }
  
  if (!start_node) {
    printf("ERROR: No Start node found in orchestration\n");
    return;
  }
  
  if (!NodeExecutor::execute(start_node, execution_context)) {
    printf("Start node execution failed\n");
    return;
  }
  
  std::vector<Node*> execution_queue;
  std::map<int, bool> executed_nodes;
  
  auto start_attrs = start_node->getAttributeIds();
  if (start_attrs.empty()) {
    printf("Start node has no output\n");
    return;
  }
  
  int current_attr = start_attrs[0];
  executed_nodes[start_node->getId()] = true;
  
  int max_iterations = 100;
  int iterations = 0;
  
  while (iterations < max_iterations) {
    iterations++;
    
    Link* next_link = nullptr;
    for (auto& link : data.links) {
      if (link->start_attr == current_attr) {
        next_link = link.get();
        break;
      }
    }
    
    if (!next_link) {
      printf("No more connected nodes, execution complete\n");
      break;
    }
    
    Node* next_node = nullptr;
    for (auto& node : data.nodes) {
      if (executed_nodes[node->getId()]) continue;
      
      auto attrs = node->getAttributeIds();
      for (int attr : attrs) {
        if (attr == next_link->end_attr) {
          next_node = node.get();
          break;
        }
      }
      if (next_node) break;
    }
    
    if (!next_node) {
      printf("Could not find next node in chain\n");
      break;
    }
    
    if (!NodeExecutor::execute(next_node, execution_context)) {
      printf("Node execution failed, stopping\n");
      break;
    }
    
    executed_nodes[next_node->getId()] = true;
    
    // Get output attribute for next iteration (last attribute is usually output)
    auto next_attrs = next_node->getAttributeIds();
    if (!next_attrs.empty()) {
      current_attr = next_attrs.back();
    } else {
      break;
    }
  }
  
  printf("=== End Orchestration Execution ===\n\n");
}
