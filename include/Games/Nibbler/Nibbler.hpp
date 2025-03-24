/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Nibbler
*/

#pragma once
#include "Includes.hpp"
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <map>

namespace arc {
    class Nibbler : public AGame {
        public:
            Nibbler();
            ~Nibbler() override;

            std::vector<element_t> handleEvents(std::string command) override;

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

            // Game state
            GameState _gameState;
            int _score;
            size_t _width;
            size_t _height;
            int _cellSize;
            Direction _direction;

            // Game elements
            std::vector<std::pair<int, int>> _snake;
            std::vector<std::pair<int, int>> _walls;
            std::map<std::pair<int, int>, Direction> _turnWalls; // Stores special walls with direction
            std::vector<std::pair<int, int>> _food;
            std::vector<std::string> _map; // Store the map data
            int _initialFoodCount; // Track initial food count for win condition

            // Time management
            std::chrono::steady_clock::time_point _lastUpdateTime;
            std::chrono::milliseconds _updateInterval;

            // Game logic methods
            bool loadMap(const std::string& mapPath);
            void initializeWalls();
            void placeFood(int count = 1);
            bool checkCollision();
            void handleWallCollision(std::pair<int, int>& newHead); // New method for handling directional walls
            void updateGame();
            void processInput(const std::string& command);
            std::vector<element_t> createGameElements();
            bool checkWinCondition(); // Check if all food has been collected
    };
}
