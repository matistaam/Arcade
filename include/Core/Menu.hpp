/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Menu
*/

#pragma once
#include "Includes.hpp"
#include <map>

namespace arc {
    class Menu {
        public:
            Menu();
            ~Menu();

            enum MenuState {
                LOGIN_SIGNUP,
                LOGIN,
                SIGN_UP,
                GAME_SELECT,
                PAUSE
            };

            void setAvailableGames(const std::vector<std::string> &games);
            std::vector<element_t> getElements() const;
            void handleInput(const std::string &input);
            bool isAuthenticated() const;
            std::string getSelectedGame() const;
            bool shouldResume() const;
            bool shouldQuit() const;
            bool shouldReturnToMenu() const;
            void updateHighScore(const std::string &game, int score);
            int getHighScore(const std::string &game) const;

        private:
            bool saveAccount(const std::string &username, const std::string &password);
            bool verifyAccount(const std::string &username, const std::string &password);
            bool checkUsernameExists(const std::string &username);
            void createLoginSignUpElements();
            void createLoginElements();
            void createSignUpElements();
            void createGameSelectElements();
            void createPauseElements();
            void handleLoginSignUpInput(const std::string &input);
            void handleLoginInput(const std::string &input);
            void handleSignUpInput(const std::string &input);
            void handleGameSelectInput(const std::string &input);
            void handlePauseInput(const std::string &input);
            MenuState _state;
            std::vector<element_t> _elements;
            std::string _username;
            std::string _password;
            bool _authenticated;
            std::map<std::string, int> _highScores;
            std::string _selectedGame;
            size_t _selectedButton;
            bool _resume;
            bool _quit;
            bool _returnToMenu;
            std::vector<std::string> _availableGames;
    };
}
