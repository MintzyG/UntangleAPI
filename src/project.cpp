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

void Project::addOrchestrationWithId(int id, const std::string& name) {
  orchestrations.push_back(std::make_unique<Orchestration>(id, name));
  
  // Update next_orchestration_id if necessary
  if (id >= next_orchestration_id) {
    next_orchestration_id = id + 1;
  }
}

void Project::removeOrchestration(int orchestration_id) {
  orchestrations.erase(
      std::remove_if(orchestrations.begin(), orchestrations.end(),
        [orchestration_id](const std::unique_ptr<Orchestration>& o) {
        return o->id == orchestration_id;
        }),
      orchestrations.end()
      );
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

void ProjectManager::addProjectWithId(int id, const std::string& name) {
  projects.push_back(std::make_unique<Project>(id, name));
  
  // Update next_project_id if necessary
  if (id >= next_project_id) {
    next_project_id = id + 1;
  }
}

void ProjectManager::removeProject(int project_id) {
  projects.erase(
      std::remove_if(projects.begin(), projects.end(),
        [project_id](const std::unique_ptr<Project>& p) {
        return p->id == project_id;
        }),
      projects.end()
      );
}

Project* ProjectManager::getProject(int project_id) {
  for(auto& proj : projects) {
    if (proj->id == project_id) {
      return proj.get();
    }
  }
  return nullptr;
}
