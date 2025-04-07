/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** NewNibbler
*/

#pragma once

#include "Includes.hpp"

namespace arc {
    class NewNibbler : public AGame {
        public:
            NewNibbler(std::string username, int highScore);
            ~NewNibbler();

            std::vector<element_t>handleEvents(std::string command) override;

        private:

            enum class Direction {
                UP,
                DOWN,
                LEFT,
                RIGHT
            };



            enum class GameState {
                RUNNING,
                GAME_OVER,
                WIN
            };

            std::size_t _map_width = 40;
            std::size_t _map_height = 30;
            std::size_t _cell_size = 20;

            // Game elements
            std::vector<std::pair<int, int>> _snake;
            std::vector<std::pair<int, int>> _walls;
            std::map<std::pair<int, int>, Direction> _turnWalls;
            std::vector<std::pair<int, int>> _tSections;
            std::vector<std::pair<int, int>> _food;
            std::vector<std::string> _map;
            std::size_t _foodCount;
            bool _snakeStopped;

            // Game state
            Direction _direction;
            Direction _lastDirection;
            std::optional<Direction> _inputQueue; // Queue of size one to store previous input when relevant
            GameState _gameState;

            // Time management
            std::chrono::steady_clock::time_point _lastUpdateTime;
            std::chrono::milliseconds _updateInterval;

            bool loadMap(const std::string& mapPath);
            void initialize();
            void processInput(const std::string& command);
            void updateGame();
            void handleWallCollision(std::pair<int, int>& newHead);
            bool isTSection(const std::pair<int, int>& position);
            bool checkWinCondition();
            bool checkSelfCollision();
            bool isOppositeDirection(Direction dir1, Direction dir2);
            bool wouldHitWall(Direction testDirection, std::pair<int, int> position);

            std::vector<element_t> createGameElements();
    };
}
