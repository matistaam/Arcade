/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** element
*/

#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_

#include <string>
#include <tuple>

enum ELEMENT_TYPE {
    TEXT,
    BUTTON,
    CIRCLE,
    RECTANGLE,
    IMAGE
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

struct element_s {
    ELEMENT_TYPE type;
    std::string text;
    std::tuple<int, int> position;
    std::string color;
    std::tuple<int, int> size;
    std::string image_path;
} element_t;

#endif /* !ELEMENT_HPP_ */
