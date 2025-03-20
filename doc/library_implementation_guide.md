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
3. Provide `create()` and `destroy()` external C functions

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
            void init() override;
            void close() override;
            std::string update() override;
            void draw() override;

            // Helper methods for drawing elements
            void draw_text(element_t element);
            void draw_image(element_t element);
            void draw_circle(element_t element);
            void draw_rectangle(element_t element);

        private:
            // Your library-specific members
    };
}
```

### 3. Required Method Implementations

#### init()
- Initialize your graphics context
- Set up windows, renderers, or other required resources
- Set window dimensions (default: 800x600)
- Set framerate limit (60 FPS recommended)
- Handle any necessary error checking
- Initialize color support if needed (for NCurses)

#### close()
- Clean up all resources
- Close windows and destroy contexts
- Free allocated memory

#### update()
- Handle input events
- Return "EXIT" to quit
- Return "RESIZE" for window resize events
- Return empty string for no special events

#### draw()
- Clear the screen/buffer
- Draw all elements from _elements vector
- Handle different element types (TEXT, IMAGE, CIRCLE, RECTANGLE, BUTTON)
- Display/present the rendered frame

### 4. External C Functions
You must provide these functions for dynamic loading:
```cpp
extern "C" {
    arc::IGraphical *create() {
        return new arc::YourGraphicsLib();
    }

    void destroy(arc::IGraphical *instance) {
        delete instance;
    }
}
```

## Game Library Implementation

### 1. Required Files
For a new game, create:
- A header file (.hpp) defining your game class
- A source file (.cpp) implementing the game logic

### 2. Class Structure
Your game must:
1. Inherit from `arc::AGame` or implement `arc::IGame`
2. Implement all required virtual methods
3. Provide `create()` and `destroy()` external C functions
4. Include virtual destructor

Example header structure:
```cpp
namespace arc {
    class YourGame : public AGame {
        public:
            YourGame();
            ~YourGame() override;

            // Required override from IGame
            std::vector<element_t> handleEvents(std::string command) override;

        private:
            // Game-specific members and methods
    };
}
```

### 3. Required Method Implementation

#### handleEvents(std::string command)
- Process the input command (empty string means no input)
- Update game state based on command
- Perform game logic (movement, collisions, etc.)
- Return vector of elements to be rendered
- Implement proper cleanup in destructor

### 4. Element Structure
Use the `element_t` struct for rendering:
```cpp
struct element_s {
    ELEMENT_TYPE _type;         // TEXT, IMAGE, CIRCLE, RECTANGLE, BUTTON
    std::string _text;          // Text content for TEXT elements
    std::tuple<int, int> _position;  // (x, y) position
    std::string _color;         // Color identifier (1-6)
    std::tuple<int, int> _size;      // Width/height or radius
    std::string _image_path;    // Path for IMAGE elements
};
```

Color codes:
- "1": Red
- "2": Green
- "3": Yellow
- "4": Blue
- "5": Magenta
- "6": Cyan
Default: White

For TEXT elements, position represents the top-left corner.
For CIRCLE elements, position represents the center and size.x is used as diameter.
For RECTANGLE elements, position represents the center and size represents width/height.

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
Libraries are loaded dynamically at runtime:
- Graphics libraries: `lib/graphicals/arcade_yourlib.so`
- Games: `lib/games/arcade_yourgame.so`

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