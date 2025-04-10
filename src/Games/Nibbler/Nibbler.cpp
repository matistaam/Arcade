/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Nibbler
*/

#include "Includes.hpp"

namespace arc{

    Nibbler::Nibbler(std::string username, int highScore) : AGame(username, highScore)
    {
        // Initialize the member variable _cell_size
        this->initialize();
    }

    Nibbler::~Nibbler()
    {
        // Nothing to clean up
    }

    void Nibbler::initialize()
    {
        this->_gameState = GameState::RUNNING;
        this->_inputQueue = std::nullopt;
        this->_snakeStopped = false;
        this->_cell_size = 20;
        this->_currentScore = 0;

        // Initialize clock timer
        this->_timeRemaining = 60.0f;
        this->_clockSpeedMultiplier = 1.0f;

        // Load the map
        // TODO: implement a map iteration system
        if (!this->loadMap("assets/NibblerMaps/nibbler_map_01.txt")){
            // TODO: implement map loading error screen
        }

        // Initialize game timer
        this->_lastUpdateTime = std::chrono::steady_clock::now();
        this->_updateInterval = std::chrono::milliseconds(150);
        this->_lastFoodEatenTime = std::chrono::steady_clock::now();
        this->_lastClockUpdateTime = std::chrono::steady_clock::now();
    }

    bool Nibbler::loadMap(const std::string &mapPath)
    {
        std::ifstream mapFile(mapPath);
        std::string line = "";

        if (!mapFile.is_open())
            return (false);
        this->_score = 0;
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

    void Nibbler::processInput(const std::string& command)
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

    void Nibbler::updateGame()
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->_lastUpdateTime);

        if (elapsedTime < this->_updateInterval) {
            return;
        }

        this->_lastUpdateTime = currentTime;

        // Stop updating the clock if the game is won or over
        if (this->_gameState == GameState::WIN || this->_gameState == GameState::GAME_OVER) {
            return;
        }

        // Update the clock timer
        auto clockElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->_lastClockUpdateTime);
        float elapsedSeconds = clockElapsed.count() / 1000.0f;
        this->_lastClockUpdateTime = currentTime;

        // Decrease time remaining based on elapsed time and current multiplier
        this->_timeRemaining -= elapsedSeconds * this->_clockSpeedMultiplier;

        // Check if time has run out
        if (this->_timeRemaining <= 0) {
            this->_timeRemaining = 0;
            this->_gameState = GameState::GAME_OVER;
            return;
        }

        // Increase clock speed if no food has been eaten for a while
        auto timeSinceLastFood = std::chrono::duration_cast<std::chrono::seconds>(
            currentTime - this->_lastFoodEatenTime).count();
        
        // Every 5 seconds without eating, increase clock speed multiplier
        this->_clockSpeedMultiplier = 1.0f + (timeSinceLastFood / 5.0f) * 1.5f; // Increases by 1.5 every 5 seconds

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
                newHead.first--;
                break;
            case Direction::DOWN:
                newHead.first++;
                newHead.first++;
                break;
            case Direction::LEFT:
                newHead.second--;
                newHead.second--;
                break;
            case Direction::RIGHT:
                newHead.second++;
                newHead.second++;
                break;
        }
        this->_lastDirection = this->_direction;

        // Change the value of newHead based on the potential wall it hit
        this->handleWallCollision(newHead);

        // Check if newHead position is the same as current head position
        if (newHead.first == head.first && newHead.second == head.second) {
            // Snake has hit a wall and couldn't move
            return;
        }

        // Add newHead to the front
        this->_snake.insert(this->_snake.begin(), newHead);

        bool foodEaten = false;

        for (auto it = this->_food.begin(); it != this->_food.end(); ) {
            if (newHead.first == it->first && newHead.second == it->second) {
                // Increment score when food is eaten
                this->_currentScore += 1;
                it = this->_food.erase(it);
                foodEaten = true;

                // Reset the food eaten timer and clock speed multiplier
                this->_lastFoodEatenTime = currentTime;
                this->_clockSpeedMultiplier = 1.0f;

                break;
            } else {
                ++it;
            }
        }

        if (!foodEaten) {
            this->_snake.pop_back();
        }

        if (this->checkWinCondition()) {
            // Add the remaining time to the score when the player wins
            this->_currentScore += static_cast<int>(this->_timeRemaining);
            this->_gameState = GameState::WIN;

            // Update high score if current score is higher
            if (this->_currentScore > this->_highScore) {
                this->_highScore = this->_currentScore;
            }
        }

        if (this->checkSelfCollision()) {
            this->_gameState = GameState::GAME_OVER;
    }

    std::vector<element_t> Nibbler::createGameElements()
    {
        std::vector<element_t> elements;
        element_t border = {};
        element_t wallElement = {};
        element_t snakeElement = {};
        element_t foodElement = {};
        element_t timerElement = {};
        element_t speedElement = {};
        element_t scoreElement = {};
        element_t gameOverElement = {};

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
            elements.push_back(snakeElement);
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

        // Check game state and display appropriate message or timer
        if (this->_gameState == GameState::GAME_OVER) {
            // Display game over message
            gameOverElement._type = TEXT;
            gameOverElement._position = std::make_tuple(75, 30);
            gameOverElement._text = "GAME OVER";
            gameOverElement._color = "1"; // Red for game over
            gameOverElement._font_size = 32;
            elements.push_back(gameOverElement);

            element_t restartElement = {};
            restartElement._type = TEXT;
            restartElement._position = std::make_tuple(75, 40);
            restartElement._text = "Press 'r' to restart";
            restartElement._color = "1"; // Red for game over
            restartElement._font_size = 24;
            elements.push_back(restartElement);
        } else if (this->_gameState == GameState::WIN) {
            // Display win message
            gameOverElement._type = TEXT;
            gameOverElement._position = std::make_tuple(75, 30); 
            gameOverElement._text = "YOU WIN!";
            gameOverElement._color = "2"; // Green for win
            gameOverElement._font_size = 32;
            elements.push_back(gameOverElement);

            // Add score display
            element_t scoreElement = {};
            scoreElement._type = TEXT;
            scoreElement._position = std::make_tuple(75, 40);
            scoreElement._text = "Final Score: " + std::to_string(this->_currentScore);
            scoreElement._color = "2"; // Green for win
            scoreElement._font_size = 24;
            elements.push_back(scoreElement);

            // Add restart prompt
            element_t restartElement = {};
            restartElement._type = TEXT;
            restartElement._position = std::make_tuple(75, 50);
            restartElement._text = "Press 'r' to play again";
            restartElement._color = "2"; 
            restartElement._font_size = 24;
            elements.push_back(restartElement);
        } else {
            // Game is still running, display timer and score as separate elements
            timerElement._type = TEXT;
            timerElement._position = std::make_tuple(75, 30);
            timerElement._text = "Time: " + std::to_string(static_cast<int>(this->_timeRemaining));

            // Change color based on time remaining
            if (this->_timeRemaining > 50) {
                timerElement._color = "2";  // Green when plenty of time
            } else if (this->_timeRemaining > 25) {
                timerElement._color = "3";  // Yellow when time is getting low
            } else {
                timerElement._color = "1";  // Red when time is critical
            }

            timerElement._font_size = 32;
            elements.push_back(timerElement);

            speedElement._type = TEXT;
            speedElement._position = std::make_tuple(75, 40);

            if (this->_clockSpeedMultiplier > 1.1f) {
                std::stringstream speedStream;
                speedStream << std::fixed << std::setprecision(1) << this->_clockSpeedMultiplier;
                speedElement._text = "Speed: x" + speedStream.str();
                speedElement._color = "3"; // Yellow for increased speed
            } else {
                speedElement._text = "Speed: Normal";
                speedElement._color = "2"; // Green for normal speed
            }

            speedElement._font_size = 24;
            elements.push_back(speedElement);

            scoreElement._type = TEXT;
            scoreElement._position = std::make_tuple(75, 50);
            scoreElement._text = "Score: " + std::to_string(this->_currentScore);
            scoreElement._color = "6"; // Cyan for score
            scoreElement._font_size = 24;
            elements.push_back(scoreElement);
        }

        return elements;
    }

    void Nibbler::handleWallCollision(std::pair<int, int>& newHead)
    {
        for (auto& wall : this->_walls) {
            if (newHead.first == wall.first && newHead.second == wall.second) {

                auto it = this->_turnWalls.find(wall);
                if (it != this->_turnWalls.end()) {

                    Direction newDirection = it->second;

                    newHead = this->_snake.front();
                    this->_direction = newDirection;
                    this->_lastDirection = newDirection;
                    this->_inputQueue = std::nullopt;

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
                    newHead = this->_snake.front();
                    this->_snakeStopped = true;
                }
                return;
            }
        }
    }

    bool Nibbler::isTSection(const std::pair<int, int>& position)
    {
        for (auto& tSection : this->_tSections) {
            if (tSection.first == position.first && tSection.second == position.second) {
                return true;
            }
        }
        return false;
    }

    bool Nibbler::checkWinCondition()
    {
        return this->_food.empty();
    }

    bool Nibbler::checkSelfCollision()
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

    bool Nibbler::isOppositeDirection(Direction dir1, Direction dir2)
    {
        return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
               (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
               (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
               (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
    }

    bool Nibbler::wouldHitWall(Direction testDirection, std::pair<int, int> position)
    {
        // Calculate the position after moving in the test direction
        std::pair<int, int> potentialPos = position;

        switch (testDirection) {
            case Direction::UP:
                potentialPos.first--;
                potentialPos.first--;
                potentialPos.first--;
                break;
            case Direction::DOWN:
                potentialPos.first++;
                potentialPos.first++;
                potentialPos.first++;
                break;
            case Direction::LEFT:
                potentialPos.second--;
                potentialPos.second--;
                potentialPos.second--;
                break;
            case Direction::RIGHT:
                potentialPos.second++;
                potentialPos.second++;
                potentialPos.second++;
                break;
        }

        // Check if this position is a wall
        for (auto& wall : this->_walls) {
            if (potentialPos.first == wall.first && potentialPos.second == wall.second) {
                return true;
            }
        }

        return false; // No wall collision
    }

    std::vector<element_t> Nibbler::handleEvents(std::string command)
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
        return (new arc::Nibbler(username, highScore));
    }

    void destroy(arc::IGame *instance)
    {
        delete instance;
    }

    const char *get_type()
    {
        return ("game");
    }
}