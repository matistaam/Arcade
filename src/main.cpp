/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** main
*/

#include "ACore.hpp"
#include <iostream>
#include <thread>

int display_help(void)
{
    std::cout << "USAGE\n";
    std::cout << "\t./arcade library_path\n\n";
    std::cout << "DESCRIPTION\n";
    std::cout << "\tlibrary_path\tpath to the initial graphical library to load\n";
    return (84);
}

std::vector<arc::element_t> create_test_elements(void)
{
    std::vector<arc::element_t> elements;
    arc::element_t text = {};
    arc::element_t rect = {};
    arc::element_t circle = {};

    text._type = arc::TEXT;
    text._text = "Arcade Test";
    text._position = std::make_tuple(50, 50);
    text._color = "1";
    elements.push_back(text);
    rect._type = arc::RECTANGLE;
    rect._position = std::make_tuple(100, 100);
    rect._size = std::make_tuple(50, 100);
    rect._color = "2";
    elements.push_back(rect);
    circle._type = arc::CIRCLE;
    circle._position = std::make_tuple(200, 200);
    circle._size = std::make_tuple(40, 40);
    circle._color = "4";
    elements.push_back(circle);
    return (elements);
}

int main(int ac, char **av)
{
    std::vector<arc::element_t> elements = {};
    std::string event = "";

    if (ac != 2)
        return (display_help());
    try {
        arc::ACore core = arc::ACore(av[1]);
        elements = create_test_elements();
        while (true) {
            core.display(elements);
            event = core.update();
            if (event == "EXIT")
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return (84);
    }
    return (0);
}
