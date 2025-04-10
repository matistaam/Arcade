/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Core
*/

#include "Includes.hpp"

namespace arc {
    Core::Core(std::string path) : _menu(), _currentLibIndex(0), _dlLoader(), _inGame(false), _isPaused(false)
    {
        std::string libName = path;
        size_t lastSlash = libName.find_last_of('/');

        this->_availableGraphicalLibs = this->_dlLoader.getAvailableGraphicalLibs();
        this->_availableGames = this->_dlLoader.getAvailableGames();
        this->_menu.setAvailableGames(this->_availableGames);
        try {
            this->_graphical = this->_dlLoader.loadGraphicalLibrary(path);
        } catch (const std::exception &e) {
            throw;
        }
        this->_game = nullptr;
        if (lastSlash != std::string::npos)
            libName = libName.substr(lastSlash + 1);
        if (libName.substr(0, 7) == "arcade_" && libName.substr(libName.length() - 3) == ".so") {
            libName = libName.substr(7, libName.length() - 10);
            for (size_t i = 0; i < this->_availableGraphicalLibs.size(); i++) {
                if (this->_availableGraphicalLibs[i] == libName) {
                    this->_currentLibIndex = i;
                    break;
                }
            }
        }
    }

    Core::~Core()
    {
        if (this->_graphical) {
            this->_dlLoader.unloadGraphicalLibrary(this->_graphical);
            this->_graphical.reset();
        }
        if (this->_game) {
            this->_dlLoader.unloadGame(this->_game);
            this->_game.reset();
        }
    }

    void Core::setGraphical(std::shared_ptr<IGraphical> Graphical)
    {
        if (this->_graphical)
            this->_dlLoader.unloadGraphicalLibrary(this->_graphical);
        this->_graphical = Graphical;
    }

    void Core::setGame(std::shared_ptr<IGame> Game)
    {
        std::vector<element_t> initialElements = {};

        this->_game = Game;
        this->_inGame = (Game != nullptr);
        if (this->_game && this->_inGame) {
            initialElements = this->_game->handleEvents("");
            this->display(initialElements);
        }
    }

    void Core::display(std::vector<element_t> elements)
    {
        if (!this->_graphical)
            return;
        this->_graphical->clearElements();
        if (!this->_inGame || this->_isPaused)
            this->_graphical->setElements(this->_menu.getElements());
        else
            this->_graphical->setElements(elements);
        this->_graphical->draw();
    }

    std::string Core::update()
    {
        std::string event = "";
        std::vector<element_t> gameElements = {};

        if (!this->_graphical)
            return ("EXIT");
        event = this->_graphical->getEvents();
        if (event == "EXIT")
            return (event);
        else if (event == "SWITCH_LIB") {
            if (!this->_availableGraphicalLibs.empty()) {
                this->_currentLibIndex = (this->_currentLibIndex + 1) % this->_availableGraphicalLibs.size();
                switchGraphicalLibrary(this->_availableGraphicalLibs[this->_currentLibIndex]);
            }
        }
        if (!this->_inGame) {
            this->_menu.handleInput(event);
            display(this->_menu.getElements());
            if (this->_menu.isAuthenticated() && !this->_menu.getSelectedGame().empty()) {
                try {
                    loadGame(this->_menu.getSelectedGame());
                    if (this->_game) {
                        gameElements = this->_game->handleEvents("");
                        display(gameElements);
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    this->_menu.handleInput("ESCAPE");
                }
            }
        } else {
            if (event == "m") {
                this->_isPaused = true;
                this->_menu.handleInput(event);
                display(this->_menu.getElements());
            }
            if (this->_isPaused) {
                this->_menu.handleInput(event);
                display(this->_menu.getElements());
                if (this->_menu.shouldResume()) {
                    this->_isPaused = false;
                    if (this->_game) {
                        gameElements = this->_game->handleEvents("");
                        display(gameElements);
                    }
                } else if (this->_menu.shouldQuit()) {
                    return ("EXIT");
                } else if (this->_menu.shouldReturnToMenu()) {
                    this->_isPaused = false;
                    this->_inGame = false;
                }
            } else if (this->_game) {
                gameElements = this->_game->handleEvents(event);
                display(gameElements);
            }
        }
        return ("");
    }

    void Core::loadGame(const std::string &name)
    {
        std::string username = this->_menu.getUsername();
        int highScore = 0;

        if (this->_game) {
            this->_dlLoader.unloadGame(this->_game);
            this->_game = nullptr;
        }
        std::ifstream file("accounts.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::string currentUser = line.substr(0, line.find(':'));
                if (currentUser == username) {
                    size_t pos = line.find(name + "=");
                    if (pos != std::string::npos) {
                        pos += name.length() + 1;
                        size_t endPos = line.find(':', pos);
                        if (endPos == std::string::npos)
                            endPos = line.length();
                        highScore = std::stoi(line.substr(pos, endPos - pos));
                    }
                    break;
                }
            }
            file.close();
        }
        try {
            this->_game = this->_dlLoader.loadGame(name, username, highScore);
            this->_inGame = true;
        } catch (const std::exception &e) {
            throw;
        }
    }

    void Core::switchGraphicalLibrary(const std::string &name)
    {
        std::vector<element_t> gameElements = {};
        std::string lib_path = "lib/arcade_" + name + ".so";

        if (this->_graphical) {
            this->_dlLoader.unloadGraphicalLibrary(this->_graphical);
            this->_graphical = nullptr;
        }
        if (name.empty())
            return;
        try {
            this->_graphical = this->_dlLoader.loadGraphicalLibrary(lib_path);
        } catch (const std::exception &e) {
            throw;
        }
        for (size_t i = 0; i < this->_availableGraphicalLibs.size(); i++) {
            if (this->_availableGraphicalLibs[i] == name) {
                this->_currentLibIndex = i;
                break;
            }
        }
        if (this->_game && this->_inGame && !this->_isPaused) {
            gameElements = this->_game->handleEvents("");
            display(gameElements);
        } else if (!this->_inGame || this->_isPaused) {
            display(this->_menu.getElements());
        }
    }

    std::vector<std::string> Core::getAvailableGames()
    {
        this->_availableGames = this->_dlLoader.getAvailableGames();
        return (this->_availableGames);
    }

    std::vector<std::string> Core::getAvailableGraphicalLibs()
    {
        this->_availableGraphicalLibs = this->_dlLoader.getAvailableGraphicalLibs();
        return (this->_availableGraphicalLibs);
    }
}
