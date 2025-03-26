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

    void AGraphical::clearElements()
    {
        this->_elements.clear();
    }

    void AGraphical::setElements(const std::vector<element_t> &elements)
    {
        this->_elements = elements;
    }
}
