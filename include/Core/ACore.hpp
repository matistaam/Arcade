/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** ACore
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class ACore : public ICore {
        public:
            ACore(std::string path);
            ~ACore();

            void setGraphical(IGraphical *Graphical) override;
            void setGame(IGame *Game) override;
            void display(std::vector<element_t> elements) override;
            std::string update();
            void loadGame(const std::string &name);

        private:
            void *_handle;
            void *_gameHandle;
            Menu _menu;
            bool _inGame;
            bool _isPaused;
    };
}
