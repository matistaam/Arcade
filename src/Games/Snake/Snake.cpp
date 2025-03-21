/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Snake
*/

#include "Includes.hpp"

namespace arc {
    Snake::Snake() : _rng(std::random_device{}())
    {
        initGame();
    }

    Snake::~Snake()
    {
    }

    void Snake::initGame()
    {
        _gameOver = false;
        _score = 0;
        _direction = LEFT;
        _lastDirection = LEFT;
        _updateInterval = 150;
        _lastUpdateTime = std::chrono::steady_clock::now();
        
        _snake.clear();
        
        int centerY = HEIGHT / 2;
        int centerX = WIDTH / 2;
        
        for (int i = 0; i < INITIAL_SNAKE_SIZE; i++) {
            _snake.push_front(std::make_tuple(centerY, centerX - i));
        }
        
        spawnFood();
    }

    void Snake::spawnFood()
    {
        std::uniform_int_distribution<int> distY(0, HEIGHT - 1);
        std::uniform_int_distribution<int> distX(0, WIDTH - 1);
        
        int foodY, foodX;
        
        do {
            foodY = distY(_rng);
            foodX = distX(_rng);
        } while (isPositionInSnake(foodY, foodX));
        
        _food = std::make_tuple(foodY, foodX);
    }

    bool Snake::isPositionInSnake(int y, int x) const
    {
        for (const auto &segment : _snake) {
            if (std::get<0>(segment) == y && std::get<1>(segment) == x)
                return true;
        }
        return false;
    }

    void Snake::moveSnake()
    {
        int headY = std::get<0>(_snake.front());
        int headX = std::get<1>(_snake.front());
        
        switch (_direction) {
            case UP:
                headY--;
                break;
            case RIGHT:
                headX++;
                break;
            case DOWN:
                headY++;
                break;
            case LEFT:
                headX--;
                break;
        }
        
        if (headY < 0)
            headY = HEIGHT - 1;
        else if (headY >= HEIGHT)
            headY = 0;
        
        if (headX < 0)
            headX = WIDTH - 1;
        else if (headX >= WIDTH)
            headX = 0;
        
        _snake.push_front(std::make_tuple(headY, headX));
        
        if (headY == std::get<0>(_food) && headX == std::get<1>(_food)) {
            _score += 10;
            
            if (_updateInterval > 50)
                _updateInterval -= 2;
                
            spawnFood();
        } else {
            _snake.pop_back();
        }
        
        _lastDirection = _direction;
    }

    void Snake::handleCollisions()
    {
        int headY = std::get<0>(_snake.front());
        int headX = std::get<1>(_snake.front());
        
        auto it = _snake.begin();
        it++;
        
        while (it != _snake.end()) {
            if (headY == std::get<0>(*it) && headX == std::get<1>(*it)) {
                _gameOver = true;
                return;
            }
            it++;
        }
    }

    std::vector<element_t> Snake::createElements()
    {
        std::vector<element_t> elements;
        
        element_t border;
        border._type = RECTANGLE;
        border._position = std::make_tuple(HEIGHT * CELL_SIZE / 2, WIDTH * CELL_SIZE / 2);
        border._size = std::make_tuple(HEIGHT * CELL_SIZE, WIDTH * CELL_SIZE);
        border._color = "6";
        elements.push_back(border);
        
        for (const auto &segment : _snake) {
            element_t snakeSegment;
            snakeSegment._type = RECTANGLE;
            
            int segmentY = std::get<0>(segment) * CELL_SIZE + CELL_SIZE / 2;
            int segmentX = std::get<1>(segment) * CELL_SIZE + CELL_SIZE / 2;
            
            snakeSegment._position = std::make_tuple(segmentY, segmentX);
            snakeSegment._size = std::make_tuple(CELL_SIZE - 2, CELL_SIZE - 2);
            
            if (segment == _snake.front())
                snakeSegment._color = "2";
            else
                snakeSegment._color = "3";
                
            elements.push_back(snakeSegment);
        }
        
        element_t food;
        food._type = CIRCLE;
        
        int foodY = std::get<0>(_food) * CELL_SIZE + CELL_SIZE / 2;
        int foodX = std::get<1>(_food) * CELL_SIZE + CELL_SIZE / 2;
        
        food._position = std::make_tuple(foodY, foodX);
        food._size = std::make_tuple(CELL_SIZE - 2, CELL_SIZE - 2);
        food._color = "1";
        elements.push_back(food);
        
        element_t scoreText;
        scoreText._type = TEXT;
        scoreText._text = "Score: " + std::to_string(_score);
        scoreText._position = std::make_tuple(10, 10);
        scoreText._color = "5";
        elements.push_back(scoreText);
        
        if (_gameOver) {
            element_t gameOverText;
            gameOverText._type = TEXT;
            gameOverText._text = "GAME OVER - Press R to restart";
            gameOverText._position = std::make_tuple(50, 40); // Center of screen
            gameOverText._color = "1";
            gameOverText._font_size = 36;
            elements.push_back(gameOverText);
            
            element_t finalScoreText;
            finalScoreText._type = TEXT;
            finalScoreText._text = "Final Score: " + std::to_string(_score);
            finalScoreText._position = std::make_tuple(50, 50); // Below game over text
            finalScoreText._color = "5";
            finalScoreText._font_size = 28;
            elements.push_back(finalScoreText);
        }

        return elements;
    }

    std::vector<element_t> Snake::handleEvents(std::string command)
    {
        if (_gameOver && command == "R") {
            initGame();
            return createElements();
        }
        
        if (_gameOver) {
            return createElements();
        }
        
        if (!_gameOver) {
            if (command == "UP" && _lastDirection != DOWN)
                _direction = UP;
            else if (command == "RIGHT" && _lastDirection != LEFT)
                _direction = RIGHT;
            else if (command == "DOWN" && _lastDirection != UP)
                _direction = DOWN;
            else if (command == "LEFT" && _lastDirection != RIGHT)
                _direction = LEFT;
        }
        
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - _lastUpdateTime).count();
            
        if (elapsedTime >= _updateInterval && !_gameOver) {
            moveSnake();
            handleCollisions();
            _lastUpdateTime = currentTime;
        }
        
        return createElements();
    }
}

extern "C" {
    arc::IGame *create()
    {
        return new arc::Snake();
    }

    void destroy(arc::IGame *instance)
    {
        delete instance;
    }
}
