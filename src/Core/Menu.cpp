/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Menu
*/

#include "Includes.hpp"
#include <fstream>
#include <sstream>

namespace arc {
    Menu::Menu() : _state(LOGIN_SIGNUP), _authenticated(false), _selectedButton(0), _resume(false), _quit(false), _returnToMenu(false)
    {
        createLoginSignUpElements();
    }

    Menu::~Menu()
    {
    }

    void Menu::setAvailableGames(const std::vector<std::string> &games)
    {
        this->_availableGames = games;
    }

    std::vector<element_t> Menu::getElements() const
    {
        return (this->_elements);
    }

    void Menu::handleInput(const std::string &input)
    {
        if (input == "m" && this->_authenticated && !this->_selectedGame.empty()) {
            this->_state = PAUSE;
            this->_selectedButton = 0;
            createPauseElements();
            return;
        }
        if (this->_state == LOGIN_SIGNUP)
            handleLoginSignUpInput(input);
        else if (this->_state == LOGIN)
            handleLoginInput(input);
        else if (this->_state == SIGN_UP)
            handleSignUpInput(input);
        else if (this->_state == PAUSE)
            handlePauseInput(input);
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

    bool Menu::shouldResume() const
    {
        return (this->_resume);
    }

    bool Menu::shouldQuit() const
    {
        return (this->_quit);
    }

    bool Menu::shouldReturnToMenu() const
    {
        return (this->_returnToMenu);
    }

    bool Menu::saveAccount(const std::string &username, const std::string &password)
    {
        std::ofstream accountFile("accounts.txt", std::ios::app);

        if (!accountFile.is_open())
            return (false);
        accountFile << username << ":" << password << std::endl;
        accountFile.close();
        return (true);
    }

    bool Menu::verifyAccount(const std::string &username, const std::string &password)
    {
        std::ifstream accountFile("accounts.txt");
        std::string line = "";
        size_t delimiterPos = 0;
        std::string storedUsername = "";
        std::string storedPassword = "";

        if (!accountFile.is_open())
            return (false);
        while (std::getline(accountFile, line)) {
            delimiterPos = line.find(":");
            if (delimiterPos != std::string::npos) {
                storedUsername = line.substr(0, delimiterPos);
                storedPassword = line.substr(delimiterPos + 1);
                if (storedUsername == username && storedPassword == password) {
                    accountFile.close();
                    return (true);
                }
            }
        }
        accountFile.close();
        return (false);
    }

    void Menu::createLoginSignUpElements()
    {
        element_t background = {};
        element_t title = {};
        element_t login = {};
        element_t signup = {};

        this->_elements.clear();

        background._type = IMAGE;
        background._image_path = "assets/menu.jpg";
        background._position = std::make_tuple(0, 0);
        background._size = std::make_tuple(600, 800);
        this->_elements.push_back(background);

        title._type = TEXT;
        title._text = "Please login or sign up and press ENTER";
        title._position = std::make_tuple(50, 20);
        title._color = "0";
        title._font_size = 42;
        this->_elements.push_back(title);

        login._type = TEXT;
        login._text = "Log In";
        login._position = std::make_tuple(50, 45);
        login._color = this->_selectedButton == 0 ? "2" : "0";
        login._font_size = 28;
        this->_elements.push_back(login);

        signup._type = TEXT;
        signup._text = "Sign Up";
        signup._position = std::make_tuple(50, 55);
        signup._color = this->_selectedButton == 1 ? "2" : "0";
        signup._font_size = 28;
        this->_elements.push_back(signup);
    }

    void Menu::createLoginElements()
    {
        element_t background = {};
        element_t title = {};
        element_t usernameText = {};
        element_t passwordText = {};
        element_t returnText = {};

        this->_elements.clear();

        background._type = IMAGE;
        background._image_path = "assets/menu.jpg";
        background._position = std::make_tuple(0, 0);
        background._size = std::make_tuple(600, 800);
        this->_elements.push_back(background);

        title._type = TEXT;
        title._text = "Please login and press ENTER";
        title._position = std::make_tuple(50, 20);
        title._color = "0";
        title._font_size = 42;
        this->_elements.push_back(title);

        usernameText._type = TEXT;
        usernameText._text = "Username: " + this->_username;
        usernameText._position = std::make_tuple(50, 45);
        usernameText._color = this->_selectedButton == 0 ? "2" : "0";
        usernameText._font_size = 28;
        this->_elements.push_back(usernameText);

        passwordText._type = TEXT;
        passwordText._text = "Password: " + std::string(this->_password.length(), '*');
        passwordText._position = std::make_tuple(50, 55);
        passwordText._color = this->_selectedButton == 1 ? "2" : "0";
        passwordText._font_size = 28;
        this->_elements.push_back(passwordText);

        returnText._type = TEXT;
        returnText._text = "Return";
        returnText._position = std::make_tuple(50, 65);
        returnText._color = this->_selectedButton == 2 ? "2" : "0";
        returnText._font_size = 28;
        this->_elements.push_back(returnText);
    }

    void Menu::createSignUpElements()
    {
        element_t background = {};
        element_t title = {};
        element_t usernameText = {};
        element_t passwordText = {};
        element_t returnText = {};

        this->_elements.clear();

        background._type = IMAGE;
        background._image_path = "assets/menu.jpg";
        background._position = std::make_tuple(0, 0);
        background._size = std::make_tuple(600, 800);
        this->_elements.push_back(background);

        title._type = TEXT;
        title._text = "Please sign up and press ENTER";
        title._position = std::make_tuple(50, 20);
        title._color = "0";
        title._font_size = 42;
        this->_elements.push_back(title);

        usernameText._type = TEXT;
        usernameText._text = "Username: " + this->_username;
        usernameText._position = std::make_tuple(50, 45);
        usernameText._color = this->_selectedButton == 0 ? "2" : "0";
        usernameText._font_size = 28;
        this->_elements.push_back(usernameText);

        passwordText._type = TEXT;
        passwordText._text = "Password: " + std::string(this->_password.length(), '*');
        passwordText._position = std::make_tuple(50, 55);
        passwordText._color = this->_selectedButton == 1 ? "2" : "0";
        passwordText._font_size = 28;
        this->_elements.push_back(passwordText);

        returnText._type = TEXT;
        returnText._text = "Return";
        returnText._position = std::make_tuple(50, 65);
        returnText._color = this->_selectedButton == 2 ? "2" : "0";
        returnText._font_size = 28;
        this->_elements.push_back(returnText);
    }

    void Menu::createGameSelectElements()
    {
        element_t background = {};
        element_t welcomeText = {};
        element_t title = {};
        element_t gameText = {};
        float yPosition = 30;

        this->_elements.clear();

        background._type = IMAGE;
        background._image_path = "assets/menu.jpg";
        background._position = std::make_tuple(0, 0);
        background._size = std::make_tuple(600, 800);
        this->_elements.push_back(background);

        welcomeText._type = TEXT;
        welcomeText._text = "Welcome " + this->_username + "!";
        welcomeText._position = std::make_tuple(50, 10);
        welcomeText._color = "0";
        welcomeText._font_size = 72;
        this->_elements.push_back(welcomeText);

        title._type = TEXT;
        title._text = "Select a game:";
        title._position = std::make_tuple(50, 20);
        title._color = "0";
        title._font_size = 72;
        this->_elements.push_back(title);

        for (size_t i = 0; i < this->_availableGames.size(); i++) {
            gameText._type = TEXT;
            gameText._text = this->_availableGames[i];
            gameText._position = std::make_tuple(50, yPosition);
            gameText._color = this->_selectedButton == i ? "2" : "0";
            gameText._font_size = 40;
            this->_elements.push_back(gameText);
            yPosition += 10.0f;
        }
    }

    void Menu::createPauseElements()
    {
        element_t background = {};
        element_t title = {};
        element_t resumeText = {};
        element_t menuText = {};
        element_t quitText = {};

        this->_elements.clear();
        this->_resume = false;
        this->_quit = false;
        this->_returnToMenu = false;

        background._type = IMAGE;
        background._image_path = "assets/pause_menu.jpg";
        background._position = std::make_tuple(0, 0);
        background._size = std::make_tuple(600, 800);
        this->_elements.push_back(background);

        title._type = TEXT;
        title._text = "PAUSE";
        title._position = std::make_tuple(45, 20);
        title._color = "0";
        title._font_size = 72;
        this->_elements.push_back(title);

        resumeText._type = TEXT;
        resumeText._text = "Resume";
        resumeText._position = std::make_tuple(45, 45);
        resumeText._color = this->_selectedButton == 0 ? "2" : "0";
        resumeText._font_size = 40;
        this->_elements.push_back(resumeText);

        menuText._type = TEXT;
        menuText._text = "Return to Menu";
        menuText._position = std::make_tuple(45, 55);
        menuText._color = this->_selectedButton == 1 ? "2" : "0";
        menuText._font_size = 40;
        this->_elements.push_back(menuText);

        quitText._type = TEXT;
        quitText._text = "Quit";
        quitText._position = std::make_tuple(45, 65);
        quitText._color = this->_selectedButton == 2 ? "2" : "0";
        quitText._font_size = 40;
        this->_elements.push_back(quitText);
    }

    void Menu::handleLoginSignUpInput(const std::string &input)
    {
        if (input == "ENTER") {
            if (this->_selectedButton == 0) {
                this->_state = LOGIN;
                this->_selectedButton = 0;
                createLoginElements();
            } else {
                this->_state = SIGN_UP;
                this->_selectedButton = 0;
                createSignUpElements();
            }
        } else if (input == "TAB") {
            this->_selectedButton = (this->_selectedButton + 1) % 2;
            createLoginSignUpElements();
        }
    }

    void Menu::handleLoginInput(const std::string &input)
    {
        if (input == "ENTER") {
            if (this->_selectedButton == 2) {
                this->_state = LOGIN_SIGNUP;
                this->_selectedButton = 0;
                this->_username.clear();
                this->_password.clear();
                createLoginSignUpElements();
            } else if (!this->_username.empty() && !this->_password.empty()) {
                if (verifyAccount(this->_username, this->_password)) {
                    this->_authenticated = true;
                    this->_state = GAME_SELECT;
                    this->_selectedButton = 0;
                    createGameSelectElements();
                }
            }
        } else if (input == "TAB") {
            this->_selectedButton = (this->_selectedButton + 1) % 3;
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

    void Menu::handleSignUpInput(const std::string &input)
    {
        if (input == "ENTER") {
            if (this->_selectedButton == 2) {
                this->_state = LOGIN_SIGNUP;
                this->_selectedButton = 0;
                this->_username.clear();
                this->_password.clear();
                createLoginSignUpElements();
            } else if (!this->_username.empty() && !this->_password.empty()) {
                if (saveAccount(this->_username, this->_password)) {
                    this->_authenticated = true;
                    this->_state = GAME_SELECT;
                    this->_selectedButton = 0;
                    createGameSelectElements();
                }
            }
        } else if (input == "TAB") {
            this->_selectedButton = (this->_selectedButton + 1) % 3;
            createSignUpElements();
        } else if (input == "BACKSPACE") {
            if (this->_selectedButton == 0 && !this->_username.empty())
                this->_username.pop_back();
            else if (this->_selectedButton == 1 && !this->_password.empty())
                this->_password.pop_back();
            createSignUpElements();
        } else if (!input.empty() && input.length() == 1) {
            if (this->_selectedButton == 0 && this->_username.length() < 16)
                this->_username += input;
            else if (this->_selectedButton == 1 && this->_password.length() < 16)
                this->_password += input;
            createSignUpElements();
        }
    }

    void Menu::handleGameSelectInput(const std::string &input)
    {
        if (input == "ENTER" && !this->_availableGames.empty()) {
            this->_selectedGame = this->_availableGames[this->_selectedButton];
        } else if (input == "TAB" && !this->_availableGames.empty()) {
            this->_selectedButton = (this->_selectedButton + 1) % this->_availableGames.size();
            createGameSelectElements();
        }
    }

    void Menu::handlePauseInput(const std::string &input)
    {
        if (input == "ENTER") {
            switch (this->_selectedButton) {
                case 0:
                    this->_resume = true;
                    this->_state = GAME_SELECT;
                    break;
                case 1:
                    this->_returnToMenu = true;
                    this->_state = GAME_SELECT;
                    this->_selectedButton = 0;
                    this->_selectedGame = "";
                    createGameSelectElements();
                    break;
                case 2:
                    this->_quit = true;
                    break;
            }
        } else if (input == "TAB") {
            this->_selectedButton = (this->_selectedButton + 1) % 3;
            createPauseElements();
        }
    }
}
