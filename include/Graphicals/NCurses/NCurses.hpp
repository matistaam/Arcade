/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** NCurses
*/

#pragma once
#include "AGraphical.hpp"

namespace arc {
    class Ncurses : public AGraphical {
        public:
            Ncurses();
            ~Ncurses();

            void init();
            void close();
            void draw_text(element_t element);
            void draw_image(element_t element);
            void draw_circle(element_t element);
            void draw_rectangle(element_t element);
            void draw();
            std::string update();
    };
}
