#pragma once
#include "imgui.h"
#include <string>
#include <vector>
#include <mutex>

class Terminal {
public:
    Terminal();
    
    void render(float available_height);
    void log(const std::string& message);
    void clear();
    
    bool isVisible() const { return visible; }
    void setVisible(bool vis) { visible = vis; }
    void toggleVisible() { visible = !visible; }
    
    float getHeight() const { return height; }
    void setHeight(float h) { height = h; }
    
private:
    std::vector<std::string> logs;
    std::mutex log_mutex;
    bool visible = true;
    float height = 200.0f;
    bool auto_scroll = true;
    char filter_buffer[256] = "";
};
