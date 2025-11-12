#include "executor.h"
#include "nodes.h"
#include <SDL.h>
#include <iostream>
#include <sstream>

void ExecutionContext::setVariable(const std::string& name, const std::any& value) {
    variables[name] = value;
}

std::any ExecutionContext::getVariable(const std::string& name) {
    if (variables.find(name) != variables.end()) {
        return variables[name];
    }
    return std::any();
}

bool ExecutionContext::hasVariable(const std::string& name) {
    return variables.find(name) != variables.end();
}

void ExecutionContext::log(const std::string& message) {
    execution_log += message + "\n";
    std::cout << "[EXEC] " << message << std::endl;
}

// Helper to parse headers from string
std::map<std::string, std::string> parseHeaders(const std::string& headers_str) {
    std::map<std::string, std::string> headers;
    std::istringstream stream(headers_str);
    std::string line;
    
    while (std::getline(stream, line)) {
        if (line.empty() || line == "\r") continue;
        
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t\r"));
            key.erase(key.find_last_not_of(" \t\r") + 1);
            value.erase(0, value.find_first_not_of(" \t\r"));
            value.erase(value.find_last_not_of(" \t\r") + 1);
            
            headers[key] = value;
        }
    }
    
    return headers;
}

bool NodeExecutor::execute(Node* node, ExecutionContext& context) {
    if (!node) return false;
    
    // DEBUG INFO
    printf("DEBUG Executor: Node pointer: %p\n", (void*)node);
    printf("DEBUG Executor: Node ID: %d\n", node->getId());
    
    std::string type = node->getType();
    printf("DEBUG Executor: Type string: '%s'\n", type.c_str());
    printf("DEBUG Executor: Type length: %zu\n", type.length());
    
    // Try all dynamic casts to see what the object actually is
    printf("DEBUG: Is StartNode? %s\n", dynamic_cast<StartNode*>(node) ? "YES" : "NO");
    printf("DEBUG: Is HttpGetNode? %s\n", dynamic_cast<HttpGetNode*>(node) ? "YES" : "NO");
    printf("DEBUG: Is HttpPostNode? %s\n", dynamic_cast<HttpPostNode*>(node) ? "YES" : "NO");
    printf("DEBUG: Is HttpPutNode? %s\n", dynamic_cast<HttpPutNode*>(node) ? "YES" : "NO");
    printf("DEBUG: Is HttpDeleteNode? %s\n", dynamic_cast<HttpDeleteNode*>(node) ? "YES" : "NO");
    printf("DEBUG: Is IfConditionNode? %s\n", dynamic_cast<IfConditionNode*>(node) ? "YES" : "NO");
    printf("DEBUG: Is LogNode? %s\n", dynamic_cast<LogNode*>(node) ? "YES" : "NO");
    
    context.log("Executing node: " + type + " (ID: " + std::to_string(node->getId()) + ")");
       
    if (type == "Start") {
        context.log("Starting workflow execution");
        return true;
    }
    else if (type == "HTTP_GET") {
        auto* http_node = dynamic_cast<HttpGetNode*>(node);
        if (!http_node) return false;
        
        std::string url = http_node->getUrl();
        std::string headers_str = http_node->getHeaders();
        
        context.log("GET Request to: " + url);
        
        auto headers = parseHeaders(headers_str);
        HttpResponse response = context.http_client.get(url, headers);
        
        if (response.success) {
            context.last_response_body = response.body;
            context.last_status_code = response.status_code;
            context.log("Response: Status " + std::to_string(response.status_code));
            context.log("Body: " + response.body.substr(0, 200) + (response.body.length() > 200 ? "..." : ""));
            return true;
        } else {
            context.log("ERROR: " + response.error_message);
            return false;
        }
    }
    else if (type == "HTTP_POST") {
        auto* http_node = dynamic_cast<HttpPostNode*>(node);
        if (!http_node) return false;
        
        std::string url = http_node->getUrl();
        std::string headers_str = http_node->getHeaders();
        std::string body = http_node->getBody();
        
        context.log("POST Request to: " + url);
        
        auto headers = parseHeaders(headers_str);
        HttpResponse response = context.http_client.post(url, body, headers);
        
        if (response.success) {
            context.last_response_body = response.body;
            context.last_status_code = response.status_code;
            context.log("Response: Status " + std::to_string(response.status_code));
            context.log("Body: " + response.body.substr(0, 200) + (response.body.length() > 200 ? "..." : ""));
            return true;
        } else {
            context.log("ERROR: " + response.error_message);
            return false;
        }
    }
    else if (type == "HTTP_PUT") {
        auto* http_node = dynamic_cast<HttpPutNode*>(node);
        if (!http_node) return false;
        
        std::string url = http_node->getUrl();
        std::string headers_str = http_node->getHeaders();
        std::string body = http_node->getBody();
        
        context.log("PUT Request to: " + url);
        
        auto headers = parseHeaders(headers_str);
        HttpResponse response = context.http_client.put(url, body, headers);
        
        if (response.success) {
            context.last_response_body = response.body;
            context.last_status_code = response.status_code;
            context.log("Response: Status " + std::to_string(response.status_code));
            context.log("Body: " + response.body.substr(0, 200) + (response.body.length() > 200 ? "..." : ""));
            return true;
        } else {
            context.log("ERROR: " + response.error_message);
            return false;
        }
    }
    else if (type == "HTTP_DELETE") {
        auto* http_node = dynamic_cast<HttpDeleteNode*>(node);
        if (!http_node) return false;
        
        std::string url = http_node->getUrl();
        std::string headers_str = http_node->getHeaders();
        
        context.log("DELETE Request to: " + url);
        
        auto headers = parseHeaders(headers_str);
        HttpResponse response = context.http_client.del(url, headers);
        
        if (response.success) {
            context.last_response_body = response.body;
            context.last_status_code = response.status_code;
            context.log("Response: Status " + std::to_string(response.status_code));
            return true;
        } else {
            context.log("ERROR: " + response.error_message);
            return false;
        }
    }
    else if (type == "SET_VARIABLE") {
        auto* var_node = dynamic_cast<SetVariableNode*>(node);
        if (!var_node) return false;
        
        std::string var_name = var_node->getVarName();
        
        // For now, set the last response body as the variable value
        context.setVariable(var_name, context.last_response_body);
        context.log("Set variable '" + var_name + "' = " + context.last_response_body.substr(0, 100));
        return true;
    }
    else if (type == "GET_VARIABLE") {
        auto* var_node = dynamic_cast<GetVariableNode*>(node);
        if (!var_node) return false;
        
        std::string var_name = var_node->getVarName();
        
        if (context.hasVariable(var_name)) {
            auto value = context.getVariable(var_name);
            try {
                std::string str_value = std::any_cast<std::string>(value);
                context.log("Get variable '" + var_name + "' = " + str_value.substr(0, 100));
                context.last_response_body = str_value;
            } catch (const std::bad_any_cast&) {
                context.log("ERROR: Variable '" + var_name + "' is not a string");
                return false;
            }
            return true;
        } else {
            context.log("ERROR: Variable '" + var_name + "' not found");
            return false;
        }
    }
    else if (type == "LOG") {
        auto* log_node = dynamic_cast<LogNode*>(node);
        if (!log_node) return false;
        
        std::string message = log_node->getMessage();
        context.log("LOG: " + message);
        return true;
    }
    else if (type == "DELAY") {
        auto* delay_node = dynamic_cast<DelayNode*>(node);
        if (!delay_node) return false;
        
        std::string delay_str = delay_node->getDelayMs();
        int delay_ms = std::stoi(delay_str);
        
        context.log("Delaying for " + std::to_string(delay_ms) + "ms");
        SDL_Delay(delay_ms);
        return true;
    }
    
    context.log("WARNING: Node type '" + type + "' execution not implemented yet");
    return true;
}
