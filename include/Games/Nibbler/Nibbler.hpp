/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Nibbler
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class Nibbler : public AGame {
        public:
            Nibbler(std::string username, int highScore);
            ~Nibbler();

            std::vector<element_t>handleEvents(std::string command) override;

        private:
            enum class Direction {
                UP,
                RIGHT,
                DOWN,
                LEFT
            };

            enum class GameState {
                RUNNING,
                GAME_OVER,
                WIN
            };

            bool loadMap(const std::string &mapPath);
            void processInput(const std::string &command);
            void handleWallCollision(std::pair<int, int> &newHead);
            void updateGame();
            void updateHighScore();
            bool isTSection(const std::pair<int, int> &position);
            bool checkWinCondition();
            bool checkSelfCollision();
            bool isOppositeDirection(Direction dir1, Direction dir2);
            bool wouldHitWall(Direction testDirection, std::pair<int, int> position);
            std::vector<element_t> createElements();

            std::size_t _map_width = 40;
            std::size_t _map_height = 30;
            std::size_t _cell_size = 20;

            std::string _username;
            int _score;
            int _highScore;
            bool _snakeStopped;
            Direction _direction;
            Direction _lastDirection;
            std::vector<std::pair<int, int>> _snake;
            std::vector<std::pair<int, int>> _food;
            std::vector<std::pair<int, int>> _walls;
            std::map<std::pair<int, int>, Direction> _turnWalls;
            std::vector<std::pair<int, int>> _tSections;
            std::vector<std::string> _map;
            std::size_t _foodCount;

            std::optional<Direction> _inputQueue;
            GameState _gameState;

            std::chrono::steady_clock::time_point _lastUpdateTime;
            std::chrono::milliseconds _updateInterval;
    };
}
