# Arcade
A modular arcade gaming platform that supports multiple display libraries and games.

## 🚀 Features
- Multiple graphical library support (SFML, SDL2, NCurses)
- Modular game system with dynamic loading
- Real-time gameplay with consistent frame rate
- Clean error handling with custom exceptions
- Extensible architecture with documented interfaces
- Cross-platform compatibility

## 📋 Supported Display Libraries
### Graphical Libraries
- SFML (Simple and Fast Multimedia Library)
- SDL2 (Simple DirectMedia Layer)
- NCurses (Text-based interface)

### Core Features
- Dynamic switching between display libraries at runtime
- Real-time game rendering with 60 FPS target
- Robust event handling system
- Window management (800x600 resolution)
- Error handling with custom exception classes
- Menu system with authentication and game selection
- Pause menu functionality with resume/quit options
- Support for text, images, shapes, and buttons rendering

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

## 🚦 Usage
```sh
# Display help
./arcade -h

# Run with a specific graphical library
./arcade lib/arcade_sfml.so
```

## 🎮 Game Controls
| Key | Action |
|-----|--------|
| 1 | Previous graphical library |
| 2 | Next graphical library |
| Enter | Confirm selection |
| Tab | Navigate menu options |
| M | Toggle pause menu |
| Escape | Exit game/menu |
| Arrow keys | Movement/Navigation |
| Backspace | Delete text input |

## 📁 Project Structure
```
arcade/
├── doc/              # Documentation
│   └── library_implementation_guide.md
├── include/          # Header files
│   ├── Core/        # Core system headers
│   ├── Games/       # Game interface headers
│   └── Graphicals/  # Display library headers
├── src/             # Source files
│   ├── Core/        # Core implementation
│   ├── Games/       # Game implementations
│   └── Graphicals/  # Display library implementations
└── assets/          # Resource files
    └── fonts/       # Font files
```

## 🔧 Adding New Components
### Adding a New Game
1. Create new game class inheriting from AGame
2. Implement required game logic in handleEvents()
3. Use element_t structs for rendering
4. Add game library to build system
5. Follow error handling guidelines

### Adding a New Display Library
1. Create new display class inheriting from AGraphical
2. Implement required display methods (init, close, update, draw)
3. Handle window management and events
4. Support all element types (TEXT, IMAGE, CIRCLE, RECTANGLE, BUTTON)
5. Add display library to build system

## 📚 Documentation
Comprehensive documentation for implementing new libraries is available in:
- `doc/library_implementation_guide.md`

## ⚠️ Requirements
- C++ Compiler (supporting C++20)
- SFML development libraries (graphics, window, system)
- SDL2 development libraries (core, ttf, image)
- NCurses development libraries
- Make build system
- DejaVu Sans font (system-wide or in assets/fonts/)

## 🐛 Error Handling
- Custom exception classes for specific error cases
- RAII-compliant resource management
- Graceful error recovery
- Detailed error messages

## 🎨 Supported Elements
### Display Elements
- TEXT: Rendered text with customizable font size and color
- IMAGE: Image display with scaling support
- CIRCLE: Filled circle shapes
- RECTANGLE: Filled rectangle shapes
- BUTTON: Interactive buttons (combination of rectangle and text)

### Color Support
- WHITE (default)
- RED (code: "1")
- GREEN (code: "2")
- YELLOW (code: "3")
- BLUE (code: "4")
- MAGENTA (code: "5")
- CYAN (code: "6")