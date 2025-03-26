/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** AGame
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class AGame : public IGame {
        public:
            AGame(std::string username, int highScore);
            ~AGame();

            virtual std::vector<element_t> handleEvents(std::string command) = 0;

        protected:
            std::vector<element_t> _elements;
            std::string _username;
            int _highScore;
    };
}
