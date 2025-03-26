/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Menu
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class Menu : public IScoreManager {
        public:
            Menu();
            ~Menu();

            std::vector<element_t> getElements() const;
            std::string getSelectedGame() const;

            bool isAuthenticated() const;
            bool shouldResume() const;
            bool shouldReturnToMenu() const;
            bool shouldQuit() const;

            void setAvailableGames(const std::vector<std::string> &games);

            void handleInput(const std::string &input);

            enum MenuState {
                LOGIN_SIGNUP,
                LOGIN,
                SIGN_UP,
                GAME_SELECT,
                PAUSE
            };

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

            std::map<std::string, int> _highScores;
            std::vector<element_t> _elements;
            std::vector<std::string> _availableGames;
            std::string _username;
            std::string _password;
            std::string _selectedGame;
            MenuState _state;
            size_t _selectedButton;
            bool _authenticated;
            bool _resume;
            bool _returnToMenu;
            bool _quit;
    };
}
