/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Menu
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class Menu {
        public:
            Menu();
            ~Menu();

            enum MenuState {
                LOGIN,
                GAME_SELECT,
                PAUSE
            };

            std::vector<element_t> getElements() const;
            void handleInput(const std::string &input);
            bool isAuthenticated() const;
            std::string getSelectedGame() const;
            bool shouldResume() const;
            bool shouldQuit() const;
            bool shouldReturnToMenu() const;
            void setAvailableGames(const std::vector<std::string> &games);

        private:
            void createLoginElements();
            void createGameSelectElements();
            void createPauseElements();
            void handleLoginInput(const std::string &input);
            void handleGameSelectInput(const std::string &input);
            void handlePauseInput(const std::string &input);
            MenuState _state;
            std::vector<element_t> _elements;
            std::string _username;
            std::string _password;
            bool _authenticated;
            std::string _selectedGame;
            size_t _selectedButton;
            bool _resume;
            bool _quit;
            bool _returnToMenu;
            std::vector<std::string> _availableGames;
    };
}
