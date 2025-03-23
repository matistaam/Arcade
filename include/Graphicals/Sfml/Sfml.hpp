/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Sfml
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class SFML : public AGraphical {
        public:
            SFML();
            ~SFML();

            void init() override;
            void close() override;
            std::string update() override;
            void draw() override;

            void draw_text(element_t element);
            void draw_image(element_t element);
            void draw_circle(element_t element);
            void draw_rectangle(element_t element);

        private:
            sf::RenderWindow *_window;
            sf::Font _font;
            sf::Texture _texture;
            int _width;
            int _height;
    };
}
