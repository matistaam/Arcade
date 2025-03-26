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
#include <queue>

namespace arc {
    Nibbler::Nibbler() : AGame()
    {
        // Initialize game state
        this->_gameState = GameState::RUNNING;
        this->_score = 0;
        this->_cellSize = 20;
        this->_initialFoodCount = 0;
        this->_stopped = false;
        
        // Initialize clock
        this->_maxClockValue = 100;
        this->_clockValue = this->_maxClockValue;
        this->_turnsWithoutFood = 0;
        this->_clockUpdateInterval = std::chrono::milliseconds(1000); // 1 second initially
        this->_lastClockUpdateTime = std::chrono::steady_clock::now();
        
        // Load map from file
        if (!this->loadMap("assets/NibblerMaps/nibbler_map_01.txt")) {
            std::cerr << "Failed to load map, using default configuration" << std::endl;
            // TODO: Implement a map loading error screen
        }
        
        // Initialize game timer
        this->_lastUpdateTime = std::chrono::steady_clock::now();
        this->_updateInterval = std::chrono::milliseconds(150); // Snake speed
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
        this->_tSections.clear();
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
                        this->_lastDirection = Direction::RIGHT;
                        headFound = true;
                        break;
                    } else if (this->_map[y][x] == '<') {
                        headPos = std::make_pair(x, y);
                        this->_direction = Direction::LEFT;
                        this->_lastDirection = Direction::LEFT;
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
                        case 'T': // T-section
                            this->_walls.push_back(pos);
                            this->_tSections.push_back(pos);
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
        
        // Clear the direction queue when loading a new map
        while (!this->_directionQueue.empty()) {
            this->_directionQueue.pop();
        }
        
        return true;
    }

    bool Nibbler::isTSection(const std::pair<int, int>& position)
    {
        // Check if this position is in _tSections vector
        for (auto& tSection : this->_tSections) {
            if (tSection.first == position.first && tSection.second == position.second) {
                return true;
            }
        }
        return false;
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
                    
                    // Check if the new position is also a wall
                    for (auto& checkWall : this->_walls) {
                        if (newHead.first == checkWall.first && newHead.second == checkWall.second) {
                            // Hit another wall when trying to turn, check if it's a T-section
                            if (this->isTSection(checkWall)) {
                                // If directly hitting a T-section, stop the snake
                                newHead = this->_snake.front();
                                this->_stopped = true;
                            } else {
                                // For regular walls, prevent stopping (undo the move but don't set stopped flag)
                                newHead = this->_snake.front();
                                // The snake will continue in its original direction in the next update
                            }
                            break;
                        }
                    }
                } else if (this->isTSection(wall)) {
                    // Direct hit on a T-section - snake stops moving
                    newHead = this->_snake.front();
                    this->_stopped = true;
                } else {
                    // Regular wall - don't stop, just prevent movement in this direction
                    newHead = this->_snake.front();
                    // The snake will continue in its original direction in the next update
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
                std::cout << "Collision between " << this->_snake[i].first << " and " << this->_snake[i].second << "with i = " << i << std::endl;
                return true;
            }
        }
        
        return false;
    }

    bool Nibbler::checkWinCondition()
    {
        return this->_food.empty();
    }

    void Nibbler::updateClock()
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - this->_lastClockUpdateTime);
        
        // Update clock based on current interval
        if (elapsedTime >= this->_clockUpdateInterval) {
            this->_lastClockUpdateTime = currentTime;
            this->_clockValue--;
            
            // Check if clock is empty
            if (this->_clockValue <= 0) {
                this->_clockValue = 0;
                this->_gameState = GameState::GAME_OVER;
            }
        }
    }

    bool Nibbler::isOppositeDirection(Direction dir1, Direction dir2)
    {
        return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
               (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
               (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
               (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
    }

    bool Nibbler::wouldHitWall(Direction newDirection, std::pair<int, int> position)
    {
        // Calculate the position after moving in the new direction
        std::pair<int, int> potentialHead = position;
        
        switch (newDirection) {
            case Direction::UP:
                potentialHead.second--;
                break;
            case Direction::DOWN:
                potentialHead.second++;
                break;
            case Direction::LEFT:
                potentialHead.first--;
                break;
            case Direction::RIGHT:
                potentialHead.first++;
                break;
        }
        
        // Check if the potential head position would be a wall
        for (auto& wall : this->_walls) {
            if (potentialHead.first == wall.first && potentialHead.second == wall.second) {
                // We found a wall at the potential position
                
                // Check if it's a turn wall (these are allowed)
                auto it = this->_turnWalls.find(wall);
                if (it != this->_turnWalls.end()) {
                    return false; // Turn walls are allowed
                }
                
                // It's a regular wall or T-section, direction change would cause a collision
                return true;
            }
        }
        
        // No wall collision
        return false;
    }

    bool Nibbler::wouldHitWall(Direction newDirection)
    {
        return wouldHitWall(newDirection, this->_snake.front());
    }

    void Nibbler::processDirectionQueue()
    {
        // Nothing to process if queue is empty
        if (this->_directionQueue.empty()) {
            return;
        }
        
        // Get the next direction from queue
        Direction nextDirection = this->_directionQueue.front();
        
        // Check if we can change to this direction without hitting a wall
        if (!this->isOppositeDirection(nextDirection, this->_lastDirection) && 
            !this->wouldHitWall(nextDirection)) {
            
            // Change direction
            this->_direction = nextDirection;
            this->_directionQueue.pop();
            
            // If snake was stopped at a T-section, allow it to move again
            if (this->_stopped) {
                this->_stopped = false;
            }
        }
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
        
        // Update clock on each game update
        this->updateClock();
        
        // Process the direction queue
        this->processDirectionQueue();
        
        // If snake is stopped at a T-section, wait for player input
        if (this->_stopped) {
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
        this->_lastDirection = this->_direction;
        
        // Handle wall collisions and direction changes
        handleWallCollision(newHead);
        
        // If game over after collision, return early
        if (this->_gameState == GameState::GAME_OVER) {
            return;
        }
        
        // Check if the new head position is the same as the current head position
        // This happens when the snake has hit a wall and couldn't move
        if (newHead.first == head.first && newHead.second == head.second) {
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
                
                // Reset clock-related counters when food is eaten
                this->_turnsWithoutFood = 0;
                
                // Add time to the clock
                this->_clockValue = std::min(this->_clockValue + 20, this->_maxClockValue);
                
                // Reset clock update interval
                this->_clockUpdateInterval = std::chrono::milliseconds(1000);
                
                // Snake grows, so we don't remove the tail
                break;
            } else {
                ++it;
            }
        }
        
        // Remove the tail if no food was eaten
        if (!foodEaten) {
            this->_snake.pop_back();
            
            // Increment turns without food and adjust clock speed
            this->_turnsWithoutFood++;
            
            // Make the clock tick faster the longer we go without food
            // Every 5 turns without food, increase clock tick rate
            if (this->_turnsWithoutFood % 5 == 0 && this->_turnsWithoutFood > 0) {
                // Decrease update interval by 10%, with a minimum of 100ms
                long newInterval = static_cast<long>(this->_clockUpdateInterval.count() * 0.9);
                this->_clockUpdateInterval = std::chrono::milliseconds(std::max(100L, newInterval));
            }
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
        // Handle reset command if game over or won
        if ((this->_gameState == GameState::GAME_OVER || this->_gameState == GameState::WIN) 
            && command == "ENTER") {
            
            // Reset the game by recreating the Nibbler object
            *this = Nibbler();
            return;
        }
        
        // Only process movement inputs if the game is still running
        if (this->_gameState != GameState::RUNNING)
            return;

        Direction newDirection = this->_direction; // Default to current direction
        bool validInput = false;

        if (command == "UP" && this->_lastDirection != Direction::DOWN) {
            newDirection = Direction::UP;
            validInput = true;
        } else if (command == "DOWN" && this->_lastDirection != Direction::UP) {
            newDirection = Direction::DOWN;
            validInput = true;
        } else if (command == "LEFT" && this->_lastDirection != Direction::RIGHT) {
            newDirection = Direction::LEFT;
            validInput = true;
        } else if (command == "RIGHT" && this->_lastDirection != Direction::LEFT) {
            newDirection = Direction::RIGHT;
            validInput = true;
        }

        // If we got a valid input and the direction queue isn't too full
        if (validInput && this->_directionQueue.size() < this->_maxQueueSize) {
            // First check if we have a valid move right now
            if (!this->wouldHitWall(newDirection)) {
                // Apply it immediately if we're stopped
                if (this->_stopped) {
                    this->_direction = newDirection;
                    this->_stopped = false;
                } else {
                    // Otherwise, queue it for the next move
                    this->_directionQueue.push(newDirection);
                }
            } else {
                // Check if this direction would be valid from the next position
                std::pair<int, int> nextPos = this->_snake.front();
                
                // Calculate the next position based on current direction
                switch (this->_direction) {
                    case Direction::UP:
                        nextPos.second--;
                        break;
                    case Direction::DOWN:
                        nextPos.second++;
                        break;
                    case Direction::LEFT:
                        nextPos.first--;
                        break;
                    case Direction::RIGHT:
                        nextPos.first++;
                        break;
                }
                
                // Check if the move would be valid from the next position
                if (!this->wouldHitWall(newDirection, nextPos) && 
                    !this->isOppositeDirection(newDirection, this->_direction)) {
                    this->_directionQueue.push(newDirection);
                }
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
        
        // Clock display - add new element
        element_t clockElement;
        clockElement._type = TEXT;
        clockElement._text = "Time: " + std::to_string(this->_clockValue);
        clockElement._position = std::make_tuple(10, 70);
        
        // Change color based on clock value
        if (this->_clockValue > 70) {
            clockElement._color = "2"; // Green for plenty of time
        } else if (this->_clockValue > 30) {
            clockElement._color = "3"; // Yellow for medium time
        } else {
            clockElement._color = "1"; // Red for low time
        }
        
        elements.push_back(clockElement);
        
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
        directionElement._position = std::make_tuple(10, 100);
        directionElement._color = "6"; // Cyan
        elements.push_back(directionElement);
        
        // Turns without food indicator
        element_t turnsElement;
        turnsElement._type = TEXT;
        turnsElement._text = "Hunger: " + std::to_string(this->_turnsWithoutFood);
        turnsElement._position = std::make_tuple(10, 130);
        turnsElement._color = this->_turnsWithoutFood > 10 ? "1" : "6"; // Red if hungry
        elements.push_back(turnsElement);
        
        // Add queue indicator
        element_t queueElement;
        queueElement._type = TEXT;
        queueElement._text = "Queue: ";
        
        // Copy the queue to display it without modifying the original
        std::queue<Direction> queueCopy = this->_directionQueue;
        while (!queueCopy.empty()) {
            Direction dir = queueCopy.front();
            
            switch (dir) {
                case Direction::UP:
                    queueElement._text += "↑ ";
                    break;
                case Direction::DOWN:
                    queueElement._text += "↓ ";
                    break;
                case Direction::LEFT:
                    queueElement._text += "← ";
                    break;
                case Direction::RIGHT:
                    queueElement._text += "→ ";
                    break;
            }
            
            queueCopy.pop();
        }
        
        if (queueElement._text == "Queue: ") {
            queueElement._text += "empty";
        }
        
        queueElement._position = std::make_tuple(10, 160);
        queueElement._color = "6"; // Cyan
        elements.push_back(queueElement);
        
        // Game state messages
        if (this->_gameState == GameState::GAME_OVER) {
            element_t gameOverElement;
            gameOverElement._type = TEXT;
            gameOverElement._text = "Game Over! Press Enter to restart";
            gameOverElement._position = std::make_tuple(this->_height * this->_cellSize / 2, 
                                                       this->_width * this->_cellSize / 4);
            gameOverElement._color = "5"; // Magenta
            gameOverElement._font_size = 32;
            elements.push_back(gameOverElement);
            
            // Add reason for game over
            element_t reasonElement;
            reasonElement._type = TEXT;
            if (this->_clockValue <= 0) {
                reasonElement._text = "You ran out of time!";
            } else {
                reasonElement._text = "You crashed into yourself!";
            }
            reasonElement._position = std::make_tuple(this->_height * this->_cellSize / 2 + 40, 
                                                    this->_width * this->_cellSize / 4);
            reasonElement._color = "1"; // Red
            reasonElement._font_size = 24;
            elements.push_back(reasonElement);
        } else if (this->_gameState == GameState::WIN) {
            element_t winElement;
            winElement._type = TEXT;
            winElement._text = "You Win! Press Enter to restart";
            winElement._position = std::make_tuple(this->_height * this->_cellSize / 2, 
                                                  this->_width * this->_cellSize / 4);
            winElement._color = "2"; // Green
            winElement._font_size = 32;
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
