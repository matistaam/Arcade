/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Core
*/

#pragma once
#include "Includes.hpp"

namespace arc
{
    class Core {
        public:
            Core(std::string path);
            ~Core();

            void setGraphical(IGraphical *Graphical);
            void setGame(IGame *Game);
            void display(std::vector<element_t> elements);

            std::string update();
            void loadGame(const std::string &name);
            void switchGraphicalLibrary(const std::string &name);
            std::vector<std::string> getAvailableGames();
            std::vector<std::string> getAvailableGraphicalLibs();

            std::vector<std::string> _availableGames;
            std::vector<std::string> _availableGraphicalLibs;

        private:
            IGraphical *_graphical;
            IGame *_game;
            Menu _menu;
            size_t _currentLibIndex;
            DlLoader _dlLoader;
            bool _inGame;
            bool _isPaused;
    };
}
