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
#include <queue>

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
            Direction _lastDirection;
            
            // Direction queue to store future inputs
            std::queue<Direction> _directionQueue;
            size_t _maxQueueSize = 3; // Limit the queue size to prevent too many stored inputs

            // Game elements
            std::vector<std::pair<int, int>> _snake;
            std::vector<std::pair<int, int>> _walls;
            std::map<std::pair<int, int>, Direction> _turnWalls; // Stores special walls with direction
            std::vector<std::pair<int, int>> _tSections; // Store T-section positions
            std::vector<std::pair<int, int>> _food;
            std::vector<std::string> _map; // Store the map data
            int _initialFoodCount; // Track initial food count for win condition
            bool _stopped; // Flag to track if snake is stopped at a T-section
            
            // Time management
            std::chrono::steady_clock::time_point _lastUpdateTime;
            std::chrono::milliseconds _updateInterval;
            
            // Clock management
            int _clockValue; // Current clock value (time remaining)
            int _maxClockValue; // Maximum clock value
            int _turnsWithoutFood; // Count turns without eating food
            std::chrono::steady_clock::time_point _lastClockUpdateTime;
            std::chrono::milliseconds _clockUpdateInterval;

            // Game logic methods
            bool loadMap(const std::string& mapPath);
            void initializeWalls();
            void placeFood(int count = 1);
            bool checkCollision();
            void handleWallCollision(std::pair<int, int>& newHead); // Method for handling directional walls
            bool isTSection(const std::pair<int, int>& position); // Method to check if position is a T-section
            void updateGame();
            void updateClock(); 
            void processInput(const std::string& command);
            void processDirectionQueue(); // New method to handle the direction queue
            std::vector<element_t> createGameElements();
            bool checkWinCondition(); // Check if all food has been collected
            bool wouldHitWall(Direction newDirection); // Method to check if the snake would hit a wall
            bool wouldHitWall(Direction newDirection, std::pair<int, int> position); // Overload to check from a specific position
            bool isOppositeDirection(Direction dir1, Direction dir2); // Check if directions are opposite
    };
}
