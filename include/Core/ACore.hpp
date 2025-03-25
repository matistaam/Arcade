/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** ACore
*/

#pragma once
#include "Includes.hpp"

namespace arc
{
    class ACore : public ICore {
        public:
        ACore(std::string path);
        ~ACore();

        void setGraphical(IGraphical *Graphical) override;
        void setGame(IGame *Game) override;
        void display(std::vector<element_t> elements) override;

        std::string update();
        void loadGame(const std::string &name);
        void switchGraphicalLibrary(const std::string &name);
        std::vector<std::string> getAvailableGames();
        std::vector<std::string> getAvailableGraphicalLibs();

        std::vector<std::string> _availableGames;
        std::vector<std::string> _availableGraphicalLibs;

        private:
            Menu _menu;
            size_t _currentLibIndex;
            void *_handle;
            void *_gameHandle;
            bool _inGame;
            bool _isPaused;
    };
}
