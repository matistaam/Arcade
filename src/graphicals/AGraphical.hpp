/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** AGraphical
*/

#ifndef AGraphical_HPP_
#define AGraphical_HPP_

#include "IGraphical.hpp"

class AGraphical : public IGraphical
{
private:

public:
    AGraphical(std::string path);
    ~AGraphical();

    void init();
    void close();
    std::string update();
    void draw();
};

#endif /* !AGraphical_HPP_ */
