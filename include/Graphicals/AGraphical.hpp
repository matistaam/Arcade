/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** AGraphical
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class AGraphical : public IGraphical {
        public:
            AGraphical(std::string path);
            ~AGraphical();

            virtual std::string update() = 0;
            virtual void draw() = 0;

            void clearElements() override;
            void addElements(const std::vector<element_t> &elements) override;

        protected:
            std::tuple<int, int> convertPositionToPixels(int percentX, int percentY);

            std::vector<element_t> _elements;
            int _width = 800;
            int _height = 600;
    };
}
