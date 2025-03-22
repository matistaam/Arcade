/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Snake
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    enum Direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    class Snake : public AGame {
        public:
            Snake();
            ~Snake() override;

            std::vector<element_t> handleEvents(std::string command) override;

        private:
            void initGame();
            void moveSnake();
            void handleCollisions();
            void spawnFood();
            std::vector<element_t> createElements();
            bool isPositionInSnake(int y, int x) const;
            void updateHighScore();

            static const int WIDTH = 40;
            static const int HEIGHT = 30;
            static const int CELL_SIZE = 20;
            static const int INITIAL_SNAKE_SIZE = 4;

            bool _gameOver;
            int _score;
            int _highScore;
            Direction _direction;
            Direction _lastDirection;
            std::list<std::tuple<int, int>> _snake;
            std::tuple<int, int> _food;

            std::chrono::time_point<std::chrono::steady_clock> _lastUpdateTime;
            int _updateInterval;

            std::mt19937 _rng;
    };
}
