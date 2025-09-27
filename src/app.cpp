#include "app.h"
#include <stdio.h>

App::App() {}

App::~App() {
  cleanup();
}

bool App::initialize() {
  if (!renderer.initialize()) {
    printf("Failed to initialize renderer\n");
    return false;
  }

  if (!ui_manager.initialize(renderer.getWindow(), renderer.getGLContext())) {
    printf("Failed to initialize UI manager\n");
    return false;
  }

  if (!node_editor.initialize()) {
    printf("Failed to initialize node editor\n");
    return false;
  }

  return true;
}

void App::cleanup() {
  node_editor.shutdown();
  ui_manager.shutdown();
}

void App::handleEvents(bool& done) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ui_manager.processEvent(event);

    if (event.type == SDL_QUIT) {
      done = true;
    }

    if (event.type == SDL_WINDOWEVENT) {
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        done = true;
      }
    }
  }
}

void App::update() {
  // update application state
}

void App::render() {
  renderer.beginFrame();
  renderer.clear();

  ui_manager.newFrame();

  sidebar.render();

  node_editor.showEditor("Node Editor");

  ui_manager.render();
  renderer.endFrame();
}

int App::run() {
  if (!initialize()) {
    printf("Failed to initialize application\n");
    return -1;
  }

  bool done = false;

  while (!done) {
    handleEvents(done);
    update();
    render();
  }

  return 0;
}
