# Arcade Architecture

```mermaid
classDiagram
    direction TB

    class ICore {
        <<Interface>>
        +setGraphical(IGraphical*)
        +setGame(IGame*)
        +display(vector~element_t~)
        #_graphical: IGraphical*
        #_game: IGame*
    }
    class ACore {
        -_handle: void*
        -_graphical: IGraphical*
        -_game: IGame*
        +ACore(string path)
        +~ACore()
        +setGraphical(IGraphical*)
        +setGame(IGame*)
        +display(vector~element_t~)
        +update() string
    }
    
    class IGraphical {
        <<Interface>>
        +init()
        +close()
        +update() string
        +draw()
        +clearElements()
        +addElements(vector~element_t~)
        #_elements: vector~element_t~
    }
    class AGraphical {
        +AGraphical(string path)
        +~AGraphical()
        +init()
        +close()
        +update() string
        +draw()
    }
    
    class IGame {
        <<Interface>>
        +handleEvents(string) vector~element_t~
        #_elements: vector~element_t~
    }
    class AGame {
        +AGame()
        +~AGame()
        #_elements: vector~element_t~
    }

    %% Graphics Libraries Implementations
    class SFML {
        -_window: RenderWindow*
        -_font: Font
        -_texture: Texture
        -_width: int
        -_height: int
        +init()
        +close()
        +update() string
        +draw()
        -draw_text(element_t)
        -draw_image(element_t)
        -draw_circle(element_t)
        -draw_rectangle(element_t)
    }
    class SDL {
        -_window: Window*
        -_renderer: Renderer*
        -_font: Font*
        -_width: int
        -_height: int
        +init()
        +close()
        +update() string
        +draw()
        -draw_text(element_t)
        -draw_image(element_t)
        -draw_circle(element_t)
        -draw_rectangle(element_t)
    }
    class NCurses {
        +init()
        +close()
        +update() string
        +draw()
        -draw_text(element_t)
        -draw_image(element_t)
        -draw_circle(element_t)
        -draw_rectangle(element_t)
    }

    %% Game Implementations
    class Snake {
        +handleEvents(string) vector~element_t~
        -updateGame()
        -checkCollision()
    }
    class Pacman {
        +handleEvents(string) vector~element_t~
        -updateGame()
        -checkCollision()
    }
    class Nibbler {
        +handleEvents(string) vector~element_t~
        -updateGame()
        -checkCollision()
    }

    %% Inheritance relationships
    ICore <|-- ACore
    IGraphical <|-- AGraphical
    AGraphical <|-- SFML
    AGraphical <|-- SDL
    AGraphical <|-- NCurses
    IGame <|-- AGame
    AGame <|-- Snake
    AGame <|-- Pacman
    AGame <|-- Nibbler

    %% Composition relationships
    ACore o-- IGraphical : uses
    ACore o-- IGame : uses

    %% Element Structure
    class element_t {
        +ELEMENT_TYPE type
        +string text
        +tuple~int,int~ position
        +string color
        +tuple~int,int~ size
        +string image_path
    }

    IGame --> element_t : creates
    IGraphical --> element_t : displays

    %% Element Types
    class ELEMENT_TYPE {
        <<enumeration>>
        TEXT
        IMAGE
        CIRCLE
        RECTANGLE
        BUTTON
    }

    element_t --> ELEMENT_TYPE : uses
```

## Architecture Explanation

### Core System
- **ACore** is the central component that:
  - Dynamically loads graphic libraries (*.so)
  - Manages game loading
  - Coordinates display and updates
  - Handles library switching

### Graphics Interface
- **IGraphical** defines the common interface for all graphic libraries
- **AGraphical** provides the base implementation
- Three libraries implemented: SFML, SDL, NCurses
- Each library can be loaded dynamically at runtime

### Game System
- **IGame** defines the common interface for all games
- **AGame** provides the base implementation for games
- Games are independent of the graphic library used
- Communication through element_t structure

### Data Flow
1. Core loads a graphic library
2. Core loads a game
3. Game receives events through handleEvents()
4. Game generates elements (element_t)
5. Core passes these elements to the graphic library
6. Graphic library displays the elements

### element_t Structure
- Common structure used for communication
- Defines all displayable element types
- Provides abstraction between games and display
- Ensures consistency across different graphics libraries