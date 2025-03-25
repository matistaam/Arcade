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

            void init() override;
            void close() override;
            std::string update() override;
            void draw() override;

            void clearElements() override;
            void addElements(const std::vector<element_t> &elements) override;

        protected:
            std::tuple<int, int> convertPositionToPixels(int percentX, int percentY);
            int _width = 800;
            int _height = 600;
    };
}
