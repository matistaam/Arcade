/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** ICore
*/

#ifndef ICORE_HPP_
#define ICORE_HPP_

#include "../graphicals/IGraphical.hpp"
#include "../games/IGame.hpp"
#include <string>

class ICore
{
protected:
    IGraphical *_Graphical;
    IGame *_game;

public:
    virtual ~ICore() = default;

    virtual void setGraphical(IGraphical *Graphical) = 0;
    virtual void setGame(IGame *Game) = 0;
    virtual void display(std::vector<element_s> elements) = 0;
};


#endif /* !ICORE_HPP_ */
