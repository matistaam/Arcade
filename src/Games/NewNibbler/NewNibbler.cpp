/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** NewNibbler
*/

#include "Includes.hpp"

namespace arc{

    NewNibbler::NewNibbler(std::string username, int highScore) : AGame(username, highScore)
    {
        // Initialize the member variable _cell_size
        this->initialize();
    }

    NewNibbler::~NewNibbler()
    {
        // Nothing to clean up
    }

    void NewNibbler::initialize()
    {
        this->_gameState = GameState::RUNNING;
        this->_inputQueue = std::nullopt;
        this->_snakeStopped = false;
        this->_cell_size = 20;

        // Load the map
        // TODO: implement a map iteration system
        if (!this->loadMap("assets/NibblerMaps/nibbler_map_01.txt")){
            // TODO: implement map loading error screen
        }

        // Initialize game timer
        this->_lastUpdateTime = std::chrono::steady_clock::now();
        this->_updateInterval = std::chrono::milliseconds(150); // Snake speed

    }

    bool NewNibbler::loadMap(const std::string& mapPath)
    {
        std::ifstream mapFile(mapPath);
        if (!mapFile.is_open()) {
            return false;
        }

        this->_map.clear();
        this->_snake.clear();
        this->_walls.clear();
        this->_turnWalls.clear();
        this->_tSections.clear();
        this->_food.clear();
        this->_foodCount = 0;
        this->_map_width = 0;
        this->_map_height = 0;

        std::string line;
        while (std::getline(mapFile, line)) {
            if (!line.empty()) {
                this->_map.push_back(line);
                this->_map_height++;
                if (line.size() > this->_map_width) {
                    this->_map_width = line.size();
                }
            }
        }

        if (this->_map.empty()) {
            return false;
        }

        // Process the map characters to create walls and other elements
        for (size_t y = 0; y < _map.size(); y++) {
            for (size_t x = 0; x < _map[y].size(); x++) {
                char tile = this->_map[y][x];
                std::pair<int, int> pos = std::make_pair(y, x);

                switch (tile) {
                    case '#':
                        this->_walls.push_back(pos);
                        break;
                    case 'T':
                        this->_walls.push_back(pos);
                        this->_tSections.push_back(pos);
                        break;
                    case 'U':
                        this->_walls.push_back(pos);
                        this->_turnWalls[pos] = Direction::UP;
                        break;
                    case 'D':
                        this->_walls.push_back(pos);
                        this->_turnWalls[pos] = Direction::DOWN;
                        break;
                    case 'L':
                        this->_walls.push_back(pos);
                        this->_turnWalls[pos] = Direction::LEFT;
                        break;
                    case 'R':
                        this->_walls.push_back(pos);
                        this->_turnWalls[pos] = Direction::RIGHT;
                        break;
                    case '*':
                        this->_food.push_back(pos);
                        this->_foodCount++;
                        break;
                    case '@':
                        this->_snake.push_back(pos);
                        break;
                    case '<':
                        this->_snake.insert(this->_snake.begin(), pos);
                        this->_direction = Direction::LEFT;
                        this->_lastDirection = Direction::LEFT;
                        break;
                    case '>':
                        this->_snake.insert(this->_snake.begin(), pos);
                        this->_direction = Direction::RIGHT;
                        this->_lastDirection = Direction::RIGHT;
                        break;
                    default:
                        break;
                }
            }
        }

        return true;
    }

    void NewNibbler::processInput(const std::string& command)
    {
        if (this->_gameState != GameState::RUNNING) {
            return;
        }

        // Convert command string to direction
        Direction newInput = this->_direction; // Default to current direction
        bool validInput = false;

        // Convert command string to direction
        if (command == "UP" && this->_lastDirection != Direction::DOWN) {
            newInput = Direction::UP;
            validInput = true;
        } else if (command == "DOWN" && this->_lastDirection != Direction::UP) {
            newInput = Direction::DOWN;
            validInput = true;
        } else if (command == "LEFT" && this->_lastDirection != Direction::RIGHT) {
            newInput = Direction::LEFT;
            validInput = true;
        } else if (command == "RIGHT" && this->_lastDirection != Direction::LEFT) {
            newInput = Direction::RIGHT;
            validInput = true;
        }

        // If no valid input, early return
        if (!validInput) {
            return;
        }

        // First try to apply the input immediately if possible
        if (!this->wouldHitWall(newInput, this->_snake.front())) {
            // Can apply immediately
            this->_direction = newInput;
            // Clear any queued input since we've made an immediate turn
            this->_inputQueue = std::nullopt;
        } else {
            // Can't apply immediately, so queue it (overwriting any existing queued input)
            this->_inputQueue = newInput;
        }
    }

    void NewNibbler::updateGame()
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->_lastUpdateTime);

        // TODO: maybe implement the skip in the processInput method
        if (elapsedTime < this->_updateInterval) {
            std::cout << "Exit 1" << std::endl;
            return;
        }

        this->_lastUpdateTime = currentTime;

        if (this->_gameState != GameState::RUNNING) {
            std::cout << "Exit 2" << std::endl;
            return;
        }

        // Get current head position
        std::pair<int, int> head = this->_snake.front();
        std::pair<int, int> newHead = head;

        // Check if we have a queued input and if it can be applied now
        if (this->_inputQueue) {
            Direction queuedDirection = *this->_inputQueue;
            
            // Check if the queued direction is valid at this position
            if (!this->wouldHitWall(queuedDirection, head)) {
                // Apply the queued input
                this->_direction = queuedDirection;
                this->_inputQueue = std::nullopt; // Clear the queue
            }
        }

        // Move head based on direction
        switch (this->_direction) {
            case Direction::UP:
                newHead.first--;
                break;
            case Direction::DOWN:
                newHead.first++;
                break;
            case Direction::LEFT:
                newHead.second--;
                break;
            case Direction::RIGHT:
                newHead.second++;
                break;
        }
        this->_lastDirection = this->_direction;

        // change the value of newHead based on the potential wall it hit
        this->handleWallCollision(newHead);

        // Check if newHead position is the same as current head position
        if (newHead.first == head.first && newHead.second == head.second) {
            // Snake has hit a wall and couldn't move
            std::cout << "Exit 3" << std::endl;
            return;
        }

        // Add newHead to the front
        this->_snake.insert(this->_snake.begin(), newHead);

        // Check if food was eaten
        bool foodEaten = false;

        for (auto it = this->_food.begin(); it != this->_food.end(); ) {
            if (newHead.first == it->first && newHead.second == it->second) {
                // TODO: handle score
                it = this->_food.erase(it);
                foodEaten = true;

                // TODO: handle clock
                break;
            } else {
                ++it;
            }
        }

        if (!foodEaten) {
            this->_snake.pop_back();

            // TODO: Handle clock speed
        }

        if (this->checkWinCondition()) {
            this->_gameState = GameState::WIN;
        }

        if (this->checkSelfCollision()) {
            this->_gameState = GameState::GAME_OVER;
        }
        std::cout << "Exit 4" << std::endl;
    }

    std::vector<element_t> NewNibbler::createGameElements()
    {
        std::vector<element_t> elements;
        element_t border = {};
        element_t wallElement = {};
        element_t snakeElement = {};
        element_t foodElement = {};

        int elementY = 0;
        int elementX = 0;

        border._type = BORDER;
        border._position = std::make_tuple(0, 0);
        border._size = std::make_tuple(this->_map_height + 2, this->_map_width +2);
        border._color = "6";
        border._display = false;
        elements.push_back(border);

        // Add walls
        for (const auto &wall : this->_walls) {
            wallElement._type = RECTANGLE;
            elementY = std::get<0>(wall);
            elementX = std::get<1>(wall);
            wallElement._position = std::make_tuple(elementY, elementX);
            wallElement._size = std::make_tuple(this->_cell_size, this->_cell_size);
            wallElement._color = "3"; // Yellow
            elements.push_back(wallElement);
        }

        // Add snake
        for (const auto &segment : this->_snake) {
            snakeElement._type = RECTANGLE;
            elementY = std::get<0>(segment);
            elementX = std::get<1>(segment);
            snakeElement._position = std::make_tuple(elementY, elementX);
            snakeElement._size = std::make_tuple(this->_cell_size, this->_cell_size);
            if (segment == this->_snake.front()) {
                snakeElement._color = "2"; // Green for head
            } else {
                snakeElement._color = "4"; // Blue for body
            }
            elements.push_back(snakeElement); // Add snake element to the elements vector
        }

        // Add all food items
        for (const auto &food : this->_food) {
            foodElement._type = CIRCLE;
            elementY = std::get<0>(food);
            elementX = std::get<1>(food);
            foodElement._position = std::make_tuple(elementY, elementX);
            foodElement._size = std::make_tuple(18, 18);
            foodElement._color = "1"; // Red
            elements.push_back(foodElement);
        }

        return elements;
    }

    void NewNibbler::handleWallCollision(std::pair<int, int>& newHead)
    {
        for (auto& wall : this->_walls) {
            if (newHead.first == wall.first && newHead.second == wall.second) {
                std::cout << "Collision detected at: (" << newHead.first << ", " << newHead.second << ")" << std::endl;

                auto it = this->_turnWalls.find(wall);
                if (it != this->_turnWalls.end()) {
                    std::cout << "Turn wall detected. Changing direction." << std::endl;
                    Direction newDirection = it->second;

                    newHead = this->_snake.front();
                    this->_direction = newDirection;

                    switch (this->_direction) {
                        case Direction::UP:
                            newHead.first--;
                            break;
                        case Direction::DOWN:
                            newHead.first++;
                            break;
                        case Direction::LEFT:
                            newHead.second--;
                            break;
                        case Direction::RIGHT:
                            newHead.second++;
                            break;
                    }
                } else {
                    std::cout << "Regular wall detected. Stopping snake." << std::endl;
                    newHead = this->_snake.front();
                    this->_snakeStopped = true;
                }
                return;
            }
        }
    }

    bool NewNibbler::isTSection(const std::pair<int, int>& position)
    {
        for (auto& tSection : this->_tSections) {
            if (tSection.first == position.first && tSection.second == position.second) {
                return true;
            }
        }
        return false;
    }

    bool NewNibbler::checkWinCondition()
    {
        return this->_food.empty();
    }

    bool NewNibbler::checkSelfCollision()
    {
        // Get the head of the snake
        std::pair<int, int> head = this->_snake.front();

        // Check self collision starting from the 4th body segment
        for (std::size_t i = 3; i < this->_snake.size(); i++) {
            if (head.first == this->_snake[i].first && head.second == this->_snake[i].second) {
                return true;
            }
        }
        return false;
    }

    bool NewNibbler::isOppositeDirection(Direction dir1, Direction dir2)
    {
        return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
               (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
               (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
               (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
    }

    bool NewNibbler::wouldHitWall(Direction testDirection, std::pair<int, int> position)
    {
        // Calculate the position after moving in the test direction
        std::pair<int, int> potentialPos = position;

        switch (testDirection) {
            case Direction::UP:
                potentialPos.first--;
                break;
            case Direction::DOWN:
                potentialPos.first++;
                break;
            case Direction::LEFT:
                potentialPos.second--;
                break;
            case Direction::RIGHT:
                potentialPos.second++;
                break;
        }

        // Check if this position is a wall
        for (auto& wall : this->_walls) {
            if (potentialPos.first == wall.first && potentialPos.second == wall.second) {
                // NOT SURE ABOUT THIS
                // Exception for turn walls which are allowed
                auto it = this->_turnWalls.find(wall);
                if (it != this->_turnWalls.end()) {
                    return false;
                }

                // Regular wall or T-section would cause collision
                return true;
            }
        }

        return false; // No wall collision
    }

    std::vector<element_t> NewNibbler::handleEvents(std::string command)
    {
        // If r is pressed, re-initialize game
        if (command == "r") {
            this->initialize();
            return this->createGameElements();
        }

        // Process input
        this->processInput(command);

        // Update game state
        this->updateGame();

        // Create and return the elements to render
        return this->createGameElements();
    }
}

extern "C" {
    arc::IGame *create(const std::string &username, int highScore)
    {
        return (new arc::NewNibbler(username, highScore));
    }

    void destroy(arc::IGame *instance)
    {
        delete instance;
    }

    const char* get_type()
    {
        return ("game");
    }
}