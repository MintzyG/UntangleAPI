#include "terminal.h"
#include <iostream>

Terminal::Terminal() {}

void Terminal::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex);
    logs.push_back(message);
    
    std::cout << message << std::endl;
    
    if (logs.size() > 10000) {
        logs.erase(logs.begin(), logs.begin() + 1000);
    }
}

void Terminal::clear() {
    std::lock_guard<std::mutex> lock(log_mutex);
    logs.clear();
}

void Terminal::render(float available_height) {
    if (!visible) return;
    
    if (height < 100.0f) height = 100.0f;
    if (height > available_height - 100.0f) height = available_height - 100.0f;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::BeginChild("Terminal", ImVec2(0, height), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    ImGui::Text("Terminal");
    ImGui::SameLine();
    
    if (ImGui::SmallButton("Clear")) {
        clear();
    }
    
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &auto_scroll);
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputText("##filter", filter_buffer, sizeof(filter_buffer));
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Filter logs (case-insensitive)");
    }
    
    ImGui::SameLine(ImGui::GetWindowWidth() - 60);
    if (ImGui::SmallButton("Hide")) {
        visible = false;
    }
    
    ImGui::Separator();
    
    ImGui::InvisibleButton("##resizer", ImVec2(-1, 4));
    if (ImGui::IsItemActive()) {
        height -= ImGui::GetIO().MouseDelta.y;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
    }
    
    ImGui::Separator();
    
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    std::lock_guard<std::mutex> lock(log_mutex);
    
    std::string filter_str = filter_buffer;
    bool has_filter = filter_str.length() > 0;
    
    for (char& c : filter_str) {
        c = tolower(c);
    }
    
    for (const auto& log : logs) {
        if (has_filter) {
            std::string log_lower = log;
            for (char& c : log_lower) {
                c = tolower(c);
            }
            if (log_lower.find(filter_str) == std::string::npos) {
                continue;
            }
        }
        
        ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White default
        
        if (log.find("[ERROR]") != std::string::npos || log.find("ERROR:") != std::string::npos) {
            color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // Red
        } else if (log.find("[WARN]") != std::string::npos || log.find("WARNING:") != std::string::npos) {
            color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f); // Yellow
        } else if (log.find("[EXEC]") != std::string::npos) {
            color = ImVec4(0.3f, 1.0f, 0.3f, 1.0f); // Green
        } else if (log.find("DEBUG") != std::string::npos) {
            color = ImVec4(0.5f, 0.5f, 1.0f, 1.0f); // Blue
        } else if (log.find("Response:") != std::string::npos || log.find("Status") != std::string::npos) {
            color = ImVec4(0.3f, 0.8f, 1.0f, 1.0f); // Cyan
        }
        
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("%s", log.c_str());
        ImGui::PopStyleColor();
    }
    
    if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }
    
    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::PopStyleVar();
}
