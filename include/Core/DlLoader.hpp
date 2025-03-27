/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** DlLoader
*/

#pragma once

#include "Includes.hpp"

namespace arc {
    class DlLoader {
        public:
            DlLoader();
            ~DlLoader();

            IGraphical *loadGraphicalLibrary(const std::string &path);
            void unloadGraphicalLibrary(IGraphical *graphical);
            std::vector<std::string> getAvailableGraphicalLibs();

            IGame *loadGame(const std::string &name, const std::string &username, int highScore);
            void unloadGame(IGame *game);
            std::vector<std::string> getAvailableGames();

        private:
            void *_graphicalHandle;
            void *_gameHandle;
    };
}
