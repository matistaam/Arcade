# Technical Documentation - Arcade Architecture

## 🎮 System Overview

The Arcade project implements a modular gaming system using dynamically loaded libraries for both games and graphical displays.

## 📊 Architecture Diagram

![Architecture Diagram](diagram.png)

## 🔧 Technical Architecture

### Core Components

#### Core Engine
- Manages game states and library transitions
- Handles dynamic library loading/unloading
- Coordinates communication between games and graphics

#### Library System
- **Graphics Libraries**:
  - Implements display and rendering capabilities
  - Supports SFML, SDL2, and NCurses
  - Uses common IGraphics interface

- **Game Libraries**:
  - Individual game implementations
  - Common IGame interface
  - State management per game

### 🔄 Data Flow

1. Core loads required libraries
2. Input events flow through graphics lib to core
3. Core processes events and updates game state
4. Game state changes are rendered via graphics lib

## 💻 Implementation Details

### Interface Specifications

```cpp
// Key interfaces overview
IGraphics {
    // Display methods
    // Input handling
    // Window management
}

IGame {
    // Game logic
    // State management
    // Event processing
}
```

### Loading Mechanism

The dynamic library system uses:
- Runtime library loading
- Symbol resolution
- Interface verification
- Error handling

## 🛠️ Build Requirements

- Modern C++ compiler (C++17)
- Dynamic library support
- Graphics dependencies:
  - SFML 2.5+
  - SDL2
  - NCurses

---
For implementation code, refer to the project source files.

```plantuml
@startuml

' Class colors and styling
skinparam classAttributeIconSize 0
skinparam classFontStyle bold
skinparam classFontSize 14
skinparam classBackgroundColor LightBlue
skinparam interfaceBackgroundColor LightGreen
skinparam abstractClassBackgroundColor #F5DEB3
skinparam packageBackgroundColor #FEFEFE

' Exception hierarchy
package "Exceptions" {
    class ArcadeException {
        -_message: string
        +what(): const char*
    }
    
    class LibraryError {
        +LibraryError(path: string)
    }
    
    class InvalidLibraryError {
        +InvalidLibraryError(path: string)
    }
    
    class GameError {
        +GameError(message: string)
    }
    
    class GraphicalError {
        +GraphicalError(message: string)
    }
}

' Core components
package "Core Components" {
    class Core {
        -_graphical: IGraphical*
        -_game: IGame*
        -_menu: Menu
        -_currentLibIndex: size_t
        -_dlLoader: DlLoader
        -_inGame: bool
        -_isPaused: bool
        +Core(path: string)
        +~Core()
        +setGraphical(Graphical: IGraphical*)
        +setGame(Game: IGame*)
        +display(elements: vector<element_t>)
        +update(): string
        +loadGame(name: string)
        +switchGraphicalLibrary(name: string)
        +getAvailableGames(): vector<string>
        +getAvailableGraphicalLibs(): vector<string>
    }
    
    class DlLoader {
        -_graphicalHandle: void*
        -_gameHandle: void*
        +DlLoader()
        +~DlLoader()
        +loadGraphicalLibrary(path: string): IGraphical*
        +unloadGraphicalLibrary(graphical: IGraphical*)
        +loadGame(name: string, username: string, highScore: int): IGame*
        +unloadGame(game: IGame*)
        +getAvailableGraphicalLibs(): vector<string>
        +getAvailableGames(): vector<string>
    }
    
    class Menu {
        -_elements: vector<element_t>
        -_availableGames: vector<string>
        -_username: string
        -_password: string
        -_selectedGame: string
        -_state: MenuState
        -_selectedButton: size_t
        -_authenticated: bool
        -_resume: bool
        -_returnToMenu: bool
        -_quit: bool
        +Menu()
        +~Menu()
        +getElements(): vector<element_t>
        +getSelectedGame(): string
        +getUsername(): string
        +isAuthenticated(): bool
        +shouldResume(): bool
        +shouldReturnToMenu(): bool
        +shouldQuit(): bool
        +setAvailableGames(games: vector<string>)
        +handleInput(input: string)
    }
    
    enum "Menu::MenuState" as MenuState {
        LOGIN_SIGNUP
        LOGIN
        SIGN_UP
        GAME_SELECT
        PAUSE
    }
}

' Graphical interfaces and classes
package "Graphical Libraries" {
    interface IGraphical {
        +~IGraphical()
        +getEvents(): string
        +draw()
        +clearElements()
        +setElements(elements: vector<element_t>)
    }
    
    abstract class AGraphical {
        #_elements: vector<element_t>
        #_width: int
        #_height: int
        +AGraphical(path: string)
        +~AGraphical()
        +clearElements()
        +setElements(elements: vector<element_t>)
    }
    
    class SFML {
        -_window: sf::RenderWindow*
        -_font: sf::Font
        -_texture: sf::Texture
        -_width: int
        -_height: int
        +SFML()
        +~SFML()
        +getEvents(): string
        +draw()
        -draw_text(element: element_t)
        -draw_image(element: element_t)
        -draw_circle(element: element_t)
        -draw_rectangle(element: element_t)
        -convertPositionToPixels(percentX: int, percentY: int): tuple<int, int>
    }
    
    class SDL {
        -_window: SDL_Window*
        -_renderer: SDL_Renderer*
        -_font: TTF_Font*
        -_width: int
        -_height: int
        +SDL()
        +~SDL()
        +getEvents(): string
        +draw()
        -draw_text(element: element_t)
        -draw_image(element: element_t)
        -draw_circle(element: element_t)
        -draw_rectangle(element: element_t)
        -convertPositionToPixels(percentX: int, percentY: int): tuple<int, int>
    }
    
    class Ncurses {
        -_window: WINDOW*
        -_maxY: int
        -_maxX: int
        -_screen: SCREEN*
        -_isInitialized: bool
        +Ncurses()
        +~Ncurses()
        +getEvents(): string
        +draw()
        -draw_text(element: element_t)
        -draw_image(element: element_t)
        -draw_circle(element: element_t)
        -draw_rectangle(element: element_t)
        -draw_border(element: element_t)
        -convertPositionToChar(percentX: int, percentY: int): tuple<int, int>
    }
}

' Game interfaces and classes
package "Games" {
    interface IGame {
        +~IGame()
        +handleEvents(command: string): vector<element_t>
    }
    
    abstract class AGame {
        #_elements: vector<element_t>
        #_username: string
        #_highScore: int
        +AGame(username: string, highScore: int)
        +~AGame()
    }
    
    class Snake {
        -_username: string
        -_score: int
        -_highScore: int
        -_updateInterval: int
        -_gameOver: bool
        -_direction: Direction
        -_lastDirection: Direction
        -_snake: list<tuple<int, int>>
        -_food: tuple<int, int>
        -_lastUpdateTime: chrono::time_point
        -_rng: mt19937
        +Snake(username: string, highScore: int)
        +~Snake()
        +handleEvents(command: string): vector<element_t>
        -restartGame()
        -moveSnake()
        -handleCollisions()
        -spawnFood()
        -updateHighScore()
        -isPositionInSnake(y: int, x: int): bool
        -createElements(): vector<element_t>
    }
    
    enum "Snake::Direction" as SnakeDirection {
        UP
        RIGHT
        DOWN
        LEFT
    }
    
    class Nibbler {
        -_gameState: GameState
        -_score: int
        -_width: size_t
        -_height: size_t
        -_cellSize: int
        -_direction: Direction
        -_lastDirection: Direction
        -_directionQueue: queue<Direction>
        -_snake: vector<pair<int, int>>
        -_walls: vector<pair<int, int>>
        -_turnWalls: map<pair<int, int>, Direction>
        -_tSections: vector<pair<int, int>>
        -_food: vector<pair<int, int>>
        -_map: vector<string>
        -_initialFoodCount: int
        -_stopped: bool
        -_lastUpdateTime: chrono::time_point
        -_updateInterval: chrono::milliseconds
        -_clockValue: int
        -_maxClockValue: int
        -_turnsWithoutFood: int
        -_lastClockUpdateTime: chrono::time_point
        -_clockUpdateInterval: chrono::milliseconds
        +Nibbler(username: string, highScore: int)
        +~Nibbler()
        +handleEvents(command: string): vector<element_t>
        -loadMap(mapPath: string): bool
        -handleWallCollision(newHead: pair<int, int>&)
        -checkCollision(): bool
        -checkWinCondition(): bool
        -updateClock()
        -wouldHitWall(newDirection: Direction): bool
        -wouldHitWall(newDirection: Direction, position: pair<int, int>): bool
        -isOppositeDirection(dir1: Direction, dir2: Direction): bool
        -isTSection(position: pair<int, int>): bool
        -processDirectionQueue()
        -updateGame()
        -processInput(command: string)
        -createGameElements(): vector<element_t>
    }
    
    enum "Nibbler::Direction" as NibblerDirection {
        UP
        DOWN
        LEFT
        RIGHT
    }
    
    enum "Nibbler::GameState" as NibblerGameState {
        RUNNING
        GAME_OVER
        WIN
    }
}

' Element structure
class "element_t" as ElementType {
    +_type: ELEMENT_TYPE
    +_text: string
    +_position: tuple<int, int>
    +_color: string
    +_size: tuple<int, int>
    +_image_path: string
    +_font_size: int
}

enum ELEMENT_TYPE {
    TEXT
    IMAGE
    CIRCLE
    RECTANGLE
    BUTTON
    BORDER
}

' Inheritance relationships
ArcadeException <|-- LibraryError
ArcadeException <|-- InvalidLibraryError
ArcadeException <|-- GameError
ArcadeException <|-- GraphicalError

IGraphical <|.. AGraphical
AGraphical <|-- SFML
AGraphical <|-- SDL
AGraphical <|-- Ncurses

IGame <|.. AGame
AGame <|-- Snake
AGame <|-- Nibbler

' Composition/Association relationships
Menu "1" -- "1" MenuState

Core "1" o-- "1" Menu
Core "1" o-- "1" DlLoader
Core "1" o-- "0..1" IGame
Core "1" o-- "0..1" IGraphical

AGraphical o-- "many" ElementType
IGame ..> "return" ElementType

Snake -- SnakeDirection
Nibbler -- NibblerDirection
Nibbler -- NibblerGameState

ElementType -- ELEMENT_TYPE

@enduml
```

