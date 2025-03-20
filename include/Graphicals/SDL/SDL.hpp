/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** SDL
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class SDL : public AGraphical {
        public:
            SDL();
            ~SDL();

            void init() override;
            void close() override;
            std::string update() override;
            void draw() override;

            void draw_text(element_t element);
            void draw_image(element_t element);
            void draw_circle(element_t element);
            void draw_rectangle(element_t element);

        private:
            SDL_Window *_window;
            SDL_Renderer *_renderer;
            TTF_Font *_font;
            int _width;
            int _height;
    };
}
