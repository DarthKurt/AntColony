# 🐜 Ant Colony Simulation

## 📌 Overview

A simulation of ant behavior in search of food using pheromones. Ants move across a grid, leave pheromone trails, and interact with their environment, mimicking real colony behavior.

## 🔁 Core Principles

- **Ants search for food**: They start from the nest and move randomly.
- **Pheromones**: When an ant finds food, it returns to the nest, leaving a pheromone trail.
- **Pheromone evaporation**: Over time, pheromone traces fade to prevent "stuck" paths.
- **Following pheromones**: Other ants are more likely to follow stronger trails.

## 🧱 Simulation Structure

| Element    | Description |
|------------|------------|
| 🟩 **Nest** | The starting point for all ants |
| 🟨 **Food** | The target that ants must find |
| 🐜 **Ant** | An agent that moves across the grid |
| 🔵 **Pheromones** | Trails left by ants, visualized with color |

## 🧠 Ant Behavior

- If **no pheromones** → move randomly.
- If **pheromones are present** → move toward the strongest concentration.
- If **food is found** → return to the nest, leaving pheromones behind.
- If **in the nest with food** → start searching for food again.

## ⚙️ Expansion Possibilities

- Multiple **food sources**.
- **Obstacles** in the path.
- Different **ant types** (scouts, workers).
- **Statistics tracking**: route efficiency, collected food amount.
- **Adapting to environmental changes**.

## 💻 Implementation

- **Languages**: C++.
- **Rendering**: OpenGL.

## Getting started

Set wrokspace for target paltform

```sh
# MacOS (x64)
cmake -B ./build-macos-x64 -D TARGET_PLATFORM=macOS-x64

# MacOS (ARM)
cmake -B ./build-macos-arm64 -D TARGET_PLATFORM=macOS-arm64

# Linux (x64)
cmake -B ./build-linux-x64 -D TARGET_PLATFORM=linux-x64

# Linux (ARM)
cmake -B ./build-linux-arm64 -D TARGET_PLATFORM=linux-arm64

# Windows (ARM)
cmake -B ./build-win-arm64 -D TARGET_PLATFORM=windows-arm64

# Windows (x64)
cmake -B ./build-win-arm64 -D TARGET_PLATFORM=windows-x64
```
