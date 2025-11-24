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
    char url[512] = "https://jsonplaceholder.typicode.com/posts/1";
    char headers[256] = "";
public:
    HttpGetNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "HTTP_GET"; }
    std::string getUrl() const { return std::string(url); }
    std::string getHeaders() const { return std::string(headers); }
};

class HttpPostNode : public Node {
private:
    char url[512] = "https://jsonplaceholder.typicode.com/posts";
    char headers[256] = "Content-Type: application/json";
    char body[512] = "{\n\"title\": \"hello world\",\n\"body\": \"this is a test post\",\n\"userId\": 1\n}";
public:
    HttpPostNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "HTTP_POST"; }
    std::string getUrl() const { return std::string(url); }
    std::string getHeaders() const { return std::string(headers); }
    std::string getBody() const { return std::string(body); }
};

class HttpPutNode : public Node {
private:
    char url[512] = "https://jsonplaceholder.typicode.com/posts/1";
    char headers[256] = "Content-Type: application/json";
    char body[512] = "{\n\"id\": 1,\n\"title\": \"updated title\",\n\"body\": \"this post has been updated\",\n\"userId\": 1\n}";
public:
    HttpPutNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "HTTP_PUT"; }
    std::string getUrl() const { return std::string(url); }
    std::string getHeaders() const { return std::string(headers); }
    std::string getBody() const { return std::string(body); }
};

class HttpDeleteNode : public Node {
private:
    char url[512] = "https://jsonplaceholder.typicode.com/posts/1";
    char headers[256] = "";
public:
    HttpDeleteNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "HTTP_DELETE"; }
    std::string getUrl() const { return std::string(url); }
    std::string getHeaders() const { return std::string(headers); }
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
    std::string getVarName() const { return std::string(var_name); }
};

class GetVariableNode : public Node {
private:
    char var_name[128] = "user_id";
public:
    GetVariableNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
    std::string getType() const override { return "GET_VARIABLE"; }
    std::string getVarName() const { return std::string(var_name); }
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
    std::string getDelayMs() const { return std::string(delay_ms); }
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
    std::string getMessage() const { return std::string(message); }
};
