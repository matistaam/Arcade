/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** AGraphical
*/

#include "Includes.hpp"

namespace arc {
    AGraphical::AGraphical(std::string path)
    {
        (void)path;
    }

    AGraphical::~AGraphical()
    {
    }

    void AGraphical::init()
    {
    }

    void AGraphical::close()
    {
    }

    std::string AGraphical::update()
    {
        return ("");
    }

    void AGraphical::draw()
    {
    }

    void AGraphical::clearElements()
    {
        this->_elements.clear();
    }

    void AGraphical::addElements(const std::vector<element_t> &elements)
    {
        this->_elements = elements;
    }

    std::tuple<int, int> AGraphical::convertPositionToPixels(int percentX, int percentY)
    {
        return (std::make_tuple((this->_width * percentX) / 100, (this->_height * percentY) / 100));
    }
}
