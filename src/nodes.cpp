#include "nodes.h"
#include <string>

// -------------------- Base --------------------
Node::Node(int nodeId, const std::string& nodeTitle)
  : id(nodeId), title(nodeTitle), position(ImVec2(0, 0)) {}

Node::~Node() {}

int Node::getId() const { return id; }

void Node::setPosition(ImVec2 pos) {
  position = pos;
  ImNodes::SetNodeGridSpacePos(id, position);
}

ImVec2 Node::getPosition() const {
  return position;
}

void Node::updatePosition() {
  position = ImNodes::GetNodeGridSpacePos(id);
}

// -------------------- StartNode --------------------
StartNode::StartNode(int nodeId) : Node(nodeId, "Start") {}

std::vector<int> StartNode::getAttributeIds() const { return {id + 1}; }

void StartNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(100, 200, 100, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(120, 220, 120, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(80, 180, 80, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("Start");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginOutputAttribute(id + 1);
  ImGui::Text("Flow");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- HttpGetNode --------------------
HttpGetNode::HttpGetNode(int nodeId) : Node(nodeId, "HTTP GET") {}

std::vector<int> HttpGetNode::getAttributeIds() const { 
  return {id + 1, id + 2, id + 3, id + 4}; 
}

void HttpGetNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(70, 130, 220, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(90, 150, 240, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(50, 110, 200, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("🌐 HTTP GET");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("URL:");
  ImGui::InputText("##url", url, sizeof(url));
  
  ImGui::Text("Headers:");
  ImGui::InputTextMultiline("##headers", headers, sizeof(headers), ImVec2(200, 40));
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 2);
  ImGui::Text("Response");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 3);
  ImGui::Text("Status Code");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 4);
  ImGui::Text("Next");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- HttpPostNode --------------------
HttpPostNode::HttpPostNode(int nodeId) : Node(nodeId, "HTTP POST") {}

std::vector<int> HttpPostNode::getAttributeIds() const { 
  return {id + 1, id + 2, id + 3, id + 4, id + 5}; 
}

void HttpPostNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(90, 180, 90, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(110, 200, 110, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(70, 160, 70, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("HTTP POST");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("URL:");
  ImGui::InputText("##url", url, sizeof(url));
  
  ImGui::Text("Headers:");
  ImGui::InputTextMultiline("##headers", headers, sizeof(headers), ImVec2(200, 40));
  
  ImGui::Text("Body:");
  ImGui::InputTextMultiline("##body", body, sizeof(body), ImVec2(200, 60));
  ImGui::PopItemWidth();

  ImNodes::BeginInputAttribute(id + 2);
  ImGui::Text("Body Data");
  ImNodes::EndInputAttribute();

  ImNodes::BeginOutputAttribute(id + 3);
  ImGui::Text("Response");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 4);
  ImGui::Text("Status Code");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 5);
  ImGui::Text("Next");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- HttpPutNode --------------------
HttpPutNode::HttpPutNode(int nodeId) : Node(nodeId, "HTTP PUT") {}

std::vector<int> HttpPutNode::getAttributeIds() const { 
  return {id + 1, id + 2, id + 3, id + 4, id + 5}; 
}

void HttpPutNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(220, 150, 60, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(240, 170, 80, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(200, 130, 40, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("HTTP PUT");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("URL:");
  ImGui::InputText("##url", url, sizeof(url));
  
  ImGui::Text("Headers:");
  ImGui::InputTextMultiline("##headers", headers, sizeof(headers), ImVec2(200, 40));
  
  ImGui::Text("Body:");
  ImGui::InputTextMultiline("##body", body, sizeof(body), ImVec2(200, 60));
  ImGui::PopItemWidth();

  ImNodes::BeginInputAttribute(id + 2);
  ImGui::Text("Body Data");
  ImNodes::EndInputAttribute();

  ImNodes::BeginOutputAttribute(id + 3);
  ImGui::Text("Response");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 4);
  ImGui::Text("Status Code");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 5);
  ImGui::Text("Next");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- HttpDeleteNode --------------------
HttpDeleteNode::HttpDeleteNode(int nodeId) : Node(nodeId, "HTTP DELETE") {}

std::vector<int> HttpDeleteNode::getAttributeIds() const { 
  return {id + 1, id + 2, id + 3, id + 4}; 
}

void HttpDeleteNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(220, 70, 70, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(240, 90, 90, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(200, 50, 50, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("HTTP DELETE");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("URL:");
  ImGui::InputText("##url", url, sizeof(url));
  
  ImGui::Text("Headers:");
  ImGui::InputTextMultiline("##headers", headers, sizeof(headers), ImVec2(200, 40));
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 2);
  ImGui::Text("Response");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 3);
  ImGui::Text("Status Code");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 4);
  ImGui::Text("Next");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- JsonExtractNode --------------------
JsonExtractNode::JsonExtractNode(int nodeId) : Node(nodeId, "JSON Extract") {}

std::vector<int> JsonExtractNode::getAttributeIds() const { 
  return {id + 1, id + 2}; 
}

void JsonExtractNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(180, 120, 200, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(200, 140, 220, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(160, 100, 180, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("JSON Extract");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("JSON");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("Path (JSONPath):");
  ImGui::InputText("##path", json_path, sizeof(json_path));
  ImGui::TextDisabled("e.g., $.data.id or $.users[0].name");
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 2);
  ImGui::Text("Value");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- SetVariableNode --------------------
SetVariableNode::SetVariableNode(int nodeId) : Node(nodeId, "Set Variable") {}

std::vector<int> SetVariableNode::getAttributeIds() const { 
  return {id + 1, id + 2}; 
}

void SetVariableNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(150, 100, 200, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(170, 120, 220, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(130, 80, 180, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("Set Variable");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("Value");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("Variable Name:");
  ImGui::InputText("##varname", var_name, sizeof(var_name));
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 2);
  ImGui::Text("Next");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- GetVariableNode --------------------
GetVariableNode::GetVariableNode(int nodeId) : Node(nodeId, "Get Variable") {}

std::vector<int> GetVariableNode::getAttributeIds() const { 
  return {id + 1}; 
}

void GetVariableNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(120, 80, 180, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(140, 100, 200, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(100, 60, 160, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("Get Variable");
  ImNodes::EndNodeTitleBar();

  ImGui::PushItemWidth(200);
  ImGui::Text("Variable Name:");
  ImGui::InputText("##varname", var_name, sizeof(var_name));
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 1);
  ImGui::Text("Value");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- IfConditionNode --------------------
IfConditionNode::IfConditionNode(int nodeId) : Node(nodeId, "If Condition") {}

std::vector<int> IfConditionNode::getAttributeIds() const { 
  return {id + 1, id + 2, id + 3}; 
}

void IfConditionNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(200, 180, 80, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(220, 200, 100, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(180, 160, 60, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("If Condition");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("Condition:");
  ImGui::InputText("##condition", condition, sizeof(condition));
  ImGui::TextDisabled("e.g., status_code == 200");
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 2);
  ImGui::Text("True");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 3);
  ImGui::Text("False");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- DelayNode --------------------
DelayNode::DelayNode(int nodeId) : Node(nodeId, "Delay") {}

std::vector<int> DelayNode::getAttributeIds() const { 
  return {id + 1, id + 2}; 
}

void DelayNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(100, 150, 180, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(120, 170, 200, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(80, 130, 160, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("Delay");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("Delay (ms):");
  ImGui::InputText("##delay", delay_ms, sizeof(delay_ms));
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 2);
  ImGui::Text("Next");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- AssertNode --------------------
AssertNode::AssertNode(int nodeId) : Node(nodeId, "Assert") {}

std::vector<int> AssertNode::getAttributeIds() const { 
  return {id + 1, id + 2, id + 3}; 
}

void AssertNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(220, 100, 150, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(240, 120, 170, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(200, 80, 130, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("Assert");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("Assertion:");
  ImGui::InputText("##assertion", assertion, sizeof(assertion));
  ImGui::TextDisabled("e.g., status_code == 200");
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 2);
  ImGui::Text("Pass");
  ImNodes::EndOutputAttribute();

  ImNodes::BeginOutputAttribute(id + 3);
  ImGui::Text("Fail");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}

// -------------------- LogNode --------------------
LogNode::LogNode(int nodeId) : Node(nodeId, "Log") {}

std::vector<int> LogNode::getAttributeIds() const { 
  return {id + 1, id + 2}; 
}

void LogNode::draw() {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(130, 130, 130, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(150, 150, 150, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(110, 110, 110, 255));
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("Log");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(id + 1);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();

  ImGui::PushItemWidth(200);
  ImGui::Text("Message:");
  ImGui::InputText("##message", message, sizeof(message));
  ImGui::PopItemWidth();

  ImNodes::BeginOutputAttribute(id + 2);
  ImGui::Text("Next");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();
}
