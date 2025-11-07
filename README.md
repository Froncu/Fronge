# Fronge

A modern 2D game engine written in C++23, featuring custom solutions across the stack. Built as a solo hobby project with a focus on learning through implementation.

## Features

- **Custom ECS Architecture**: High-performance Entity Component System built on sparse set data structures for optimal cache efficiency and iteration speed.
- **Integrated Physics**: Custom physics system designed to work seamlessly with the ECS architecture.
- **Flexible Input System**: Keyboard and gamepad inputs are routed to per-user controllers, allowing dynamic device assignment and customizable action mappings.
- **Thread-Safe Logging**: Centralized logging class ensuring safe access from multiple threads. Supports source-directory mapping.
- **Smart References**: Pointer-like class that automatically updates its address when the object it points to is moved or becomes `nullptr` if the object is destroyed.
- **Event System**: Event listener and dispatcher classes for decoupling systems through event handling.

## Philosophy

This engine is developed with three core principles:

- **Learning by Building**: Relies only on minimal foundational libraries for platform and tooling support; everything else is built from scratch.
- **Abstraction First**: The dependencies are wrapped behind clean, engine-specific interfaces to maintain flexibility and control over the API.
- **Modern C++**: Written in C++23 with a focus on clean design, safety, and performance through modern language features. Future updates will explore C++26 to leverage compile-time reflection for serialization of components, scenes, and other engine data.

## Dependencies

- [SDL3](https://github.com/libsdl-org/SDL) - Core functionality (entry point, system events, input, windowing, ...)
- [SDL3_image](https://github.com/libsdl-org/SDL_image) - Image loading support
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf) - Font rendering and text support
- [Dear ImGui](https://github.com/ocornut/imgui) - Editor UI

Those are managed through my custom [vcpkg](https://github.com/microsoft/vcpkg) fork and will be automatically installed during the configuration process.

## Quickstart

### Requirements

- C++23 compatible compiler
- CMake 3.15 or higher

### Cloning

Clone the repository **with** the `--recursive` flag. This will cause the vcpkg submodule to also be pulled:

```bash
git clone https://github.com/Froncu/Fronge --recursive
```

### Configuring & Building

The project includes CMake presets (`CMakePresets.json`) that configure vcpkg integration automatically. You can build using your preferred IDE or command line.

**Note:** If creating custom configurations, ensure they inherit from the provided `base`, `debug` and/or `release` preset to maintain proper vcpkg integration.

### Usage

The engine is distributed as a shared library and must be linked to an executable project. [Sandbox](https://github.com/Froncu/Sandbox) serves as a reference implementation, demonstrating how to integrate and use the engine in your own applications. There, the engine is included as a Git submodule for ease of development. However, other users may prefer using CMake’s FetchContent for now as I would love to make this available on some package managers in the future.