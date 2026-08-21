# Void Breaker

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Qt 6](https://img.shields.io/badge/Qt-6.x-green.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-orange.svg)
![License](https://img.shields.io/badge/License-MIT-purple.svg)
![Build Status](https://github.com/Sarayu705/VoidBreaker-AsteroidShootingGame/actions/workflows/build.yml/badge.svg)

A high-performance **2D Arcade Space Shooter & PvP Battle Game** written in **C++17** and **Qt 6**. Built with custom pixel-art raster rendering, 2-player modes, item drop systems, procedural background generation, dynamic difficulty scaling, and 8-bit aesthetic particle physics.

---

## 📥 Download Pre-Compiled Executables

Recruiters and players can run the game directly on **Windows** or **macOS** without installing Qt or C++ compilers:

1. Go to the **[Releases Section](https://github.com/Sarayu705/VoidBreaker-AsteroidShootingGame/releases)** on this GitHub repository.
2. Download the package for your OS:
   - **Windows**: `VoidBreaker-Windows.zip` (Extract & run `VoidBreaker.exe`)
   - **macOS**: `VoidBreaker-macOS.zip` (Open & run `VoidBreaker.app`)
3. Play instantly!

---

## Key Features & Gameplay Mechanics

### 🎮 Game Modes & Menu Dashboard
- **Start Dashboard Menu**: On startup, an arcade menu lets players choose between **Single Player Mode** and **1 VS 1 PvP Battle Mode** using `1`/`2` keys or `UP`/`DOWN` arrows.
- **Single Player Survival**: Pilot your spacecraft against infinite waves of space hazards with dynamic round progressions.
- **1 VS 1 PvP Battle Mode**: Head-to-head competitive arena on the same screen! Player lasers deal PvP damage (-20 HP), award bonus score (+50 pts), and trigger custom player victory overlays (`PLAYER 1 WINS!` / `PLAYER 2 WINS!`).

---

###  Scoring Rules & Hazard Mechanics

| Action / Event | Effect | Details |
| :--- | :--- | :--- |
| **Destroy Small Asteroid** | **+30 Points** | High-point small target |
| **Destroy Medium Asteroid** | **+20 Points** | Splits into 2 small asteroids |
| **Destroy Large Asteroid** | **+10 Points** | Splits into 2 medium asteroids |
| **Destroy Space Mine** | **+100 Points** | High reward for destroying sine-wave mines |
| **Land PvP Laser Hit (1v1)** | **+50 Points** | Hitting opponent ship in 1v1 mode |
| **Asteroid Collision** | **-12 HP Damage** | Direct collision with asteroid hazard |
| **Asteroid Leak Penalty** | **-12 HP Damage** | Failing to hit an asteroid before it crosses the bottom boundary |
| **Mine Collision** | **-1 Life Instant Kill** | Hitting a mine triggers an explosion and costs 1 full life |

---

### ⚡ Power-Up Drop System
Destroying space hazards has a 25% drop chance to spawn item capsules:
- **Triple Shot (`3X`)**: Upgrades cannon to fire 3 angled spread lasers simultaneously for 10 seconds.
- **Plasma Shield (`SH`)**: Renders a translucent cyan energy shield around the ship that absorbs 1 collision hit completely.
- **Health Pack (`HP`)**: Restores **+25 HP** to the player's health bar.

---

### 6 Procedural Background Themes & FX
- **Themes**: *Deep Galaxy*, *Nebula Storm* (with lightning fibers & scanlines), *Cyberpunk Neon Grid*, *Alien Moon Atmosphere*, *Toxic Alien Void*, and *Solar Flare Atmosphere*.
- **Visual FX**: Dynamic screen shake on damage/explosions, animated particle debris physics, 2-layer scrolling starfield, and an optional retro raster grid toggle.

---

## Technical Architecture & Design Patterns

The codebase strictly adheres to object-oriented programming (OOP) principles and idiomatic C++17 design patterns:

### 1. Model-View Architecture (`QGraphicsScene` / `QGraphicsView`)
- **`Game`** ([Game.h](src/Game.h)): Central state controller managing the 60 FPS tick loop (`QTimer`), spatial collisions, power-up item drops, procedural themes, and 1v1 match evaluation.
- **`Bullet`** ([Bullet.h](src/Bullet.h)): Supports projectile ownership (`m_ownerIndex`), player-colored laser vectors, and PvP hit collisions.
- **`Player`** ([Player.h](src/Player.h)): Multi-entity spacecraft controller supporting independent key bindings, sprite palette themes, spread laser vectors, and plasma shield bubbles.
- **`PowerUp`** ([PowerUp.h](src/PowerUp.h)): Object-oriented item entity supporting floating animations, collision triggers, and power-up state applications.
- **`HUD`** ([HUD.h](src/HUD.h)): Dual-player graphics overlay tracking independent P1/P2 live scores, HP bars, lives counters, and player victory overlays.

### 2. Centralized Input Dispatcher (`eventFilter`)
- Uses a central event filter on `QGraphicsView` to route keyboard events to P1 (`WASD`/`Space`) and P2 (`Arrows`/`Enter`) simultaneously, bypassing Qt's single-item focus limitation.

---

## Game Controls

| Player | Ship Theme | Movement | Fire Cannon |
| :--- | :--- | :--- | :--- |
| **Player 1** | 🚀 **Blue Falcon** (Steel Blue & Cyan) | `A` / `D` | `Space` / `Z` |
| **Player 2** | 🚀 **Red Phoenix** (Crimson & Gold) | `←` / `→` (Arrow Keys) | `Enter` / `M` |

- `1` / `2` or `UP` / `DOWN`: Select Mode on Start Menu
- `P` or `Esc`: Pause / Resume Game
- `G`: Toggle Retro Raster Grid Overlay

---

## Building from Source

### Prerequisites
- **C++17 Compatible Compiler** (MSVC 2019+, GCC 10+, or Clang 12+)
- **CMake 3.16 or higher**
- **Qt 6.x** (Core, Gui, Widgets)

### 1. Command Line Build (CMake + Ninja / Make)

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/VoidBreaker.git
cd VoidBreaker

# Configure CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build executable
cmake --build build --config Release
```

The compiled binary will be located in the `build/` directory:
- **macOS**: `build/VoidBreaker.app`
- **Windows**: `build/Release/VoidBreaker.exe`
- **Linux**: `build/VoidBreaker`

### 2. Qt Creator IDE
1. Open Qt Creator.
2. Select **Open File or Project...** and choose `CMakeLists.txt`.
3. Select your Qt 6 Kit (e.g., Desktop Qt 6.x.x Clang/MSVC).
4. Click **Run** (`Ctrl+R` / `Cmd+R`).

---

