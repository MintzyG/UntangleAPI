#include "nodes.h"
#include <string>

// -------------------- Base --------------------
Node::Node(int nodeId, const std::string& nodeTitle)
    : id(nodeId), title(nodeTitle) {}

Node::~Node() {}

int Node::getId() const { return id; }

// -------------------- StartNode --------------------
StartNode::StartNode(int nodeId) : Node(nodeId, "Start") {}

std::vector<int> StartNode::getAttributeIds() const { return {id + 1}; }

void StartNode::draw() {
    ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(200, 60, 60, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(220, 80, 80, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(180, 40, 40, 255));
    ImNodes::BeginNode(id);

      ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(title.c_str());
      ImNodes::EndNodeTitleBar();

      ImNodes::BeginOutputAttribute(id + 1);
        ImGui::Text("Start ->");
      ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

// -------------------- GetNode --------------------
GetNode::GetNode(int nodeId) : Node(nodeId, "GET") {}

std::vector<int> GetNode::getAttributeIds() const { return {id + 1, id + 2}; }

void GetNode::draw() {
    ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(60, 180, 75, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(80, 200, 95, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(40, 160, 55, 255));
    ImNodes::BeginNode(id);

      ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(title.c_str());
      ImNodes::EndNodeTitleBar();

      ImNodes::BeginInputAttribute(id + 1);
        ImGui::Text("URL");
      ImNodes::EndInputAttribute();

      ImNodes::BeginOutputAttribute(id + 2);
        ImGui::Text("Response");
      ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}


// -------------------- PostNode --------------------
PostNode::PostNode(int nodeId) : Node(nodeId, "POST") {}

std::vector<int> PostNode::getAttributeIds() const { return {id + 1, id + 2, id + 3}; }

void PostNode::draw() {
    ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(255, 200, 60, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(235, 180, 40, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(255, 220, 90, 255));
    ImNodes::BeginNode(id);

      ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(title.c_str());
      ImNodes::EndNodeTitleBar();

      ImNodes::BeginInputAttribute(id + 1);
        ImGui::Text("URL");
      ImNodes::EndInputAttribute();

      ImNodes::BeginInputAttribute(id + 2);
        ImGui::Text("Body");
      ImNodes::EndInputAttribute();

      ImNodes::BeginOutputAttribute(id + 3);
        ImGui::Text("Response");
      ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}
