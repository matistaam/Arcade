/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** DlLoader
*/

#pragma once

#include <string>
#include <vector>
#include <dlfcn.h>

namespace arc {
    class IGraphical;
    class IGame;

    class DlLoader {
        public:
            DlLoader();
            ~DlLoader();

            IGraphical *loadGraphicalLibrary(const std::string &path);
            void switchGraphicalLibrary(const std::string &name);
            void closeGraphicalLibrary();
            std::vector<std::string> getAvailableGraphicalLibs();

            IGame *loadGame(const std::string &name, const std::string &username, int highScore);
            void closeGameLibrary();
            std::vector<std::string> getAvailableGames();

        private:
            void *_handle;
            void *_gameHandle;
            IGraphical *_graphical;
            IGame *_game;
    };
}