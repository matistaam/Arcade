/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** ICore
*/

#pragma once
#include "IGraphical.hpp"
#include "IGame.hpp"

namespace arc {
    class ICore {
        public:
            virtual ~ICore() = default;

            virtual void setGraphical(IGraphical *Graphical) = 0;
            virtual void setGame(IGame *Game) = 0;
            virtual void display(std::vector<element_t> elements) = 0;

        protected:
            IGraphical *_graphical;
            IGame *_game;
    };
}
