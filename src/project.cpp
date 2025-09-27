#include "project.h"
#include <algorithm>

// -------------------- Orchestration --------------------
Orchestration::Orchestration(int id, const std::string& name)
  : id(id), name(name) {}

// -------------------- Project --------------------
Project::Project(int id, const std::string& name)
  : id(id), name(name) {}

void Project::addOrchestration(const std::string& name) {
  orchestrations.push_back(std::make_unique<Orchestration>(next_orchestration_id++, name));
}

Orchestration* Project::getOrchestration(int orchestration_id) {
  for(auto& orch : orchestrations) {
    if (orch->id == orchestration_id) {
      return orch.get();
    }
  }
  return nullptr;
}

// -------------------- ProjectManager --------------------
ProjectManager::ProjectManager() {}

void ProjectManager::addProject(const std::string& name) {
  projects.push_back(std::make_unique<Project>(next_project_id++, name));
}

Project* ProjectManager::getProject(int project_id) {
  for(auto& proj : projects) {
    if (proj->id == project_id) {
      return proj.get();
    }
  }
  return nullptr;
}


