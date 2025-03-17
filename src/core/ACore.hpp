/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** ACore
*/

#ifndef ACORE_HPP_
#define ACORE_HPP_

#include "ICore.hpp"
#include "../graphicals/AGraphical.hpp"
#include "../games/AGame.hpp"
#include <string>

class ACore : public ICore
{
private:
    IGraphical *_Graphical;
    IGame *_game;

public:
    ACore(std::string path);
    ~ACore();

    void setGraphical(IGraphical *Graphical);
    void setGame(IGame *Game);
    void display(std::vector<element_s> elements);
};

ACore::ACore(std::string path)
{
    this->_Graphical = new AGraphical(path);
}

ACore::~ACore()
{
}

#endif /* !ACORE_HPP_ */
