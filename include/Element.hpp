/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Element
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    enum ELEMENT_TYPE {
        TEXT,
        IMAGE,
        CIRCLE,
        RECTANGLE,
        BUTTON
    };

    enum ELEMENT_COLOR {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE
    };

    typedef struct element_s {
        ELEMENT_TYPE _type;
        std::string _text;
        std::tuple<int, int> _position;
        std::string _color;
        std::tuple<int, int> _size;
        std::string _image_path;
    } element_t;
}
