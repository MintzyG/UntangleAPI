#include "database.h"
#include "project.h"
#include "node_editor.h"
#include "nodes.h"
#include <stdio.h>

Database::Database() {}

Database::~Database() {
    close();
}

bool Database::initialize(const std::string& db_path) {
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc != SQLITE_OK) {
        printf("Failed to open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    return createTables();
}

void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::createTables() {
    const char* sql_projects = R"(
        CREATE TABLE IF NOT EXISTS projects (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL
        );
    )";
    
    const char* sql_orchestrations = R"(
        CREATE TABLE IF NOT EXISTS orchestrations (
            id INTEGER PRIMARY KEY,
            project_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            FOREIGN KEY (project_id) REFERENCES projects(id) ON DELETE CASCADE
        );
    )";
    
    const char* sql_nodes = R"(
        CREATE TABLE IF NOT EXISTS nodes (
            id INTEGER PRIMARY KEY,
            orchestration_id INTEGER NOT NULL,
            type TEXT NOT NULL,
            pos_x REAL NOT NULL,
            pos_y REAL NOT NULL,
            FOREIGN KEY (orchestration_id) REFERENCES orchestrations(id) ON DELETE CASCADE
        );
    )";
    
    const char* sql_links = R"(
        CREATE TABLE IF NOT EXISTS links (
            id INTEGER PRIMARY KEY,
            orchestration_id INTEGER NOT NULL,
            start_attr INTEGER NOT NULL,
            end_attr INTEGER NOT NULL,
            FOREIGN KEY (orchestration_id) REFERENCES orchestrations(id) ON DELETE CASCADE
        );
    )";
    
    char* err_msg = nullptr;
    
    if (sqlite3_exec(db, sql_projects, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        printf("Failed to create projects table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    
    if (sqlite3_exec(db, sql_orchestrations, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        printf("Failed to create orchestrations table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    
    if (sqlite3_exec(db, sql_nodes, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        printf("Failed to create nodes table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    
    if (sqlite3_exec(db, sql_links, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        printf("Failed to create links table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    
    return true;
}

bool Database::clearTable(const std::string& table_name) {
    std::string sql = "DELETE FROM " + table_name + ";";
    char* err_msg = nullptr;
    
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        printf("Failed to clear table %s: %s\n", table_name.c_str(), err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    
    return true;
}

bool Database::saveProjects(ProjectManager& project_manager) {
    if (!db) return false;
    
    clearTable("links");
    clearTable("nodes");
    clearTable("orchestrations");
    clearTable("projects");
    
    const char* sql_project = "INSERT INTO projects (id, name) VALUES (?, ?);";
    const char* sql_orch = "INSERT INTO orchestrations (id, project_id, name) VALUES (?, ?, ?);";
    
    sqlite3_stmt* stmt_project = nullptr;
    sqlite3_stmt* stmt_orch = nullptr;
    
    sqlite3_prepare_v2(db, sql_project, -1, &stmt_project, nullptr);
    sqlite3_prepare_v2(db, sql_orch, -1, &stmt_orch, nullptr);
    
    for (const auto& project : project_manager.getProjects()) {
        sqlite3_bind_int(stmt_project, 1, project->id);
        sqlite3_bind_text(stmt_project, 2, project->name.c_str(), -1, SQLITE_TRANSIENT);
        
        if (sqlite3_step(stmt_project) != SQLITE_DONE) {
            printf("Failed to save project: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_reset(stmt_project);
        
        for (const auto& orch : project->orchestrations) {
            sqlite3_bind_int(stmt_orch, 1, orch->id);
            sqlite3_bind_int(stmt_orch, 2, project->id);
            sqlite3_bind_text(stmt_orch, 3, orch->name.c_str(), -1, SQLITE_TRANSIENT);
            
            if (sqlite3_step(stmt_orch) != SQLITE_DONE) {
                printf("Failed to save orchestration: %s\n", sqlite3_errmsg(db));
            }
            sqlite3_reset(stmt_orch);
        }
    }
    
    sqlite3_finalize(stmt_project);
    sqlite3_finalize(stmt_orch);
    
    return true;
}

bool Database::saveNodes(NodeEditor& node_editor) {
    if (!db) return false;
    
    clearTable("nodes");
    
    const char* sql = "INSERT INTO nodes (id, orchestration_id, type, pos_x, pos_y) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    auto nodes_data = node_editor.getAllNodesData();
    for (const auto& node_data : nodes_data) {
        sqlite3_bind_int(stmt, 1, node_data.id);
        sqlite3_bind_int(stmt, 2, node_data.orchestration_id);
        sqlite3_bind_text(stmt, 3, node_data.type.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 4, node_data.pos_x);
        sqlite3_bind_double(stmt, 5, node_data.pos_y);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Failed to save node: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_reset(stmt);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool Database::saveLinks(NodeEditor& node_editor) {
    if (!db) return false;
    
    clearTable("links");
    
    const char* sql = "INSERT INTO links (id, orchestration_id, start_attr, end_attr) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    auto links_data = node_editor.getAllLinksData();
    for (const auto& link_data : links_data) {
        sqlite3_bind_int(stmt, 1, link_data.id);
        sqlite3_bind_int(stmt, 2, link_data.orchestration_id);
        sqlite3_bind_int(stmt, 3, link_data.start_attr);
        sqlite3_bind_int(stmt, 4, link_data.end_attr);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Failed to save link: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_reset(stmt);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool Database::loadProjects(ProjectManager& project_manager) {
    if (!db) return false;
    
    const char* sql_projects = "SELECT id, name FROM projects ORDER BY id;";
    const char* sql_orchs = "SELECT id, name FROM orchestrations WHERE project_id = ? ORDER BY id;";
    
    sqlite3_stmt* stmt_proj = nullptr;
    sqlite3_stmt* stmt_orch = nullptr;
    
    sqlite3_prepare_v2(db, sql_projects, -1, &stmt_proj, nullptr);
    sqlite3_prepare_v2(db, sql_orchs, -1, &stmt_orch, nullptr);
    
    while (sqlite3_step(stmt_proj) == SQLITE_ROW) {
        int proj_id = sqlite3_column_int(stmt_proj, 0);
        const char* proj_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt_proj, 1));
        
        project_manager.addProjectWithId(proj_id, proj_name);
        
        sqlite3_bind_int(stmt_orch, 1, proj_id);
        
        while (sqlite3_step(stmt_orch) == SQLITE_ROW) {
            int orch_id = sqlite3_column_int(stmt_orch, 0);
            const char* orch_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt_orch, 1));
            
            Project* proj = project_manager.getProject(proj_id);
            if (proj) {
                proj->addOrchestrationWithId(orch_id, orch_name);
            }
        }
        
        sqlite3_reset(stmt_orch);
    }
    
    sqlite3_finalize(stmt_proj);
    sqlite3_finalize(stmt_orch);
    
    return true;
}

bool Database::loadNodes(NodeEditor& node_editor) {
    if (!db) return false;
    
    const char* sql = "SELECT id, orchestration_id, type, pos_x, pos_y FROM nodes ORDER BY orchestration_id, id;";
    sqlite3_stmt* stmt = nullptr;
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    std::vector<NodeData> nodes_data;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        NodeData node_data;
        node_data.id = sqlite3_column_int(stmt, 0);
        node_data.orchestration_id = sqlite3_column_int(stmt, 1);
        node_data.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        node_data.pos_x = sqlite3_column_double(stmt, 3);
        node_data.pos_y = sqlite3_column_double(stmt, 4);
        
        nodes_data.push_back(node_data);
    }
    
    sqlite3_finalize(stmt);
    
    node_editor.loadNodesData(nodes_data);
    return true;
}

bool Database::loadLinks(NodeEditor& node_editor) {
    if (!db) return false;
    
    const char* sql = "SELECT id, orchestration_id, start_attr, end_attr FROM links ORDER BY orchestration_id, id;";
    sqlite3_stmt* stmt = nullptr;
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    std::vector<LinkData> links_data;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LinkData link_data;
        link_data.id = sqlite3_column_int(stmt, 0);
        link_data.orchestration_id = sqlite3_column_int(stmt, 1);
        link_data.start_attr = sqlite3_column_int(stmt, 2);
        link_data.end_attr = sqlite3_column_int(stmt, 3);
        
        links_data.push_back(link_data);
    }
    
    sqlite3_finalize(stmt);
    
    node_editor.loadLinksData(links_data);
    return true;
}

bool Database::saveAll(ProjectManager& project_manager, NodeEditor& node_editor) {
    if (!db) return false;
    
    char* err_msg = nullptr;
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &err_msg);
    
    bool success = saveProjects(project_manager);
    success &= saveNodes(node_editor);
    success &= saveLinks(node_editor);
    
    if (success) {
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err_msg);
        printf("Data saved successfully!\n");
    } else {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, &err_msg);
        printf("Failed to save data, rolled back transaction\n");
    }
    
    return success;
}

bool Database::loadAll(ProjectManager& project_manager, NodeEditor& node_editor) {
    if (!db) return false;
    
    bool success = loadProjects(project_manager);
    success &= loadNodes(node_editor);
    success &= loadLinks(node_editor);
    
    if (success) {
        printf("Data loaded successfully!\n");
    }
    
    return success;
}
