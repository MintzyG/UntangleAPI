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
    int getId() const;

    void setPosition(ImVec2 pos);
    ImVec2 getPosition() const;
    void updatePosition();
};

class StartNode : public Node {
public:
    StartNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;

};

class GetNode : public Node {
public:
    GetNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
};

class PostNode : public Node {
public:
    PostNode(int nodeId);
    std::vector<int> getAttributeIds() const override;
    void draw() override;
};

