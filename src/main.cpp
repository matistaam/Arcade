/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** main
*/

#include "Includes.hpp"

int display_help(void)
{
    std::cout << "USAGE\n";
    std::cout << "\t./arcade library_path\n";
    std::cout << "\t./arcade -h\n\n";
    std::cout << "DESCRIPTION\n";
    std::cout << "\tlibrary_path\tpath to the initial graphical library to load\n";
    std::cout << "\t-h\t\tdisplay this help\n";
    return (0);
}

int main(int ac, char **av)
{
    std::string event = "";
    std::vector<arc::element_t> elements = {};

    if (ac != 2) {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        return (84);
    }
    std::string arg(av[1]);
    if (arg == "-h")
        return (display_help());
    try {
        arc::ACore core = arc::ACore(av[1]);
        while (true) {
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
