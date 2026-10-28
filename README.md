# UntangleAPI

A desktop node editor for building and running HTTP API workflows visually. Instead of writing scripts, you wire nodes together into a graph, run it, and watch the result in a built-in terminal.

> **Archived.** This project is no longer developed. It still builds, but it's unfinished.

## Concepts

- **Projects** hold one or more **orchestrations**.
- An **orchestration** is a node graph that runs from its **Start** node, following the links.
- Everything is saved locally in `untangle.db` (SQLite): projects, orchestrations, nodes, links and node state.

## Nodes

| Node | What it does |
|---|---|
| Start | Entry point of an orchestration |
| HTTP GET / POST / PUT / DELETE | Sends a request with libcurl |
| JSON Extract | Pulls a value out of a JSON response |
| Set Variable / Get Variable | Stores and reads values shared across the run |
| If Condition | Branches the flow |
| Delay | Waits before continuing |
| Assert | Checks a value and fails the run if it doesn't match |
| Log | Prints to the terminal |

## Features

- Sidebar to create, open and delete projects and orchestrations
- Node graph editing with zoom and a collapsible sidebar
- Graph execution with output in a built-in terminal panel
- Saving and loading everything, including each node's state

## Building

Requires CMake 3.20+, a C++20 compiler, and these system packages: SDL2, OpenGL, SQLite3 and libcurl. Dear ImGui and ImNodes are downloaded by CMake.

```sh
cmake -B build
cmake --build build
./build/imgui_minimal
```

## Built with

C++20, [Dear ImGui](https://github.com/ocornut/imgui) (SDL2 + OpenGL 3 backends), [ImNodes](https://github.com/Nelarius/imnodes), SQLite and libcurl.
