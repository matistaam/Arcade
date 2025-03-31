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

            bool loadMap(const std::string& mapPath);
            void initialize();
            void processInput(const std::string& command);


            std::vector<element_t> createGameElements();
    };
}
