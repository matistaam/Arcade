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

    DlLoader::DlLoader() : _handle(nullptr), _gameHandle(nullptr), _graphical(nullptr), _game(nullptr)
    {
    }

    DlLoader::~DlLoader()
    {
        closeGraphicalLibrary();
        closeGameLibrary();
    }

    IGraphical *DlLoader::loadGraphicalLibrary(const std::string &path)
    {
        create_graphical_t create = nullptr;
        get_type_t get_type = nullptr;

        closeGraphicalLibrary();
        
        this->_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!this->_handle)
            throw LibraryError(std::string(path) + ": " + std::string(dlerror()));
        
        get_type = (get_type_t)dlsym(this->_handle, "get_type");
        if (!get_type) {
            dlclose(this->_handle);
            this->_handle = nullptr;
            throw InvalidLibraryError(path);
        }
        
        if (std::string(get_type()) != "graphical") {
            dlclose(this->_handle);
            this->_handle = nullptr;
            throw InvalidLibraryError(path + ": not a graphical library");
        }
        
        create = (create_graphical_t)dlsym(this->_handle, "create");
        if (!create) {
            dlclose(this->_handle);
            this->_handle = nullptr;
            throw InvalidLibraryError(path);
        }
        
        this->_graphical = create();
        if (!this->_graphical) {
            dlclose(this->_handle);
            this->_handle = nullptr;
            throw InvalidLibraryError(path);
        }
        
        return this->_graphical;
    }

    void DlLoader::switchGraphicalLibrary(const std::string &name)
    {
        std::string lib_path = "lib/arcade_" + name + ".so";
        loadGraphicalLibrary(lib_path);
    }

    void DlLoader::closeGraphicalLibrary()
    {
        destroy_graphical_t destroy = nullptr;

        if (this->_graphical && this->_handle) {
            destroy = (destroy_graphical_t)dlsym(this->_handle, "destroy");
            if (destroy)
                destroy(this->_graphical);
            this->_graphical = nullptr;
        }
        
        if (this->_handle) {
            dlclose(this->_handle);
            this->_handle = nullptr;
        }
    }

    IGame *DlLoader::loadGame(const std::string &name, const std::string &username, int highScore)
    {
        create_game_t create = nullptr;
        std::string lib_path = "lib/arcade_" + name + ".so";

        closeGameLibrary();

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
        
        return this->_game;
    }

    void DlLoader::closeGameLibrary()
    {
        destroy_game_t destroy_game = nullptr;

        if (this->_game && this->_gameHandle) {
            destroy_game = (destroy_game_t)dlsym(this->_gameHandle, "destroy");
            if (destroy_game)
                destroy_game(this->_game);
            this->_game = nullptr;
        }
        
        if (this->_gameHandle) {
            dlclose(this->_gameHandle);
            this->_gameHandle = nullptr;
        }
    }

    std::vector<std::string> DlLoader::getAvailableGames()
    {
        DIR *dir = nullptr;
        struct dirent *entry = nullptr;
        std::string filename = "";
        void *handle = nullptr;
        get_type_t get_type = nullptr;
        std::vector<std::string> availableGames;

        dir = opendir("lib");
        if (dir == nullptr)
            return availableGames;
            
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
        return availableGames;
    }

    std::vector<std::string> DlLoader::getAvailableGraphicalLibs()
    {
        DIR *dir = nullptr;
        struct dirent *entry = nullptr;
        std::string filename = "";
        void *handle = nullptr;
        get_type_t get_type = nullptr;
        std::vector<std::string> availableGraphicalLibs;

        dir = opendir("lib");
        if (dir == nullptr)
            return availableGraphicalLibs;
            
        while ((entry = readdir(dir)) != nullptr) {
            filename = entry->d_name;
            if ((filename.substr(0, 7) == "arcade_") && (filename.substr(filename.length() - 3) == ".so")) {
                handle = dlopen(("lib/" + filename).c_str(), RTLD_LAZY | RTLD_GLOBAL);
                if (handle) {
                    get_type = (get_type_t)dlsym(handle, "get_type");
                    if (get_type && std::string(get_type()) == "graphical") {
                        availableGraphicalLibs.push_back(filename.substr(7, filename.length() - 10));
                    }
                    dlclose(handle);
                }
            }
        }
        closedir(dir);
        return availableGraphicalLibs;
    }
}