#pragma once
#include <string>
#include <vector>
#include <memory>

class Orchestration {
  public:
    int id;
    std::string name;

    Orchestration(int id, const std::string& name);
};

class Project {
  public:
    int id;
    std::string name;
    std::vector<std::unique_ptr<Orchestration>> orchestrations;

    Project(int id, const std::string& name);

    void addOrchestration(const std::string& name);
    void addOrchestrationWithId(int id, const std::string& name);
    void removeOrchestration(int orchestration_id);
    Orchestration* getOrchestration(int orchestration_id);
  private:
    int next_orchestration_id = 1000;
};

class ProjectManager {
  public:
    ProjectManager();

    void addProject(const std::string& name);
    void addProjectWithId(int id, const std::string& name);
    void removeProject(int project_id);
    Project* getProject(int project_id);

    const std::vector<std::unique_ptr<Project>>& getProjects() const { return projects; }

  private:
    std::vector<std::unique_ptr<Project>> projects;
    int next_project_id = 1;
};
