# Arcade Library Implementation Guide

## Table of Contents
1. [Introduction](#introduction)
2. [Graphics Library Implementation](#graphics-library-implementation)
3. [Game Library Implementation](#game-library-implementation)
4. [Building and Loading Libraries](#building-and-loading-libraries)

## Introduction

The Arcade project is designed with extensibility in mind, allowing developers to easily add new graphics libraries and games. This document explains how to implement new libraries compatible with the system.

## Graphics Library Implementation

### 1. Required Files
To implement a new graphics library, you need to create:
- A header file (.hpp) defining your library class
- A source file (.cpp) implementing the library functionality

### 2. Class Structure
Your graphics library must:
1. Inherit from `arc::AGraphical` or implement `arc::IGraphical`
2. Implement all required virtual methods
3. Provide `create()`, `destroy()`, and `get_type()` external C functions

Default window dimensions should be:
- Width: 800 pixels
- Height: 600 pixels

Example header structure:
```cpp
namespace arc {
    class YourGraphicsLib : public AGraphical {
        public:
            YourGraphicsLib();
            ~YourGraphicsLib();

            // Required overrides from IGraphical
            std::string getEvents() override;
            void draw() override;

            // Helper methods for drawing elements
            void draw_text(element_t element);
            void draw_image(element_t element);
            void draw_circle(element_t element);
            void draw_rectangle(element_t element);
            void draw_border(element_t element);  // Optional for border support

        private:
            // Position conversion helpers
            std::tuple<int, int> convertPositionToPixels(int percentX, int percentY);

            // Library-specific members for window/renderer
            int _width = 800;
            int _height = 600;
    };
}
```

### 3. Required Method Implementations

#### Constructor
- Initialize your graphics context in the constructor
- Set up windows, renderers, or other required resources
- Set window dimensions (default: 800x600)
- Set framerate limit (60 FPS recommended)
- Load ByteBounce font from assets/fonts/ByteBounce.ttf
- Initialize hardware acceleration if supported
- Handle any necessary error checking
- Initialize color support (required for all libraries)
- Throw GraphicalError with descriptive message on failure

#### Destructor
- Clean up all resources in reverse order of initialization
- Close windows and destroy contexts
- Free allocated memory
- Handle library-specific cleanup (TTF_Quit, SDL_Quit, etc.)

#### getEvents()
- Handle input events with consistent mapping:
  - Arrow keys: "UP", "DOWN", "LEFT", "RIGHT"
  - Enter: "ENTER"
  - Tab: "TAB"
  - Backspace: "BACKSPACE"
  - Escape: "EXIT"
  - Space: "SWITCH_LIB"
  - m: "m" (for menu)
  - r: "r" (for restart)
  - Standard ASCII characters: Return as single character string
- Return empty string for no events
- Support window events (RESIZE, EXIT)

#### draw()
- Clear the screen/buffer with black background
- Process elements vector in order:
  1. Background images
  2. Game elements (walls, food, snake segments)
  3. UI elements (text, scores)
  4. Border elements
- Support all element types:
  - TEXT: Render with ByteBounce font at specified size
  - IMAGE: Load and scale textures
  - CIRCLE: Fill with solid color
  - RECTANGLE: Fill with solid color
  - BUTTON: Combine rectangle and text
  - BORDER: Draw border with specified color
- Handle standard colors (1-6):
  - "1": Red (RGB: 255, 0, 0)
  - "2": Green (RGB: 0, 255, 0)
  - "3": Yellow (RGB: 255, 255, 0)
  - "4": Blue (RGB: 0, 0, 255)
  - "5": Magenta (RGB: 255, 0, 255)
  - "6": Cyan (RGB: 0, 255, 255)
  Default: White (RGB: 255, 255, 255)

### 4. External C Functions
You must provide these functions for dynamic loading:
```cpp
extern "C" {
    arc::IGraphical *create() {
        return (new arc::YourGraphicsLib());
    }

    void destroy(arc::IGraphical *instance) {
        delete instance;
    }

    const char* get_type() {
        return ("graphical");
    }
}
```

## Game Library Implementation

### 1. Required Files
For a new game, create:
- A header file (.hpp) defining your game class
- A source file (.cpp) implementing the game logic
- Map files if needed (place in assets/YourGame/maps/)

### 2. Class Structure
Your game must:
1. Inherit from `arc::AGame` 
2. Implement all required virtual methods
3. Provide `create()`, `destroy()` and `get_type()` external C functions
4. Include proper state management and timing controls

Example header structure:
```cpp
namespace arc {
    class YourGame : public AGame {
        public:
            YourGame(std::string username, int highScore);
            ~YourGame();

            std::vector<element_t> handleEvents(std::string command) override;

        private:
            // Game logic methods
            void restartGame();
            void updateGame();
            bool checkCollision();
            std::vector<element_t> createElements();

            // Constants
            static const int WIDTH = 40;
            static const int HEIGHT = 30;
            static const int CELL_SIZE = 20;

            // Game state
            std::string _username;
            int _score;
            int _highScore;
            int _updateInterval;
            bool _gameOver;
            std::chrono::time_point<std::chrono::steady_clock> _lastUpdateTime;
    };
}
```

### 3. Required Method Implementations

#### Constructor
- The constructor takes username and highScore parameters:
  ```cpp
  YourGame(std::string username, int highScore) : AGame(username, highScore)
  ```
- Initialize game state variables (_score, _gameOver, etc.)
- Set up initial timers with std::chrono::steady_clock
- Set up game elements (snake segments, walls, food, etc.)
- Load any required maps or assets

#### handleEvents(std::string command)
- Process the input command (UP, DOWN, LEFT, RIGHT, etc.)
- Update game state based on elapsed time using std::chrono
- Handle movement, collisions, and scoring
- Check win/lose conditions
- Return a vector of elements for rendering

### 4. Game State Management

#### Game Loop Timing
- Use std::chrono for consistent timing
- Implement frame-rate independent movement

#### State Transitions
- Handle game over conditions
- Implement win state checks
- Support pause/resume functionality
- Allow game restart

### 5. External C Functions
```cpp
extern "C" {
    arc::IGame *create(const std::string &username, int highScore) {
        return (new arc::YourGame(username, highScore));
    }

    void destroy(arc::IGame *instance) {
        delete instance;
    }

    const char *get_type() {
        return ("game");
    }
}
```

### 6. Element Structure
Use the `element_t` struct for rendering:
```cpp
typedef struct element_s {
    ELEMENT_TYPE _type;        // TEXT, IMAGE, CIRCLE, RECTANGLE, BUTTON, BORDER
    std::string _text;         // Text content for TEXT elements
    std::tuple<int, int> _position;  // Position (interpreted differently per type)
    std::string _color;        // Color identifier (1-6)
    std::tuple<int, int> _size;      // Width/height or radius
    std::string _image_path;   // Path for IMAGE elements
    int _font_size = 24;       // Font size for TEXT elements
} element_t;
```

Color codes:
- "1": Red (RGB: 255, 0, 0)
- "2": Green (RGB: 0, 255, 0)
- "3": Yellow (RGB: 255, 255, 0)
- "4": Blue (RGB: 0, 0, 255)
- "5": Magenta (RGB: 255, 0, 255)
- "6": Cyan (RGB: 0, 255, 255)
Default: White (RGB: 255, 255, 255)

Position values are percentages of screen dimensions (0-100), which each library will convert to pixel coordinates. This allows for consistent layouts across different graphics libraries.

## Building and Loading Libraries

### 1. Directory Structure
Place your files in:
- Graphics libraries: `src/Graphicals/YourLib/` and `include/Graphicals/YourLib/`
- Games: `src/Games/YourGame/` and `include/Games/YourGame/`

Font files should be accessed from:
- Default path: `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`
- Fallback path: `assets/fonts/DejaVuSans.ttf`

### 2. Compilation
Add your library to the Makefile:
1. Add library-specific flags if needed
2. Update the compilation rules
3. Ensure shared library (.so) output

### 3. Loading
Libraries are loaded from:
- Graphics libraries: `lib/arcade_yourlib.so`
- Games: `lib/arcade_yourgame.so`

## Best Practices

1. Error Handling
   - Use try-catch blocks for initialization
   - Return meaningful error messages
   - Clean up resources on failure

2. Resource Management
   - Follow RAII principles
   - Clean up in destructor
   - Handle memory carefully

3. Cross-Platform Compatibility
   - Use standard C++ features
   - Handle platform-specific code carefully
   - Document any platform requirements
