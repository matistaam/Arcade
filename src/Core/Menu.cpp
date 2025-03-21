/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Menu
*/

#include "Includes.hpp"

namespace arc {
    Menu::Menu() : _state(LOGIN), _authenticated(false), _selectedButton(0)
    {
        createLoginElements();
    }

    Menu::~Menu()
    {
    }

    std::vector<element_t> Menu::getElements() const
    {
        return (this->_elements);
    }

    void Menu::handleInput(const std::string &input)
    {
        if (this->_state == LOGIN)
            handleLoginInput(input);
        else
            handleGameSelectInput(input);
    }

    bool Menu::isAuthenticated() const
    {
        return (this->_authenticated);
    }

    std::string Menu::getSelectedGame() const
    {
        return (this->_selectedGame);
    }

    void Menu::createLoginElements()
    {
        element_t background = {};
        element_t title = {};
        element_t usernameText = {};
        element_t passwordText = {};

        this->_elements.clear();

        background._type = IMAGE;
        background._image_path = "assets/menu.jpg";
        background._position = std::make_tuple(0, 0);
        background._size = std::make_tuple(600, 800);
        this->_elements.push_back(background);

        title._type = TEXT;
        title._text = "Please login and press ENTER";
        title._position = std::make_tuple(25, 20);
        title._color = "0";
        title._font_size = 42;
        this->_elements.push_back(title);

        usernameText._type = TEXT;
        usernameText._text = "Username: " + this->_username;
        usernameText._position = std::make_tuple(40, 45);
        usernameText._color = this->_selectedButton == 0 ? "2" : "0";
        usernameText._font_size = 28;
        this->_elements.push_back(usernameText);

        passwordText._type = TEXT;
        passwordText._text = "Password: " + std::string(this->_password.length(), '*');
        passwordText._position = std::make_tuple(40, 55);
        passwordText._color = this->_selectedButton == 1 ? "2" : "0";
        passwordText._font_size = 28;
        this->_elements.push_back(passwordText);
    }

    void Menu::createGameSelectElements()
    {
        element_t background = {};
        element_t WelcomeText = {};
        element_t title = {};
        element_t snakeText = {};
        element_t nibblerText = {};

        this->_elements.clear();

        background._type = IMAGE;
        background._image_path = "assets/menu.jpg";
        background._position = std::make_tuple(0, 0);
        background._size = std::make_tuple(600, 800);
        this->_elements.push_back(background);

        WelcomeText._type = TEXT;
        WelcomeText._text = "Welcome " + this->_username + "!";
        WelcomeText._position = std::make_tuple(25, 10);
        WelcomeText._color = "0";
        WelcomeText._font_size = 72;
        this->_elements.push_back(WelcomeText);

        title._type = TEXT;
        title._text = "Select a game:";
        title._position = std::make_tuple(25, 20);
        title._color = "0";
        title._font_size = 72;
        this->_elements.push_back(title);

        snakeText._type = TEXT;
        snakeText._text = "Snake";
        snakeText._position = std::make_tuple(45, 45);
        snakeText._color = this->_selectedButton == 0 ? "2" : "0";
        snakeText._font_size = 40;
        this->_elements.push_back(snakeText);

        nibblerText._type = TEXT;
        nibblerText._text = "Nibbler";
        nibblerText._position = std::make_tuple(45, 55);
        nibblerText._color = this->_selectedButton == 1 ? "2" : "0";
        nibblerText._font_size = 40;
        this->_elements.push_back(nibblerText);
    }

    void Menu::handleLoginInput(const std::string &input)
    {
        if (input == "ENTER") {
            if (this->_username == "admin" && this->_password == "admin") {
                this->_authenticated = true;
                this->_state = GAME_SELECT;
                this->_selectedButton = 0;
                createGameSelectElements();
            }
        } else if (input == "TAB") {
            this->_selectedButton = (this->_selectedButton + 1) % 2;
            createLoginElements();
        } else if (input == "BACKSPACE") {
            if (this->_selectedButton == 0 && !this->_username.empty())
                this->_username.pop_back();
            else if (this->_selectedButton == 1 && !this->_password.empty())
                this->_password.pop_back();
            createLoginElements();
        } else if (!input.empty() && input.length() == 1) {
            if (this->_selectedButton == 0 && this->_username.length() < 16)
                this->_username += input;
            else if (this->_selectedButton == 1 && this->_password.length() < 16)
                this->_password += input;
            createLoginElements();
        }
    }

    void Menu::handleGameSelectInput(const std::string &input)
    {
        if (input == "ENTER") {
            this->_selectedGame = this->_selectedButton == 0 ? "snake" : "nibbler";
        } else if (input == "TAB") {
            this->_selectedButton = (this->_selectedButton + 1) % 2;
            createGameSelectElements();
        }
    }
}