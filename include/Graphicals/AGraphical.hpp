/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** AGraphical
*/

#pragma once
#include "IGraphical.hpp"

namespace arc {
    class AGraphical : public IGraphical {
        public:
            AGraphical(std::string path);
            ~AGraphical();

            void init();
            void close();
            std::string update();
            void draw();

        private:

    };
}
