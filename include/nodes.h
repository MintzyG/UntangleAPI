#pragma once
#include "imnodes.h"
#include "imgui.h"
#include <string>
#include <vector>

class Node {
protected:
    int id;
    int orchestration_id;
    std::string title;
    ImVec2 position;

public:
    Node(int nodeId, const std::string& nodeTitle);
    virtual ~Node();

    virtual std::vector<int> getAttributeIds() const = 0;
    virtual void draw() = 0;
    virtual std::string getType() const = 0;
    int getId() const;

    void setPosition(ImVec2 pos);
    ImVec2 getPosition() const;
    void updatePosition();
};

// Entry point for the workflow
class StartNode : public Node {
public:
    StartNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "Start"; }
};

// HTTP Request Nodes
class HttpGetNode : public Node {
private:
    char url[512] = "https://api.example.com/users";
    char headers[256] = "Authorization: Bearer token";
public:
    HttpGetNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "HTTP_GET"; }
};

class HttpPostNode : public Node {
private:
    char url[512] = "https://api.example.com/users";
    char headers[256] = "Content-Type: application/json";
    char body[512] = "{\n  \"name\": \"John\"\n}";
public:
    HttpPostNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "HTTP_POST"; }
};

class HttpPutNode : public Node {
private:
    char url[512] = "https://api.example.com/users/1";
    char headers[256] = "Content-Type: application/json";
    char body[512] = "{\n  \"name\": \"Jane\"\n}";
public:
    HttpPutNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "HTTP_PUT"; }
};

class HttpDeleteNode : public Node {
private:
    char url[512] = "https://api.example.com/users/1";
    char headers[256] = "Authorization: Bearer token";
public:
    HttpDeleteNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "HTTP_DELETE"; }
};

// Data Processing Nodes
class JsonExtractNode : public Node {
private:
    char json_path[256] = "$.data.id";
public:
    JsonExtractNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "JSON_EXTRACT"; }
};

class SetVariableNode : public Node {
private:
    char var_name[128] = "user_id";
public:
    SetVariableNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "SET_VARIABLE"; }
};

class GetVariableNode : public Node {
private:
    char var_name[128] = "user_id";
public:
    GetVariableNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "GET_VARIABLE"; }
};

// Logic Nodes
class IfConditionNode : public Node {
private:
    char condition[256] = "status_code == 200";
public:
    IfConditionNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "IF_CONDITION"; }
};

class DelayNode : public Node {
private:
    char delay_ms[64] = "1000";
public:
    DelayNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "DELAY"; }
};

// Validation Node
class AssertNode : public Node {
private:
    char assertion[256] = "status_code == 200";
public:
    AssertNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "ASSERT"; }
};

// Output Node
class LogNode : public Node {
private:
    char message[256] = "Request completed";
public:
    LogNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "LOG"; }
};
