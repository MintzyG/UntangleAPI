cmake_minimum_required(VERSION 3.20)
project(imgui_minimal LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ---- Fetch ImGui ----
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.8
)
FetchContent_MakeAvailable(imgui)

# ---- Fetch ImNodes ----
FetchContent_Declare(
  imnodes
  GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
  GIT_TAG master
)
# just populate, don't "make available"
FetchContent_Populate(imnodes)

# ---- Backends ----
set(IMGUI_BACKENDS
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# ---- SDL2 + OpenGL (system packages on Arch) ----
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(imgui_minimal
  src/main.cpp
  src/app.cpp
  src/node_editor.cpp
  src/ui_manager.cpp
  src/sidebar.cpp
  src/project.cpp
  src/renderer.cpp
  src/nodes.cpp
  src/link.cpp
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${IMGUI_BACKENDS}

  # ---- ImNodes ----
  ${imnodes_SOURCE_DIR}/imnodes.cpp
)

target_include_directories(imgui_minimal PRIVATE
  include
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
  ${SDL2_INCLUDE_DIRS}
  # so we can include <imnodes.h>
  ${imnodes_SOURCE_DIR}
)

target_link_libraries(imgui_minimal PRIVATE
  SDL2::SDL2
  OpenGL::GL
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(imgui_minimal PRIVATE -Wall -Wextra -Wpedantic)
endif()
