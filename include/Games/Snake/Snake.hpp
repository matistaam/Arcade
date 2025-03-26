/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Snake
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class Snake : public AGame {
        public:
            Snake(std::string username, int highScore);
            ~Snake();

            std::vector<element_t> handleEvents(std::string command) override;

            enum Direction {
                UP,
                RIGHT,
                DOWN,
                LEFT
            };

        private:
            void restartGame();
            void moveSnake();
            void handleCollisions();
            void spawnFood();
            void updateHighScore();
            bool isPositionInSnake(int y, int x) const;
            std::vector<element_t> createElements();

            static const int WIDTH = 40;
            static const int HEIGHT = 30;
            static const int CELL_SIZE = 20;
            static const int INITIAL_SNAKE_SIZE = 4;

            std::string _username;
            int _score;
            int _highScore;
            int _updateInterval;
            bool _gameOver;
            Direction _direction;
            Direction _lastDirection;
            std::list<std::tuple<int, int>> _snake;
            std::tuple<int, int> _food;
            std::chrono::time_point<std::chrono::steady_clock> _lastUpdateTime;
            std::mt19937 _rng;
    };
}
