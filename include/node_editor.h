#pragma once
#include "imgui.h"
#include "imnodes.h"
#include "nodes.h"
#include "link.h"
#include "executor.h"
#include "sidebar.h"
#include <vector>
#include <memory>
#include <map>

struct NodeData;
struct LinkData;

struct OrchestrationData {
  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::unique_ptr<Link>> links;
  int next_node_id = 1;
  int next_link_id = 10000;
};

class NodeEditor {
  public:
    NodeEditor();
    ~NodeEditor();

    bool initialize();
    void shutdown();
    void render(const Sidebar& sidebar);

    std::vector<NodeData> getAllNodesData() const;
    std::vector<LinkData> getAllLinksData() const;
    
    void loadNodesData(const std::vector<NodeData>& nodes_data);
    void loadLinksData(const std::vector<LinkData>& links_data);

    void executeSelectedNode();
    void executeOrchestration(int orchestration_id);
    std::string getExecutionLog() const { return execution_context.execution_log; }

  private:
    std::map<int, std::unique_ptr<OrchestrationData>> orchestration_data;
    bool initialized = false;
    ExecutionContext execution_context;

    void handleContextMenu(OrchestrationData& data);
    void handleRightClick();
    ImVec2 context_menu_pos;
    bool right_clicked_in_editor = false;

    void createNode(const std::string& nodeType, ImVec2 position, OrchestrationData& data);
    void createNodeWithId(int node_id, const std::string& nodeType, ImVec2 position, OrchestrationData& data);
    void deleteNodes(OrchestrationData& data);
    void drawNodes(const OrchestrationData& data) const;

    void createLinks(OrchestrationData& data);
    void deleteLinks(OrchestrationData& data);
    void drawLinks(const OrchestrationData& data) const;

    OrchestrationData& getOrchestrationData(int orchestration_id);
};
