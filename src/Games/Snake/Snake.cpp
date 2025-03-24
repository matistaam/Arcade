/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Snake
*/

#include "Includes.hpp"

namespace arc {
    Snake::Snake() : _scoreManager(nullptr), _highScore(0), _rng(std::random_device{}())
    {
        initGame();
    }

    Snake::~Snake()
    {
    }

    void Snake::setScoreManager(IScoreManager *scoreManager)
    {
        this->_scoreManager = scoreManager;
        if (this->_scoreManager)
            this->_highScore = this->_scoreManager->getHighScore("snake");
    }

    void Snake::initGame()
    {
        int centerY = HEIGHT / 2;
        int centerX = WIDTH / 2;

        this->_gameOver = false;
        this->_score = 0;
        this->_direction = LEFT;
        this->_lastDirection = LEFT;
        this->_updateInterval = 150;
        this->_lastUpdateTime = std::chrono::steady_clock::now();
        this->_snake.clear();
        for (int i = 0; i < INITIAL_SNAKE_SIZE; i++)
            this->_snake.push_front(std::make_tuple(centerY, centerX - i));
        spawnFood();
    }

    void Snake::spawnFood()
    {
        std::uniform_int_distribution<int> distY(0, HEIGHT - 1);
        std::uniform_int_distribution<int> distX(0, WIDTH - 1);
        int foodY = 0;
        int foodX = 0;

        do {
            foodY = distY(this->_rng);
            foodX = distX(this->_rng);
        } while (isPositionInSnake(foodY, foodX));
        this->_food = std::make_tuple(foodY, foodX);
    }

    bool Snake::isPositionInSnake(int y, int x) const
    {
        for (const auto &segment : this->_snake) {
            if (std::get<0>(segment) == y && std::get<1>(segment) == x)
                return (true);
        }
        return (false);
    }

    void Snake::moveSnake()
    {
        int headY = std::get<0>(this->_snake.front());
        int headX = std::get<1>(this->_snake.front());

        switch (this->_direction) {
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
        this->_snake.push_front(std::make_tuple(headY, headX));
        if (headY == std::get<0>(this->_food) && headX == std::get<1>(this->_food)) {
            this->_score += 10;
            if (this->_updateInterval > 50)
                this->_updateInterval -= 2;
            spawnFood();
        } else {
            this->_snake.pop_back();
        }
        this->_lastDirection = this->_direction;
    }

    void Snake::updateHighScore()
    {
        if (this->_score > this->_highScore)
            this->_highScore = this->_score;
        if (this->_scoreManager)
            this->_scoreManager->updateHighScore("snake", this->_score);
    }

    void Snake::handleCollisions()
    {
        int headY = std::get<0>(this->_snake.front());
        int headX = std::get<1>(this->_snake.front());
        auto it = this->_snake.begin();

        it++;
        while (it != this->_snake.end()) {
            if (headY == std::get<0>(*it) && headX == std::get<1>(*it)) {
                this->_gameOver = true;
                updateHighScore();
                return;
            }
            it++;
        }
    }

    std::vector<element_t> Snake::createElements()
    {
        std::vector<element_t> elements = {};
        element_t background = {};
        element_t snakeSegment = {};
        element_t food = {};
        element_t scoreText = {};
        element_t highScoreText = {};
        element_t gameOverText = {};
        element_t finalScoreText = {};
        int segmentY = 0;
        int segmentX = 0;

        background._type = IMAGE;
        background._image_path = "assets/snake_background.png";
        background._position = std::make_tuple(0, 0);
        background._size = std::make_tuple(600, 800);
        elements.push_back(background);
        for (const auto &segment : this->_snake) {
            snakeSegment._type = RECTANGLE;
            segmentY = std::get<0>(segment);
            segmentX = std::get<1>(segment);
            snakeSegment._position = std::make_tuple(segmentY, segmentX);
            snakeSegment._size = std::make_tuple(18, 18);
            if (segment == this->_snake.front())
                snakeSegment._color = "2";
            else
                snakeSegment._color = "3";
            elements.push_back(snakeSegment);
        }
        food._type = CIRCLE;
        food._position = std::make_tuple(std::get<0>(this->_food), std::get<1>(this->_food));
        food._size = std::make_tuple(18, 18);
        food._color = "1";
        elements.push_back(food);
        scoreText._type = TEXT;
        scoreText._text = "Score: " + std::to_string(this->_score);
        scoreText._position = std::make_tuple(10, 20);
        scoreText._color = "5";
        elements.push_back(scoreText);
        highScoreText._type = TEXT;
        highScoreText._text = "Highest Score: " + std::to_string(this->_highScore);
        highScoreText._position = std::make_tuple(15, 5);
        highScoreText._color = "5";
        elements.push_back(highScoreText);
        if (this->_gameOver) {
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

    std::vector<element_t> Snake::handleEvents(std::string command)
    {
        if (this->_gameOver && command == "r") {
            initGame();
            return (createElements());
        }
        if (this->_gameOver)
            return (createElements());
        if (!this->_gameOver) {
            if (command == "UP" && this->_lastDirection != DOWN)
                this->_direction = UP;
            else if (command == "RIGHT" && this->_lastDirection != LEFT)
                this->_direction = RIGHT;
            else if (command == "DOWN" && this->_lastDirection != UP)
                this->_direction = DOWN;
            else if (command == "LEFT" && this->_lastDirection != RIGHT)
                this->_direction = LEFT;
        }
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - this->_lastUpdateTime).count();
        if (elapsedTime >= this->_updateInterval && !this->_gameOver) {
            moveSnake();
            handleCollisions();
            this->_lastUpdateTime = currentTime;
        }
        return (createElements());
    }
}

extern "C" {
    arc::IGame *create()
    {
        return (new arc::Snake());
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
