/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Ncurses
*/
#pragma once
#include "Includes.hpp"

namespace arc {
    class Ncurses : public AGraphical {
        public:
            Ncurses();
            ~Ncurses();

            void init() override;
            void close() override;
            std::string update() override;
            void draw() override;

        private:
            void draw_text(element_t element);
            void draw_image(element_t element);
            void draw_circle(element_t element);
            void draw_rectangle(element_t element);
            void draw_border(element_t element);

            std::tuple<int, int> convertPositionToChar(int percentX, int percentY);

            WINDOW *_window;
            int _maxY;
            int _maxX;
            bool _isInitialized;
    };
}
