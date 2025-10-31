# Fronge

A modern 2D game engine written in C++23, featuring a custom Entity Component System (ECS) architecture and integrated physics simulation.

## Features

- **Custom ECS Architecture**: High-performance Entity Component System built on sparse set data structures for optimal cache efficiency and iteration speed
- **Modern C++23**: Leverages cutting-edge C++ features including `std::views::zip` used inside the `Groups` implementation in the above mentioned ECS system
- **Integrated Physics**: Custom physics system designed to work seamlessly with the ECS architecture
- **SDL3 Rendering**: Hardware-accelerated 2D rendering using SDL3 with plans of switching to the SDL_GPU API
- **Rich Media Support**: Image loading via SDL3_image and text rendering through SDL3_ttf

## Architecture

### Entity Component System

The engine implements a sparse set-based ECS architecture, providing:

- **O(1) component access** - Fast component lookups using sparse sets
- **Cache-friendly iteration** - Contiguous memory layout for efficient system processing
- **Flexible composition** - Easy entity creation through component composition

### Physics System

The integrated physics system is built from the ground up to leverage the ECS architecture, offering:

- Native integration with ECS components
- Efficient collision detection and response
- Customizable physics behaviors through component data

## Quickstart

### Requirements

- C++23 compatible compiler
- CMake 3.15 or higher

### Cloning

Clone the repository **with** the `--recursive` flag. This will cause the vcpkg submodule to also be pulled:

```bash
git clone https://github.com/Froncu/Fronge --recursive
```

### Building

The project includes CMake presets (`CMakePresets.json`) that configure vcpkg integration automatically. You can build using your preferred IDE or command line.

**Note:** If creating custom configurations, ensure they inherit from the provided presets to maintain proper vcpkg integration.

### Usage

The engine is distributed as a shared library and must be linked to an executable project. [Sandbox](https://github.com/Froncu/Sandbox) serves as a reference implementation, demonstrating how to integrate and use the engine in your own applications.

## Dependencies

- [SDL3](https://github.com/libsdl-org/SDL) - Core rendering and windowing
- [SDL3_image](https://github.com/libsdl-org/SDL_image) - Image loading support
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf) - TrueType font rendering

All dependencies are managed through [vcpkg](https://github.com/microsoft/vcpkg) and will be automatically installed during the build process.