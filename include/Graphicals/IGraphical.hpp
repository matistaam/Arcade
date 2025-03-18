/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** IGraphical
*/

#pragma once
#include "Element.hpp"
#include <string>
#include <vector>

class IGraphical {
    public:
        virtual ~IGraphical() = default;

        virtual void init() = 0;
        virtual void close() = 0;
        virtual std::string update() = 0;

        protected:
        std::vector<element_t> _elements;
};
