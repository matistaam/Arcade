/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** IGraphical
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class IGraphical {
        public:
            virtual ~IGraphical() = default;

            virtual std::string getEvents() = 0;
            virtual void draw() = 0;

            virtual void clearElements() = 0;
            virtual void setElements(const std::vector<element_t> &elements) = 0;
    };
}
