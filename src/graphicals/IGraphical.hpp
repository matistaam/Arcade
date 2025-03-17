/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** IGraphical
*/

#ifndef IGraphical_HPP_
#define IGraphical_HPP_

#include <string>
#include <vector>
#include "../element.hpp"

class IGraphical
{
protected:
    std::vector<element_t> _elements;
public:
    virtual ~IGraphical() = default;

    virtual void init() = 0;
    virtual void close() = 0;
    virtual std::string update() = 0;
};

#endif /* !IGraphical_HPP_ */
