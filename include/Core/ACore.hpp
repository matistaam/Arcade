/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** ACore
*/

#pragma once
#include "ICore.hpp"
#include "AGraphical.hpp"
#include "AGame.hpp"
#include <string>

class ACore : public ICore {
    public:
        ACore(std::string path);
        ~ACore();

        void setGraphical(IGraphical *Graphical);
        void setGame(IGame *Game);
        void display(std::vector<element_t> elements);

    private:
        IGraphical *_graphical;
        IGame *_game;
};
