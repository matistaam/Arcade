/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Nibbler
*/

#include "Includes.hpp"
#include <random>
#include <chrono>
#include <fstream>
#include <iostream>

namespace arc {
    Nibbler::Nibbler() : AGame()
    {
        // Initialize game state
        this->_gameState = GameState::RUNNING;
        this->_score = 0;
        this->_cellSize = 20;
        this->_initialFoodCount = 0;
        
        // Set initial direction (right)
        this->_direction = Direction::RIGHT;
        
        // Load map from file
        if (!loadMap("assets/NibblerMaps/nibbler_map_01.txt")) {
            std::cerr << "Failed to load map, using default configuration" << std::endl;
            // Set default dimensions if map loading fails
            this->_width = 40;
            this->_height = 30;
            
            // Initialize walls with default configuration
            initializeWalls();
            
            // Place default food
            placeFood(10);
            this->_initialFoodCount = 10;
            
            // Initialize snake at the center of the grid with default direction (right)
            size_t centerX = this->_width / 2;
            size_t centerY = this->_height / 2;
            this->_direction = Direction::RIGHT;
            this->_snake.push_back(std::make_pair(centerX, centerY));
            this->_snake.push_back(std::make_pair(centerX - 1, centerY));
            this->_snake.push_back(std::make_pair(centerX - 2, centerY));
        }
        
        // Initialize game timer
        this->_lastUpdateTime = std::chrono::steady_clock::now();
        this->_updateInterval = std::chrono::milliseconds(200); // Snake speed
    }

    Nibbler::~Nibbler()
    {
        // Nothing to clean up specifically
    }

    bool Nibbler::loadMap(const std::string& mapPath)
    {
        std::ifstream mapFile(mapPath);
        if (!mapFile.is_open()) {
            return false;
        }

        this->_map.clear();
        this->_walls.clear();
        this->_turnWalls.clear();
        this->_food.clear();
        this->_snake.clear();

        std::string line;
        while (std::getline(mapFile, line)) {
            if (!line.empty()) {
                this->_map.push_back(line);
            }
        }
        
        if (this->_map.empty()) {
            return false;
        }

        // Set dimensions based on the map
        this->_height = this->_map.size();
        this->_width = this->_map[0].size();
        
        // First pass: Find snake head to determine direction and starting positions
        std::pair<int, int> headPos = {-1, -1};
        bool headFound = false;
        
        for (size_t y = 0; y < this->_height; y++) {
            for (size_t x = 0; x < this->_width; x++) {
                if (x < this->_map[y].size()) {
                    if (this->_map[y][x] == '>') {
                        headPos = std::make_pair(x, y);
                        this->_direction = Direction::RIGHT;
                        headFound = true;
                        break;
                    } else if (this->_map[y][x] == '<') {
                        headPos = std::make_pair(x, y);
                        this->_direction = Direction::LEFT;
                        headFound = true;
                        break;
                    }
                }
            }
            if (headFound) break;
        }
        
        // Second pass: Process all map elements
        for (size_t y = 0; y < this->_height; y++) {
            for (size_t x = 0; x < this->_width; x++) {
                if (x < this->_map[y].size()) {
                    char tile = this->_map[y][x];
                    std::pair<int, int> pos = std::make_pair(x, y);
                    
                    switch (tile) {
                        case '#': // Normal Wall
                            this->_walls.push_back(pos);
                            break;
                        case 'U': // Up direction wall
                            this->_walls.push_back(pos);
                            this->_turnWalls[pos] = Direction::UP;
                            break;
                        case 'D': // Down direction wall
                            this->_walls.push_back(pos);
                            this->_turnWalls[pos] = Direction::DOWN;
                            break;
                        case 'L': // Left direction wall
                            this->_walls.push_back(pos);
                            this->_turnWalls[pos] = Direction::LEFT;
                            break;
                        case 'R': // Right direction wall
                            this->_walls.push_back(pos);
                            this->_turnWalls[pos] = Direction::RIGHT;
                            break;
                        case '*': // Food
                            this->_food.push_back(pos);
                            break;
                        case '@': // Snake body
                            // Store snake body segment positions for later ordering
                            this->_snake.push_back(pos);
                            break;
                        case '>': // Snake head (right)
                        case '<': // Snake head (left)
                            // Add the head as the first element of the snake
                            this->_snake.insert(this->_snake.begin(), pos);
                            break;
                        default:
                            // Empty spaces or unrecognized characters
                            break;
                    }
                }
            }
        }
        
        // Store initial food count for win condition
        this->_initialFoodCount = this->_food.size();
        
        // Check if we found a valid snake
        if (this->_snake.empty()) {
            std::cerr << "No snake found in map file" << std::endl;
            return false;
        }
        
        return true;
    }

    void Nibbler::initializeWalls()
    {
        // This is only used if map loading fails
        this->_walls.clear();
        this->_turnWalls.clear();
        
        // Create walls around the perimeter
        for (size_t x = 0; x < this->_width; x++) {
            this->_walls.push_back(std::make_pair(x, 0));
            this->_walls.push_back(std::make_pair(x, this->_height - 1));
        }
        
        for (size_t y = 0; y < this->_height; y++) {
            this->_walls.push_back(std::make_pair(0, y));
            this->_walls.push_back(std::make_pair(this->_width - 1, y));
        }
        
        // Add some inner walls (specific to Nibbler level design)
        this->_walls.push_back(std::make_pair(this->_width / 3, this->_height / 3));
        this->_walls.push_back(std::make_pair(this->_width / 3 + 1, this->_height / 3));
        this->_walls.push_back(std::make_pair(this->_width * 2 / 3, this->_height * 2 / 3));
        this->_walls.push_back(std::make_pair(this->_width * 2 / 3 + 1, this->_height * 2 / 3));
    }

    void Nibbler::placeFood(int count)
    {
        // Only add random food if needed
        if (this->_food.size() >= static_cast<size_t>(count)) {
            return;
        }
        
        int foodToAdd = count - this->_food.size();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> xDist(1, this->_width - 2);
        std::uniform_int_distribution<size_t> yDist(1, this->_height - 2);
        
        for (int i = 0; i < foodToAdd; i++) {
            bool validPosition = false;
            std::pair<int, int> newFood;
            
            while (!validPosition) {
                newFood = std::make_pair(xDist(gen), yDist(gen));
                validPosition = true;
                
                // Check if food is not on a wall
                for (auto& wall : this->_walls) {
                    if (wall.first == newFood.first && wall.second == newFood.second) {
                        validPosition = false;
                        break;
                    }
                }
                
                // Check if food is not on the snake
                if (validPosition) {
                    for (auto& segment : this->_snake) {
                        if (segment.first == newFood.first && segment.second == newFood.second) {
                            validPosition = false;
                            break;
                        }
                    }
                }
                
                // Check if food is not on another food
                if (validPosition) {
                    for (auto& existingFood : this->_food) {
                        if (existingFood.first == newFood.first && existingFood.second == newFood.second) {
                            validPosition = false;
                            break;
                        }
                    }
                }
            }
            
            this->_food.push_back(newFood);
        }
    }

    void Nibbler::handleWallCollision(std::pair<int, int>& newHead)
    {
        // Check if the new head position is a wall
        for (auto& wall : this->_walls) {
            if (newHead.first == wall.first && newHead.second == wall.second) {
                // Check if it's a turn wall
                auto it = this->_turnWalls.find(wall);
                if (it != this->_turnWalls.end()) {
                    // Get the new direction from the wall type
                    Direction newDirection = it->second;
                    
                    // Restore the head position (undo the move)
                    newHead = this->_snake.front();
                    
                    // Change direction
                    this->_direction = newDirection;
                    
                    // Move the head based on the new direction
                    switch (this->_direction) {
                        case Direction::UP:
                            newHead.second--;
                            break;
                        case Direction::DOWN:
                            newHead.second++;
                            break;
                        case Direction::LEFT:
                            newHead.first--;
                            break;
                        case Direction::RIGHT:
                            newHead.first++;
                            break;
                    }
                    
                    // Check if the new position is also a wall (T-junction)
                    for (auto& checkWall : this->_walls) {
                        if (newHead.first == checkWall.first && newHead.second == checkWall.second) {
                            // Hit a T-junction, restore head to original position
                            newHead = this->_snake.front();
                            break;
                        }
                    }
                } else {
                    // Regular wall (T-junction) - snake stops moving until new direction input
                    newHead = this->_snake.front();
                }
                
                // We found our wall collision, no need to check others
                break;
            }
        }
    }

    bool Nibbler::checkCollision()
    {
        // Get the head of the snake
        std::pair<int, int> head = this->_snake.front();
        
        // Check self-collision (starting from the 3rd segment)
        for (size_t i = 3; i < this->_snake.size(); i++) {
            if (head.first == this->_snake[i].first && head.second == this->_snake[i].second) {
                return true;
            }
        }
        
        return false;
    }

    bool Nibbler::checkWinCondition() 
    {
        return this->_food.empty();
    }

    void Nibbler::updateGame()
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - this->_lastUpdateTime);
        
        if (elapsedTime < this->_updateInterval) {
            return; // Not time to update yet
        }
        
        this->_lastUpdateTime = currentTime;
        
        // Don't update if game is not running
        if (this->_gameState != GameState::RUNNING) {
            return;
        }
        
        // Get current head position
        std::pair<int, int> head = this->_snake.front();
        std::pair<int, int> newHead = head;
        
        // Move the head based on direction
        switch (this->_direction) {
            case Direction::UP:
                newHead.second--;
                break;
            case Direction::DOWN:
                newHead.second++;
                break;
            case Direction::LEFT:
                newHead.first--;
                break;
            case Direction::RIGHT:
                newHead.first++;
                break;
        }
        
        // Handle wall collisions and direction changes
        handleWallCollision(newHead);
        
        // If game over after collision, return early
        if (this->_gameState == GameState::GAME_OVER) {
            return;
        }
        
        // Check if the new head position is the same as the current head position
        // This happens at T-junctions where the snake can't turn
        if (newHead.first == head.first && newHead.second == head.second) {
            // Snake is stopped at a T-junction - waiting for player input
            return;
        }
        
        // Add new head to the front
        this->_snake.insert(this->_snake.begin(), newHead);
        
        // Check if the snake eats food
        bool foodEaten = false;
        
        for (auto it = this->_food.begin(); it != this->_food.end(); ) {
            if (newHead.first == it->first && newHead.second == it->second) {
                this->_score += 10;
                it = this->_food.erase(it);
                foodEaten = true;
                // Snake grows, so we don't remove the tail
                break;
            } else {
                ++it;
            }
        }
        
        // Remove the tail if no food was eaten
        if (!foodEaten) {
            this->_snake.pop_back();
        }
        
        // Check for win condition
        if (checkWinCondition()) {
            this->_gameState = GameState::WIN;
            return;
        }
        
        // Check for self-collisions
        if (checkCollision()) {
            this->_gameState = GameState::GAME_OVER;
        }
    }

    void Nibbler::processInput(const std::string& command)
    {
        if (this->_gameState == GameState::RUNNING) {
            if (command == "UP" && this->_direction != Direction::DOWN) {
                this->_direction = Direction::UP;
            } else if (command == "DOWN" && this->_direction != Direction::UP) {
                this->_direction = Direction::DOWN;
            } else if (command == "LEFT" && this->_direction != Direction::RIGHT) {
                this->_direction = Direction::LEFT;
            } else if (command == "RIGHT" && this->_direction != Direction::LEFT) {
                this->_direction = Direction::RIGHT;
            }
        } else if (this->_gameState == GameState::GAME_OVER || this->_gameState == GameState::WIN) {
            if (command == "RETURN") {
                // Restart game
                *this = Nibbler();
            }
        }
    }

    std::vector<element_t> Nibbler::createGameElements()
    {
        std::vector<element_t> elements;
        
        // Add walls
        for (auto& wall : this->_walls) {
            element_t wallElement;
            wallElement._type = RECTANGLE;
            wallElement._position = std::make_tuple(wall.second * this->_cellSize + this->_cellSize/2, 
                                                   wall.first * this->_cellSize + this->_cellSize/2);
            wallElement._size = std::make_tuple(this->_cellSize, this->_cellSize);
            
            // All walls should look the same - use a consistent color
            wallElement._color = "3"; // Yellow for all walls
            
            elements.push_back(wallElement);
        }
        
        // Add snake
        for (size_t i = 0; i < this->_snake.size(); i++) {
            element_t snakeElement;
            snakeElement._type = RECTANGLE;
            snakeElement._position = std::make_tuple(this->_snake[i].second * this->_cellSize + this->_cellSize/2, 
                                                    this->_snake[i].first * this->_cellSize + this->_cellSize/2);
            snakeElement._size = std::make_tuple(this->_cellSize, this->_cellSize);
            
            if (i == 0) {
                snakeElement._color = "2"; // Green for the head
            } else {
                snakeElement._color = "4"; // Blue for the body
            }
            
            elements.push_back(snakeElement);
        }
        
        // Add all food items
        for (auto& foodPos : this->_food) {
            element_t foodElement;
            foodElement._type = CIRCLE;
            foodElement._position = std::make_tuple(foodPos.second * this->_cellSize + this->_cellSize/2, 
                                                   foodPos.first * this->_cellSize + this->_cellSize/2);
            foodElement._size = std::make_tuple(this->_cellSize, this->_cellSize);
            foodElement._color = "1"; // Red
            elements.push_back(foodElement);
        }
        
        // Score display
        element_t scoreElement;
        scoreElement._type = TEXT;
        scoreElement._text = "Score: " + std::to_string(this->_score);
        scoreElement._position = std::make_tuple(10, 10);
        scoreElement._color = "6"; // Cyan
        elements.push_back(scoreElement);
        
        // Food count display
        element_t foodCountElement;
        foodCountElement._type = TEXT;
        foodCountElement._text = "Food: " + std::to_string(this->_food.size()) + "/" + std::to_string(this->_initialFoodCount);
        foodCountElement._position = std::make_tuple(10, 40);
        foodCountElement._color = "6"; // Cyan
        elements.push_back(foodCountElement);
        
        // Direction indicator
        element_t directionElement;
        directionElement._type = TEXT;
        directionElement._text = "Direction: ";
        switch (this->_direction) {
            case Direction::UP:
                directionElement._text += "UP";
                break;
            case Direction::DOWN:
                directionElement._text += "DOWN";
                break;
            case Direction::LEFT:
                directionElement._text += "LEFT";
                break;
            case Direction::RIGHT:
                directionElement._text += "RIGHT";
                break;
        }
        directionElement._position = std::make_tuple(10, 70);
        directionElement._color = "6"; // Cyan
        elements.push_back(directionElement);
        
        // Game state messages
        if (this->_gameState == GameState::GAME_OVER) {
            element_t gameOverElement;
            gameOverElement._type = TEXT;
            gameOverElement._text = "Game Over! Press Enter to restart";
            gameOverElement._position = std::make_tuple(this->_height * this->_cellSize / 2, 
                                                       this->_width * this->_cellSize / 4);
            gameOverElement._color = "5"; // Magenta
            elements.push_back(gameOverElement);
        } else if (this->_gameState == GameState::WIN) {
            element_t winElement;
            winElement._type = TEXT;
            winElement._text = "You Win! Press Enter to restart";
            winElement._position = std::make_tuple(this->_height * this->_cellSize / 2, 
                                                  this->_width * this->_cellSize / 4);
            winElement._color = "2"; // Green
            elements.push_back(winElement);
        }
        
        return elements;
    }

    std::vector<element_t> Nibbler::handleEvents(std::string command)
    {
        // Process input
        this->processInput(command);
        
        // Update game state
        this->updateGame();
        
        // Create and return render elements
        return this->createGameElements();
    }
}

extern "C" {
    arc::IGame *create()
    {
        return (new arc::Nibbler());
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
