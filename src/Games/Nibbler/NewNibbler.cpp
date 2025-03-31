/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** NewNibbler
*/

#include "Includes.hpp"

namespace arc{
    NewNibbler::NewNibbler(std::string username, int highScore) : AGame(username, highScore)
    {
        this->initialize();
    }

    NewNibbler::~NewNibbler()
    {
        // Nothing to clean up
    }

    void NewNibbler::initialize()
    {
        this->_gameState = GameState::RUNNING;
        this->_inputQueue = std::nullopt;

        // Load the map
        // TODO: implement a map iteration system
        if (!this->loadMap("assets/NibblerMaps/nibbler_map_01.txt")){
            // TODO: implement map loading error screen
        }
    }

    bool NewNibbler::loadMap(const std::string& mapPath)
    {
        std::ifstream mapFile(mapPath);
        if (!mapFile.is_open()) {
            return false;
        }

        this->_map.clear();
        this->_snake.clear();
        this->_walls.clear();
        this->_turnWalls.clear();
        this->_tSections.clear();
        this->_food.clear();
        this->_foodCount = 0;

        std::string line;
        while (std::getline(mapFile, line)) {
            if (!line.empty()) {
                this->_map.push_back(line);
            }
        }

        if (this->_map.empty()) {
            return false;
        }

        // Process the map characters to create walls and other elements
        for (size_t y = 0; y < _map.size(); y++) {
            for (size_t x = 0; x < _map[y].size(); x++) {
                char tile = this->_map[y][x];
                std::pair<int, int> pos = std::make_pair(x, y);

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
        return true;
    }

    void NewNibbler::processInput(const std::string& command)
    {
        if (this->_gameState != GameState::RUNNING){
            return;
        }


    }

    std::vector<element_t> NewNibbler::createGameElements()
    {
        // TODO: Implement
    }

    std::vector<element_t> NewNibbler::handleEvents(std::string command)
    {
        // If r is pressed, re-initialize game
        if (command == "r"){
            this->initialize();
            return this->createGameElements();
        }

        // Process input
        this->processInput(command);
    }

extern "C" {
    arc::IGame *create(const std::string &username, int highScore)
    {
        return new arc::NewNibbler(username, highScore);
    }

    void destroy(arc::IGame *instance)
    {
        delete instance;
    }

    const char *get_type()
    {
        return "game";
    }
}