/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Nibbler
*/

#include "Includes.hpp"

namespace arc{

    Nibbler::Nibbler(std::string username, int highScore) : AGame(username, highScore), _cell_size(20),
    _snakeStopped(false), _inputQueue(std::nullopt), _gameState(GameState::RUNNING),
    _lastUpdateTime(std::chrono::steady_clock::now()), _updateInterval(std::chrono::milliseconds(150))
    {
        this->_username = username;
        this->_highScore = highScore;
        this->_score = 0;
        if (!this->loadMap("assets/NibblerMaps/nibbler_map_01.txt")){
            // TODO: implement map loading error screen
        }
    }

    Nibbler::~Nibbler()
    {
    }

    std::vector<element_t> Nibbler::handleEvents(std::string command)
    {
        if ((this->_gameState == GameState::GAME_OVER && command == "r") || command == "r") {
            this->_gameState = GameState::RUNNING;
            this->_inputQueue = std::nullopt;
            this->_snakeStopped = false;
            this->_lastUpdateTime = std::chrono::steady_clock::now();
            if (!this->loadMap("assets/NibblerMaps/nibbler_map_01.txt")) {
                // TODO: implement map loading error screen
            }
            return (this->createElements());
        }
        this->processInput(command);
        this->updateGame();
        return (this->createElements());
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
        this->_snake.clear();
        this->_walls.clear();
        this->_turnWalls.clear();
        this->_tSections.clear();
        this->_food.clear();
        this->_foodCount = 0;
        this->_map_width = 0;
        this->_map_height = 0;
        while (std::getline(mapFile, line)) {
            if (!line.empty()) {
                this->_map.push_back(line);
                this->_map_height++;
                if (line.size() > this->_map_width)
                    this->_map_width = line.size();
            }
        }
        if (this->_map.empty())
            return (false);
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
        return (true);
    }

    void Nibbler::processInput(const std::string &command)
    {
        Direction newInput = this->_direction;
        bool validInput = false;

        if (this->_gameState != GameState::RUNNING)
            return;
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
        if (!validInput)
            return;
        if (!this->wouldHitWall(newInput, this->_snake.front())) {
            this->_direction = newInput;
            this->_inputQueue = std::nullopt;
        } else {
            this->_inputQueue = newInput;
        }
    }

    void Nibbler::handleWallCollision(std::pair<int, int> &newHead)
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

    void Nibbler::updateGame()
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->_lastUpdateTime);
        std::pair<int, int> head = this->_snake.front();
        std::pair<int, int> newHead = head;
        bool foodEaten = false;

        if (elapsedTime < this->_updateInterval)
            return;
        this->_lastUpdateTime = currentTime;
        if (this->_gameState != GameState::RUNNING)
            return;
        if (this->_inputQueue) {
            Direction queuedDirection = *this->_inputQueue;
            if (!this->wouldHitWall(queuedDirection, head)) {
                this->_direction = queuedDirection;
                this->_inputQueue = std::nullopt;
            }
        }
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
        this->handleWallCollision(newHead);
        if (newHead.first == head.first && newHead.second == head.second)
            return;
        this->_snake.insert(this->_snake.begin(), newHead);
        for (auto it = this->_food.begin(); it != this->_food.end(); ) {
            if (newHead.first == it->first && newHead.second == it->second) {
                it = this->_food.erase(it);
                foodEaten = true;
                this->_score += 10;
                break;
            } else {
                ++it;
            }
        }
        if (!foodEaten)
            this->_snake.pop_back();
        if (this->checkWinCondition())
            this->_gameState = GameState::WIN;
        if (this->checkSelfCollision())
            this->_gameState = GameState::GAME_OVER;
    }

    void Nibbler::updateHighScore()
    {
        std::ifstream infile("accounts.txt");
        std::ofstream outfile("accounts_tmp.txt");
        std::string line = "";

        if (this->_score > this->_highScore) {
            this->_highScore = this->_score;
            while (std::getline(infile, line)) {
                std::size_t pos = line.find(this->_username + ":");
                if (pos != std::string::npos) {
                    std::size_t snakePos = line.find("nibbler=");
                    if (snakePos != std::string::npos) {
                        std::size_t endPos = line.find(':', snakePos);
                        std::string updatedLine = line.substr(0, snakePos + 8) +
                        std::to_string(this->_highScore) +
                        (endPos != std::string::npos ? line.substr(endPos) : "");
                        outfile << updatedLine << std::endl;
                    } else {
                        outfile << line << std::endl;
                    }
                } else {
                    outfile << line << std::endl;
                }
            }
            infile.close();
            outfile.close();
            std::remove("accounts.txt");
            std::rename("accounts_tmp.txt", "accounts.txt");
        }
    }

    bool Nibbler::isTSection(const std::pair<int, int> &position)
    {
        for (auto &tSection : this->_tSections) {
            if (tSection.first == position.first && tSection.second == position.second)
                return (true);
        }
        return (false);
    }

    bool Nibbler::checkWinCondition()
    {
        return (this->_food.empty());
    }

    bool Nibbler::checkSelfCollision()
    {
        std::pair<int, int> head = this->_snake.front();

        for (std::size_t i = 3; i < this->_snake.size(); i++) {
            if (head.first == this->_snake[i].first && head.second == this->_snake[i].second) {
                updateHighScore();
                return (true);
            }
        }
        return (false);
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
        std::pair<int, int> potentialPos = position;

        switch (testDirection) {
            case Direction::UP:
                potentialPos.first--;
                potentialPos.first--;
                break;
            case Direction::DOWN:
                potentialPos.first++;
                potentialPos.first++;
                break;
            case Direction::LEFT:
                potentialPos.second--;
                potentialPos.second--;
                break;
            case Direction::RIGHT:
                potentialPos.second++;
                potentialPos.second++;
                break;
        }
        for (auto& wall : this->_walls) {
            if (potentialPos.first == wall.first && potentialPos.second == wall.second)
                return (true);
        }
        return (false);
    }

    std::vector<element_t> Nibbler::createElements()
    {
        std::vector<element_t> elements;
        element_t border = {};
        element_t wallElement = {};
        element_t snakeSegment = {};
        element_t foodElement = {};
        element_t scoreText = {};
        element_t highScoreText = {};
        element_t gameOverText = {};
        element_t finalScoreText = {};
        int elementY = 0;
        int elementX = 0;

        border._type = BORDER;
        border._position = std::make_tuple(0, 0);
        border._size = std::make_tuple(this->_map_height + 2, this->_map_width +2);
        border._color = "6";
        border._display = false;
        elements.push_back(border);
        for (const auto &wall : this->_walls) {
            wallElement._type = RECTANGLE;
            elementY = std::get<0>(wall);
            elementX = std::get<1>(wall);
            wallElement._position = std::make_tuple(elementY, elementX);
            wallElement._size = std::make_tuple(this->_cell_size, this->_cell_size);
            wallElement._color = "3";
            elements.push_back(wallElement);
        }
        for (const auto &segment : this->_snake) {
            snakeSegment._type = RECTANGLE;
            elementY = std::get<0>(segment);
            elementX = std::get<1>(segment);
            snakeSegment._position = std::make_tuple(elementY, elementX);
            snakeSegment._size = std::make_tuple(this->_cell_size, this->_cell_size);
            if (segment == this->_snake.front())
                snakeSegment._color = "2";
            else
                snakeSegment._color = "4";
            elements.push_back(snakeSegment);
        }
        for (const auto &food : this->_food) {
            foodElement._type = CIRCLE;
            elementY = std::get<0>(food);
            elementX = std::get<1>(food);
            foodElement._position = std::make_tuple(elementY, elementX);
            foodElement._size = std::make_tuple(18, 18);
            foodElement._color = "1";
            elements.push_back(foodElement);
        }
        highScoreText._type = TEXT;
        highScoreText._text = "Highest Score: " + std::to_string(this->_highScore);
        highScoreText._position = std::make_tuple(50, 2);
        highScoreText._color = "5";
        elements.push_back(highScoreText);
        scoreText._type = TEXT;
        scoreText._text = "Score: " + std::to_string(this->_score);
        scoreText._position = std::make_tuple(50, 5);
        scoreText._color = "5";
        elements.push_back(scoreText);
        if (this->_gameState == GameState::GAME_OVER) {
            gameOverText._type = TEXT;
            gameOverText._text = "GAME OVER - Press R to restart";
            gameOverText._position = std::make_tuple(50, 40);
            gameOverText._color = "1";
            gameOverText._font_size = 36;
            elements.push_back(gameOverText);
            finalScoreText._type = TEXT;
            finalScoreText._text = "Final Score: " + std::to_string(this->_score);
            finalScoreText._position = std::make_tuple(50, 50);
            finalScoreText._color = "5";
            finalScoreText._font_size = 28;
            elements.push_back(finalScoreText);
        }
        return (elements);
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