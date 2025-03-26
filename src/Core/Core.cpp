/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Core
*/

#include "Includes.hpp"

namespace arc {
    typedef IGraphical* (*create_graphical_t)();
    typedef void (*destroy_graphical_t)(IGraphical*);
    typedef IGame* (*create_game_t)(const std::string &, int);
    typedef void (*destroy_game_t)(IGame*);
    typedef const char* (*get_type_t)();

    Core::Core(std::string path) : _menu(), _currentLibIndex(0), _handle(nullptr), _gameHandle(nullptr), _inGame(false), _isPaused(false)
    {
        create_graphical_t create = nullptr;
        get_type_t get_type = nullptr;
        std::string libName = path;
        size_t lastSlash = libName.find_last_of('/');

        this->_availableGraphicalLibs = getAvailableGraphicalLibs();
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
        destroy_graphical_t destroy = nullptr;
        destroy_game_t destroy_game = nullptr;

        if (this->_graphical) {
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

    void Core::setGraphical(IGraphical *Graphical)
    {
        destroy_graphical_t destroy = nullptr;

        if (this->_graphical) {
            destroy = (destroy_graphical_t)dlsym(this->_handle, "destroy");
            if (destroy)
                destroy(this->_graphical);
        }
        this->_graphical = Graphical;
    }

    void Core::setGame(IGame *Game)
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
        if (!this->_inGame || this->_isPaused) {
            this->_graphical->setElements(this->_menu.getElements());
        } else {
            this->_graphical->setElements(elements);
        }
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
        create_game_t create = nullptr;
        destroy_game_t destroy_game = nullptr;
        std::string lib_path = "lib/arcade_" + name + ".so";
        std::string username = this->_menu.getUsername();
        int highScore = 0;

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
        
        // Open accounts.txt file to read the high score
        std::ifstream file("accounts.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::string currentUser = line.substr(0, line.find(':'));
                if (currentUser == username) {
                    // Parse the line to find the high score for the selected game
                    size_t pos = line.find(name + "=");
                    if (pos != std::string::npos) {
                        pos += name.length() + 1; // Skip past the game name and =
                        size_t endPos = line.find(':', pos);
                        if (endPos == std::string::npos)
                            endPos = line.length();
                        highScore = std::stoi(line.substr(pos, endPos - pos));
                        std::cout << "High score for " << name << ": " << highScore << std::endl;
                    }
                    break;
                }
            }
            file.close();
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
        this->_game = create(username, highScore);
        if (!this->_game) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
            throw GameError(std::string("Failed to create game instance from '") + lib_path + "'");
        }
        
        this->_inGame = true;
    }

    void Core::switchGraphicalLibrary(const std::string &name)
    {
        create_graphical_t create = nullptr;
        destroy_graphical_t destroy = nullptr;
        get_type_t get_type = nullptr;
        void *newHandle = nullptr;
        IGraphical *newGraphical = nullptr;
        std::string lib_path = "lib/arcade_" + name + ".so";
        std::vector<element_t> gameElements = {};

        if (this->_graphical) {
            destroy = (destroy_graphical_t)dlsym(this->_handle, "destroy");
            if (destroy)
                destroy(this->_graphical);
        }
        if (this->_handle)
            dlclose(this->_handle);
        if (name.empty())
            return;
        newHandle = dlopen(lib_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!newHandle)
            throw GraphicalError(std::string("Cannot load graphical library '") + lib_path + "': " + std::string(dlerror()));
        get_type = (get_type_t)dlsym(newHandle, "get_type");
        if (!get_type) {
            dlclose(newHandle);
            throw GraphicalError(std::string("Invalid graphical library '") + lib_path + "'");
        }
        if (std::string(get_type()) != "graphical") {
            dlclose(newHandle);
            throw GraphicalError(lib_path + ": not a graphical library");
        }
        create = (create_graphical_t)dlsym(newHandle, "create");
        if (!create) {
            dlclose(newHandle);
            throw GraphicalError(std::string("Invalid graphical library '") + lib_path + "'");
        }
        newGraphical = create();
        if (!newGraphical) {
            dlclose(newHandle);
            throw GraphicalError(std::string("Failed to create graphical instance from '") + lib_path + "'");
        }
        this->_handle = newHandle;
        this->_graphical = newGraphical;
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

    std::vector<std::string> Core::getAvailableGraphicalLibs()
    {
        DIR *dir = nullptr;
        struct dirent *entry = nullptr;
        std::string filename = "";
        void *handle = nullptr;
        get_type_t get_type = nullptr;

        this->_availableGraphicalLibs.clear();
        dir = opendir("lib");
        if (dir == nullptr)
            return (this->_availableGraphicalLibs);
        while ((entry = readdir(dir)) != nullptr) {
            filename = entry->d_name;
            if ((filename.substr(0, 7) == "arcade_") && (filename.substr(filename.length() - 3) == ".so")) {
                handle = dlopen(("lib/" + filename).c_str(), RTLD_LAZY | RTLD_GLOBAL);
                if (handle) {
                    get_type = (get_type_t)dlsym(handle, "get_type");
                    if (get_type && std::string(get_type()) == "graphical") {
                        this->_availableGraphicalLibs.push_back(filename.substr(7, filename.length() - 10));
                    }
                    dlclose(handle);
                }
            }
        }
        closedir(dir);
        return (this->_availableGraphicalLibs);
    }
}
