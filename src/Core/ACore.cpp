/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** ACore
*/

#include "Includes.hpp"

namespace arc {
    typedef IGraphical* (*create_graphical_t)();
    typedef void (*destroy_graphical_t)(IGraphical*);
    typedef IGame* (*create_game_t)();
    typedef void (*destroy_game_t)(IGame*);
    typedef const char* (*get_type_t)();

    ACore::ACore(std::string path) : _handle(nullptr), _gameHandle(nullptr), _menu(), _inGame(false), _isPaused(false)
    {
        create_graphical_t create = nullptr;
        get_type_t get_type = nullptr;

        getAvailableGames();
        this->_menu.setAvailableGames(this->_availableGames);
        this->_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!this->_handle)
            throw LibraryError(std::string(path) + ": " + std::string(dlerror()));
        get_type = (get_type_t)dlsym(this->_handle, "get_type");
        if (!get_type) {
            dlclose(this->_handle);
            throw InvalidLibraryError(path);
        }
        if (std::string(get_type()) != "graphical") {
            dlclose(this->_handle);
            throw InvalidLibraryError(path + ": not a graphical library");
        }
        create = (create_graphical_t)dlsym(this->_handle, "create");
        if (!create) {
            dlclose(this->_handle);
            throw InvalidLibraryError(path);
        }
        this->_graphical = create();
        if (!this->_graphical) {
            dlclose(this->_handle);
            throw InvalidLibraryError(path);
        }
        this->_graphical->init();
        this->_game = nullptr;
    }

    ACore::~ACore()
    {
        destroy_graphical_t destroy = nullptr;
        destroy_game_t destroy_game = nullptr;

        if (this->_graphical) {
            this->_graphical->close();
            destroy = (destroy_graphical_t)dlsym(this->_handle, "destroy");
            if (destroy)
                destroy(this->_graphical);
        }
        if (this->_handle)
            dlclose(this->_handle);
        if (this->_game) {
            destroy_game = (destroy_game_t)dlsym(this->_gameHandle, "destroy");
            if (destroy_game)
                destroy_game(this->_game);
        }
        if (this->_gameHandle)
            dlclose(this->_gameHandle);
    }

    void ACore::setGraphical(IGraphical *Graphical)
    {
        destroy_graphical_t destroy = nullptr;

        if (this->_graphical) {
            this->_graphical->close();
            destroy = (destroy_graphical_t)dlsym(this->_handle, "destroy");
            if (destroy)
                destroy(this->_graphical);
        }
        this->_graphical = Graphical;
        if (this->_graphical)
            this->_graphical->init();
    }

    void ACore::loadGame(const std::string &name)
    {
        create_game_t create = nullptr;
        destroy_game_t destroy_game = nullptr;
        std::string lib_path = "lib/arcade_" + name + ".so";

        if (this->_game) {
            destroy_game = (destroy_game_t)dlsym(this->_gameHandle, "destroy");
            if (destroy_game)
                destroy_game(this->_game);
            this->_game = nullptr;
        }
        if (this->_gameHandle) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
        }
        this->_gameHandle = dlopen(lib_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!this->_gameHandle)
            throw GameError(std::string("Cannot load game library '") + lib_path + "': " + std::string(dlerror()));
        create = (create_game_t)dlsym(this->_gameHandle, "create");
        if (!create) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
            throw GameError(std::string("Invalid game library '") + lib_path + "'");
        }
        this->_game = create();
        if (!this->_game) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
            throw GameError(std::string("Failed to create game instance from '") + lib_path + "'");
        }
        this->_inGame = true;
    }

    void ACore::setGame(IGame *Game)
    {
        std::vector<element_t> initialElements = {};

        this->_game = Game;
        this->_inGame = (Game != nullptr);
        if (this->_game && this->_inGame) {
            initialElements = this->_game->handleEvents("");
            this->display(initialElements);
        }
    }

    void ACore::display(std::vector<element_t> elements)
    {
        if (!this->_graphical)
            return;
        this->_graphical->clearElements();
        if (!this->_inGame || this->_isPaused) {
            this->_graphical->addElements(this->_menu.getElements());
        } else {
            this->_graphical->addElements(elements);
        }
        this->_graphical->draw();
    }

    std::string ACore::update()
    {
        std::string event;
        std::vector<element_t> gameElements;

        if (!this->_graphical)
            return ("EXIT");
        event = this->_graphical->update();
        if (event == "EXIT")
            return (event);
        if (event == "PREV_LIB") {
            std::cout << "Previous library" << std::endl;
            // TODO: Implement library switching
        }
        if (event == "NEXT_LIB") {
            std::cout << "Next library" << std::endl;
            // TODO: Implement library switching
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

    std::vector<std::string> ACore::getAvailableGames()
    {
        DIR *dir = nullptr;
        struct dirent *entry = nullptr;
        std::string filename = "";
        void *handle = nullptr;
        get_type_t get_type = nullptr;

        this->_availableGames.clear();
        dir = opendir("lib");
        if (dir == nullptr)
            return (this->_availableGames);
        while ((entry = readdir(dir)) != nullptr) {
            filename = entry->d_name;
            if ((filename.substr(0, 7) == "arcade_") && (filename.substr(filename.length() - 3) == ".so")) {
                handle = dlopen(("lib/" + filename).c_str(), RTLD_LAZY | RTLD_GLOBAL);
                if (handle) {
                    get_type = (get_type_t)dlsym(handle, "get_type");
                    if (get_type && std::string(get_type()) == "game")
                        this->_availableGames.push_back(filename.substr(7, filename.length() - 10));
                    dlclose(handle);
                }
            }
        }
        closedir(dir);
        return (this->_availableGames);
    }
}
