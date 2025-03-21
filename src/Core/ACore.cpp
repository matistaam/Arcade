/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** ACore
*/

#include "Includes.hpp"

namespace arc {
    typedef IGraphical* (*create_t)();
    typedef void (*destroy_t)(IGraphical*);

    ACore::ACore(std::string path) : _inGame(false)
    {
        create_t create = nullptr;

        this->_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!this->_handle)
            throw LibraryError(std::string(path) + ": " + std::string(dlerror()));
        create = (create_t)dlsym(this->_handle, "create");
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
        destroy_t destroy = nullptr;

        if (this->_graphical) {
            this->_graphical->close();
            destroy = (destroy_t)dlsym(this->_handle, "destroy");
            if (destroy)
                destroy(this->_graphical);
        }
        if (this->_handle)
            dlclose(this->_handle);
    }

    void ACore::setGraphical(IGraphical *Graphical)
    {
        destroy_t destroy = nullptr;

        if (this->_graphical) {
            this->_graphical->close();
            destroy = (destroy_t)dlsym(this->_handle, "destroy");
            if (destroy)
                destroy(this->_graphical);
        }
        this->_graphical = Graphical;
        if (this->_graphical)
            this->_graphical->init();
    }

    void ACore::setGame(IGame *Game)
    {
        this->_game = Game;
        this->_inGame = (Game != nullptr);
    }

    void ACore::display(std::vector<element_t> elements)
    {
        if (!this->_graphical)
            return;
        this->_graphical->clearElements();
        if (!_inGame) {
            this->_graphical->addElements(_menu.getElements());
        } else {
            this->_graphical->addElements(elements);
        }
        this->_graphical->draw();
    }

    std::string ACore::update()
    {
        std::string event;

        if (!this->_graphical)
            return ("EXIT");
        event = this->_graphical->update();
        if (event == "EXIT")
            return (event);
        if (!this->_inGame) {
            this->_menu.handleInput(event);
            if (this->_menu.isAuthenticated() && !_menu.getSelectedGame().empty()) {
                this->_inGame = true;
            }
        } else if (this->_game) {
            auto gameElements = this->_game->handleEvents(event);
            display(gameElements);
        }
        return ("");
    }
}
