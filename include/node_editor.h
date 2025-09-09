#pragma once
#include "imgui.h"
#include "imnodes.h"
#include "nodes.h"
#include "link.h"
#include <vector>
#include <memory>

class NodeEditor {
  public:
    NodeEditor();
    ~NodeEditor();

    bool initialize();
    void shutdown();
    void render();

    void showEditor(const char* window_title);

  private:
    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<std::unique_ptr<Link>> links;
    bool initialized = false;
    int next_node_id = 1;
    int next_link_id = 10000;

    void handleContextMenu();
    void handleRightClick();
    ImVec2 context_menu_pos;
    bool right_clicked_in_editor = false;

    void createNode(const std::string& nodeType, ImVec2 position);
    void drawNodes() const;

    void createLinks();
    void drawLinks() const;
};
