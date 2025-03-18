/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** IGame
*/

#pragma once
#include "Element.hpp"
#include <string>
#include <vector>

class IGame {
    public:
        virtual ~IGame() = default;

        virtual std::vector<element_t> handleEvents(std::string command) = 0;

    protected:
        std::vector<element_t> _elements;
};
