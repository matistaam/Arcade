/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** DlLoader
*/

#include "Includes.hpp"

namespace arc {
    typedef IGraphical* (*create_graphical_t)();
    typedef void (*destroy_graphical_t)(IGraphical*);
    typedef IGame* (*create_game_t)(const std::string &, int);
    typedef void (*destroy_game_t)(IGame*);
    typedef const char* (*get_type_t)();

    DlLoader::DlLoader() : _graphicalHandle(nullptr), _gameHandle(nullptr)
    {
    }

    DlLoader::~DlLoader()
    {
        if (this->_graphicalHandle)
            dlclose(this->_graphicalHandle);
        if (this->_gameHandle)
            dlclose(this->_gameHandle);
    }

    std::shared_ptr<IGraphical> DlLoader::loadGraphicalLibrary(const std::string &path)
    {
        create_graphical_t create = nullptr;
        get_type_t get_type = nullptr;
        IGraphical *graphical_raw = nullptr;
        std::shared_ptr<IGraphical> graphical = nullptr;

        if (this->_graphicalHandle) {
            dlclose(this->_graphicalHandle);
            this->_graphicalHandle = nullptr;
        }
        this->_graphicalHandle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!this->_graphicalHandle)
            throw LibraryError(std::string(path) + ": " + std::string(dlerror()));
        get_type = (get_type_t)dlsym(this->_graphicalHandle, "get_type");
        if (!get_type) {
            dlclose(this->_graphicalHandle);
            this->_graphicalHandle = nullptr;
            throw InvalidLibraryError(path);
        }
        if (std::string(get_type()) != "graphical") {
            dlclose(this->_graphicalHandle);
            this->_graphicalHandle = nullptr;
            throw InvalidLibraryError(path + ": not a graphical library");
        }
        create = (create_graphical_t)dlsym(this->_graphicalHandle, "create");
        if (!create) {
            dlclose(this->_graphicalHandle);
            this->_graphicalHandle = nullptr;
            throw InvalidLibraryError(path);
        }
        graphical_raw = create();
        if (!graphical_raw) {
            dlclose(this->_graphicalHandle);
            this->_graphicalHandle = nullptr;
            throw InvalidLibraryError(path);
        }
        auto deleter = [](IGraphical *) {};
        graphical = std::shared_ptr<IGraphical>(graphical_raw, deleter);
        return (graphical);
    }

    void DlLoader::unloadGraphicalLibrary(std::shared_ptr<IGraphical> graphical)
    {
        destroy_graphical_t destroy = nullptr;

        if (graphical && this->_graphicalHandle) {
            destroy = (destroy_graphical_t)dlsym(this->_graphicalHandle, "destroy");
            if (destroy)
                destroy(graphical.get());
        }
        if (this->_graphicalHandle) {
            dlclose(this->_graphicalHandle);
            this->_graphicalHandle = nullptr;
        }
    }

    std::shared_ptr<IGame> DlLoader::loadGame(const std::string &name, const std::string &username, int highScore)
    {
        create_game_t create = nullptr;
        get_type_t get_type = nullptr;
        IGame *game_raw = nullptr;
        std::shared_ptr<IGame> game = nullptr;
        std::string lib_path = "lib/arcade_" + name + ".so";

        if (this->_gameHandle) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
        }
        this->_gameHandle = dlopen(lib_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!this->_gameHandle)
            throw GameError(std::string("Cannot load game library '") + lib_path + "': " + std::string(dlerror()));
        get_type = (get_type_t)dlsym(this->_gameHandle, "get_type");
        if (!get_type || std::string(get_type()) != "game") {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
            throw GameError(std::string("Invalid game library '") + lib_path + "'");
        }
        create = (create_game_t)dlsym(this->_gameHandle, "create");
        if (!create) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
            throw GameError(std::string("Invalid game library '") + lib_path + "'");
        }
        game_raw = create(username, highScore);
        if (!game_raw) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
            throw GameError(std::string("Failed to create game instance from '") + lib_path + "'");
        }
        auto deleter = [](IGame *) {};
        game = std::shared_ptr<IGame>(game_raw, deleter);
        return (game);
    }

    void DlLoader::unloadGame(std::shared_ptr<IGame> game)
    {
        destroy_game_t destroy = nullptr;

        if (game && this->_gameHandle) {
            destroy = (destroy_game_t)dlsym(this->_gameHandle, "destroy");
            if (destroy)
                destroy(game.get());
        }
        if (this->_gameHandle) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
        }
    }

    std::vector<std::string> DlLoader::getAvailableGraphicalLibs()
    {
        std::vector<std::string> availableGraphicalLibs;
        DIR *dir = nullptr;
        struct dirent *entry = nullptr;
        std::string filename = "";
        void *handle = nullptr;
        get_type_t get_type = nullptr;

        dir = opendir("lib");
        if (dir == nullptr)
            return (availableGraphicalLibs);
        while ((entry = readdir(dir)) != nullptr) {
            filename = entry->d_name;
            if ((filename.substr(0, 7) == "arcade_") && (filename.substr(filename.length() - 3) == ".so")) {
                handle = dlopen(("lib/" + filename).c_str(), RTLD_LAZY | RTLD_GLOBAL);
                if (handle) {
                    get_type = (get_type_t)dlsym(handle, "get_type");
                    if (get_type && std::string(get_type()) == "graphical")
                        availableGraphicalLibs.push_back(filename.substr(7, filename.length() - 10));
                    dlclose(handle);
                }
            }
        }
        closedir(dir);
        return (availableGraphicalLibs);
    }

    std::vector<std::string> DlLoader::getAvailableGames()
    {
        std::vector<std::string> availableGames;
        DIR *dir = nullptr;
        struct dirent *entry = nullptr;
        std::string filename = "";
        void *handle = nullptr;
        get_type_t get_type = nullptr;

        dir = opendir("lib");
        if (dir == nullptr)
            return (availableGames);
        while ((entry = readdir(dir)) != nullptr) {
            filename = entry->d_name;
            if ((filename.substr(0, 7) == "arcade_") && (filename.substr(filename.length() - 3) == ".so")) {
                handle = dlopen(("lib/" + filename).c_str(), RTLD_LAZY | RTLD_GLOBAL);
                if (handle) {
                    get_type = (get_type_t)dlsym(handle, "get_type");
                    if (get_type && std::string(get_type()) == "game")
                        availableGames.push_back(filename.substr(7, filename.length() - 10));
                    dlclose(handle);
                }
            }
        }
        closedir(dir);
        return (availableGames);
    }
}
