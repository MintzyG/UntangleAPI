#include "nodes.h"
#include <string>
#include <sstream>
#include <cstring>

std::string escape_string(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (c == '|') result += "\\|";
        else if (c == '\\') result += "\\\\";
        else result += c;
    }
    return result;
}

std::string unescape_string(const std::string& str) {
    std::string result;
    bool escape = false;
    for (char c : str) {
        if (escape) {
            result += c;
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else {
            result += c;
        }
    }
    return result;
}

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
  ImGui::TextUnformatted("HTTP GET");
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

std::string HttpGetNode::serializeData() const {
    return escape_string(std::string(url)) + "|" + escape_string(std::string(headers));
}

void HttpGetNode::deserializeData(const std::string& data) {
    size_t pos = data.find('|');
    if (pos != std::string::npos) {
        std::string url_str = unescape_string(data.substr(0, pos));
        std::string headers_str = unescape_string(data.substr(pos + 1));
        
        strncpy(url, url_str.c_str(), sizeof(url) - 1);
        url[sizeof(url) - 1] = '\0';
        
        strncpy(headers, headers_str.c_str(), sizeof(headers) - 1);
        headers[sizeof(headers) - 1] = '\0';
    }
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

std::string HttpPostNode::serializeData() const {
    return escape_string(std::string(url)) + "|" + 
           escape_string(std::string(headers)) + "|" + 
           escape_string(std::string(body));
}

void HttpPostNode::deserializeData(const std::string& data) {
    std::istringstream ss(data);
    std::string url_str, headers_str, body_str;
    
    size_t pos1 = data.find('|');
    if (pos1 == std::string::npos) return;
    
    size_t pos2 = data.find('|', pos1 + 1);
    if (pos2 == std::string::npos) return;
    
    url_str = unescape_string(data.substr(0, pos1));
    headers_str = unescape_string(data.substr(pos1 + 1, pos2 - pos1 - 1));
    body_str = unescape_string(data.substr(pos2 + 1));
    
    strncpy(url, url_str.c_str(), sizeof(url) - 1);
    url[sizeof(url) - 1] = '\0';
    
    strncpy(headers, headers_str.c_str(), sizeof(headers) - 1);
    headers[sizeof(headers) - 1] = '\0';
    
    strncpy(body, body_str.c_str(), sizeof(body) - 1);
    body[sizeof(body) - 1] = '\0';
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

std::string HttpPutNode::serializeData() const {
    return escape_string(std::string(url)) + "|" + 
           escape_string(std::string(headers)) + "|" + 
           escape_string(std::string(body));
}

void HttpPutNode::deserializeData(const std::string& data) {
    size_t pos1 = data.find('|');
    if (pos1 == std::string::npos) return;
    
    size_t pos2 = data.find('|', pos1 + 1);
    if (pos2 == std::string::npos) return;
    
    std::string url_str = unescape_string(data.substr(0, pos1));
    std::string headers_str = unescape_string(data.substr(pos1 + 1, pos2 - pos1 - 1));
    std::string body_str = unescape_string(data.substr(pos2 + 1));
    
    strncpy(url, url_str.c_str(), sizeof(url) - 1);
    url[sizeof(url) - 1] = '\0';
    
    strncpy(headers, headers_str.c_str(), sizeof(headers) - 1);
    headers[sizeof(headers) - 1] = '\0';
    
    strncpy(body, body_str.c_str(), sizeof(body) - 1);
    body[sizeof(body) - 1] = '\0';
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

std::string HttpDeleteNode::serializeData() const {
    return escape_string(std::string(url)) + "|" + escape_string(std::string(headers));
}

void HttpDeleteNode::deserializeData(const std::string& data) {
    size_t pos = data.find('|');
    if (pos != std::string::npos) {
        std::string url_str = unescape_string(data.substr(0, pos));
        std::string headers_str = unescape_string(data.substr(pos + 1));
        
        strncpy(url, url_str.c_str(), sizeof(url) - 1);
        url[sizeof(url) - 1] = '\0';
        
        strncpy(headers, headers_str.c_str(), sizeof(headers) - 1);
        headers[sizeof(headers) - 1] = '\0';
    }
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

std::string JsonExtractNode::serializeData() const {
    return escape_string(std::string(json_path));
}

void JsonExtractNode::deserializeData(const std::string& data) {
    std::string path_str = unescape_string(data);
    strncpy(json_path, path_str.c_str(), sizeof(json_path) - 1);
    json_path[sizeof(json_path) - 1] = '\0';
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

std::string SetVariableNode::serializeData() const {
    return escape_string(std::string(var_name));
}

void SetVariableNode::deserializeData(const std::string& data) {
    std::string name_str = unescape_string(data);
    strncpy(var_name, name_str.c_str(), sizeof(var_name) - 1);
    var_name[sizeof(var_name) - 1] = '\0';
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

std::string GetVariableNode::serializeData() const {
    return escape_string(std::string(var_name));
}

void GetVariableNode::deserializeData(const std::string& data) {
    std::string name_str = unescape_string(data);
    strncpy(var_name, name_str.c_str(), sizeof(var_name) - 1);
    var_name[sizeof(var_name) - 1] = '\0';
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

std::string IfConditionNode::serializeData() const {
    return escape_string(std::string(condition));
}

void IfConditionNode::deserializeData(const std::string& data) {
    std::string cond_str = unescape_string(data);
    strncpy(condition, cond_str.c_str(), sizeof(condition) - 1);
    condition[sizeof(condition) - 1] = '\0';
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

std::string DelayNode::serializeData() const {
    return escape_string(std::string(delay_ms));
}

void DelayNode::deserializeData(const std::string& data) {
    std::string delay_str = unescape_string(data);
    strncpy(delay_ms, delay_str.c_str(), sizeof(delay_ms) - 1);
    delay_ms[sizeof(delay_ms) - 1] = '\0';
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

std::string AssertNode::serializeData() const {
    return escape_string(std::string(assertion));
}

void AssertNode::deserializeData(const std::string& data) {
    std::string assert_str = unescape_string(data);
    strncpy(assertion, assert_str.c_str(), sizeof(assertion) - 1);
    assertion[sizeof(assertion) - 1] = '\0';
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

std::string LogNode::serializeData() const {
    return escape_string(std::string(message));
}

void LogNode::deserializeData(const std::string& data) {
    std::string msg_str = unescape_string(data);
    strncpy(message, msg_str.c_str(), sizeof(message) - 1);
    message[sizeof(message) - 1] = '\0';
}
