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
- Wrap-around wall collision system
- Dynamic food spawning with collision detection
- Game over and restart functionality

### Nibbler
- Maze-based gameplay with custom map support
- Directional wall mechanics for guided movement
- Strategic food placement and collection
- Advanced collision detection system
- Multiple map layouts via text files
- Win condition based on food collection
- Scoring system based on completion time
- Real-time direction indicator

## 📋 Supported Display Libraries
### Graphical Libraries
- SFML (Simple and Fast Multimedia Library)
  - Hardware accelerated rendering
  - High-quality text rendering with ByteBounce font
  - Smooth circle and rectangle drawing
  - Texture-based image support
  - Window event handling

- SDL2 (Simple DirectMedia Layer)
  - Hardware accelerated graphics
  - Custom TTF font integration
  - Optimized image loading and rendering
  - Efficient shape drawing primitives
  - Event-driven input handling

- NCurses (Text-based interface)
  - Terminal-based display with color support
  - Character-based rendering optimized for text displays
  - Border and special character support
  - Low resource footprint
  - Responsive input handling

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

## 📁 Project Structure
```
arcade/
├── assets/           # Game resources
│   ├── fonts/       # Font files
│   ├── menu.jpg    # Menu background
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

### Game Framework
- Consistent game loop
- Event handling system
- Score tracking
- Pause functionality
- State management
- Collision detection

## 📚 Documentation
See `doc/library_implementation_guide.md` for detailed information about:
- Implementing new games
- Adding display libraries
- Core system architecture
- Element rendering system
- Best practices

## ⚖️ License
This project is part of the Epitech curriculum.