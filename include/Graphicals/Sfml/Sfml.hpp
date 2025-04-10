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

            std::string getEvents() override;
            void draw() override;

            void draw_text(element_t element);
            void draw_image(element_t element);
            void draw_circle(element_t element);
            void draw_rectangle(element_t element);

        private:
            std::tuple<int, int> convertPositionToPixels(int percentX, int percentY);
            std::unique_ptr<sf::RenderWindow> _window;
            sf::Font _font;
            sf::Texture _texture;
            int _width;
            int _height;
    };
}
