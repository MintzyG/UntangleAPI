#pragma once
#include "http_client.h"
#include <string>
#include <map>
#include <any>

class Node;

struct ExecutionContext {
    std::map<std::string, std::any> variables;
    HttpClient http_client;
    std::string last_response_body;
    int last_status_code = 0;
    std::string execution_log;
    
    void setVariable(const std::string& name, const std::any& value);
    std::any getVariable(const std::string& name);
    bool hasVariable(const std::string& name);
    void log(const std::string& message);
};

class NodeExecutor {
public:
    static bool execute(Node* node, ExecutionContext& context);
};
