#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>

class ProjectManager;
class NodeEditor;

struct NodeData {
    int id;
    int orchestration_id;
    std::string type;
    float pos_x;
    float pos_y;
};

struct LinkData {
    int id;
    int orchestration_id;
    int start_attr;
    int end_attr;
};

class Database {
public:
    Database();
    ~Database();

    bool initialize(const std::string& db_path = "untangle.db");
    void close();

    bool saveProjects(ProjectManager& project_manager);
    bool saveNodes(NodeEditor& node_editor);
    bool saveLinks(NodeEditor& node_editor);
    
    bool loadProjects(ProjectManager& project_manager);
    bool loadNodes(NodeEditor& node_editor);
    bool loadLinks(NodeEditor& node_editor);

    bool saveAll(ProjectManager& project_manager, NodeEditor& node_editor);
    bool loadAll(ProjectManager& project_manager, NodeEditor& node_editor);

private:
    sqlite3* db = nullptr;
    bool createTables();
    
    bool clearTable(const std::string& table_name);
};
