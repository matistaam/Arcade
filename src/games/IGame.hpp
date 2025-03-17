/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** IGame
*/

#ifndef IGAME_HPP_
#define IGAME_HPP_

#include <string>
#include <vector>

struct element_s {
    std::string _form;
    std::tuple<int, int> _position;
} element_t;

class IGame
{
protected:
    std::vector<element_s> _elements;
public:
    virtual ~IGame() = default;

    virtual std::vector<element_s> handleEvents(std::string command) = 0;
};

#endif /* !IGAME_HPP_ */
