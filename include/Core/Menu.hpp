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
                GAME_SELECT
            };

            std::vector<element_t> getElements() const;
            void handleInput(const std::string &input);
            bool isAuthenticated() const;
            std::string getSelectedGame() const;

        private:
            void createLoginElements();
            void createGameSelectElements();
            void handleLoginInput(const std::string &input);
            void handleGameSelectInput(const std::string &input);

            MenuState _state;
            std::vector<element_t> _elements;
            std::string _username;
            std::string _password;
            bool _authenticated;
            std::string _selectedGame;
            size_t _selectedButton;
    };
}
