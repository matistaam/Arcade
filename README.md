# Arcade
A modular arcade gaming platform that supports multiple display libraries and games.

## 🚀 Features
- Multiple graphical library support (SFML, SDL2, NCurses) with real-time switching
- Dynamic game loading and switching via shared libraries
- Persistent user accounts with encrypted password storage
- Menu system with login/signup functionality and game selection
- Real-time gameplay with consistent 60 FPS frame rate
- Clean error handling with custom exception hierarchy
- Extensible architecture with documented interfaces
- Cross-platform compatibility

## 🎮 Available Games
### Snake
- Classic snake gameplay with dynamic difficulty
- Progressive speed increase as score grows
- Per-user high score tracking
- Customizable snake and food colors
- Screen boundary teleportation - snake appears on opposite side when crossing screen edge
- Dynamic food spawning with collision detection
- Self-collision detection for game over condition
- Game over and restart functionality

### Nibbler
- Advanced maze-based gameplay with custom map file support
- Advanced input queue system for precision control
- T-junction decision points requiring player interaction
- Dynamic time-based scoring system with hunger mechanics
- Adaptive clock speed that increases challenge over time
- Visual input queue display showing planned movements
- Comprehensive game state display (time, score, hunger)
- Multiple special wall types (turning walls, T-sections)
- Detailed visual feedback for game progress and status
- Win condition based on complete food collection

## 📋 Supported Display Libraries
### Graphical Libraries
- SFML (Simple and Fast Multimedia Library)
  - Hardware accelerated rendering with 60 FPS frame limiting
  - High-quality vector-based text rendering with ByteBounce font
  - Smooth anti-aliased circle and rectangle primitives
  - Texture-based image support with proportional scaling
  - Efficient keyboard and window event handling
  - Dynamic color palette management

- SDL2 (Simple DirectMedia Layer)
  - Hardware accelerated graphics with optimized rendering pipeline
  - Custom TTF font integration with dynamic size support
  - Optimized image loading and hardware-accelerated textures
  - CPU-efficient shape drawing primitives using Bresenham's algorithm
  - Comprehensive input event handling with keyboard mapping
  - Resource management with automatic cleanup

- NCurses (Text-based interface)
  - Terminal-optimized display with 6-color palette support
  - Character-based rendering with adaptive screen dimensions
  - Smart border drawing with centered content positioning
  - Resource-efficient rendering for low-end systems
  - Unicode character support with setlocale integration
  - Optimized screen refresh to minimize terminal flicker

## 🛠️ Installation
### Dependencies
#### Ubuntu/Debian
```sh
# SFML dependencies
sudo apt-get install libsfml-dev

# SDL2 dependencies
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev

# NCurses dependencies
sudo apt-get install libncurses5-dev
```

### Building the Project
```sh
# Build the project
make

# Clean object files
make clean

# Full clean (including libraries)
make fclean

# Rebuild
make re
```

## 🎮 Controls
### Global Controls
| Key | Action |
|-----|--------|
| SPACE | Switch graphics library |
| M | Toggle pause menu |
| ESC | Exit game/menu |

### Menu Controls
| Key | Action |
|-----|--------|
| Enter | Confirm selection |
| Tab | Navigate menu options |
| Backspace | Delete text input |

### Game Controls
| Key | Action |
|-----|--------|
| Arrow Up | Move up |
| Arrow Down | Move down |
| Arrow Left | Move left |
| Arrow Right | Move right |
| R | Restart game |
| ENTER | Restart after game over (Nibbler) |

## 📁 Project Structure
```
arcade/
├── assets/           # Game resources
│   ├── fonts/       # Font files
│   └── NibblerMaps/ # Nibbler game maps
├── doc/             # Documentation
├── include/         # Header files
│   ├── Core/       # Core system headers
│   ├── Games/      # Game interface headers
│   └── Graphicals/ # Display library headers
├── lib/            # Compiled libraries
└── src/            # Source files
    ├── Core/       # Core implementation
    ├── Games/      # Game implementations
    └── Graphicals/ # Display library implementations
```

## 🔧 Architecture
- Core system manages library loading and game state
- Modular design with clear interfaces
- Event-driven architecture
- Score management system
- User authentication system
- Dynamic library loading
- Exception handling system

## 📦 Features in Detail
### User Management
- Account creation and login
- Persistent high scores
- Score tracking per game
- Secure password storage

### Display System
- Window management (800x600 default)
- Multiple render types support
- Real-time display updates
- Custom font rendering
- Shape and image drawing
- Consistent color palette across libraries

### Game Framework
- Consistent game loop with frame timing
- Event-driven input handling
- High score tracking and persistence
- Pause menu integration
- Game state management with transitions
- Advanced collision detection systems
- Direction-based movement with input queuing

## 📚 Documentation
See `doc/library_implementation_guide.md` for detailed information about:
- Implementing new games
- Adding display libraries
- Core system architecture
- Element rendering system
- Best practices

## ⚖️ License
This project is part of the Epitech curriculum.