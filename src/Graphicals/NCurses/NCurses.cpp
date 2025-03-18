/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** NCurses
*/

#include "NCurses.hpp"

void Ncurses::init()
{
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color();
    refresh();
}

void Ncurses::close()
{
    endwin();
}

void Ncurses::draw_text(element_t element)
{
    attron(COLOR_PAIR(std::stoi(element._color)));
    mvprintw(std::get<0>(element._position), std::get<1>(element._position), "%s", element._text.c_str());
    attroff(COLOR_PAIR(std::stoi(element._color)));
}

void Ncurses::draw_image(element_t element)
{
    (void)element;
}

void Ncurses::draw_circle(element_t element)
{
    attron(COLOR_PAIR(std::stoi(element._color)));
    int radius = std::get<0>(element._size) / 2;
    int centerY = std::get<0>(element._position);
    int centerX = std::get<1>(element._position);

    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                mvprintw(centerY + y, centerX + x, " ");
            }
        }
    }
    attroff(COLOR_PAIR(std::stoi(element._color)));
}

void Ncurses::draw_rectangle(element_t element)
{
    attron(COLOR_PAIR(std::stoi(element._color)));
    int width = std::get<1>(element._size);
    int height = std::get<0>(element._size);
    int centerY = std::get<0>(element._position);
    int centerX = std::get<1>(element._position);

    int startY = centerY - height/2;
    int startX = centerX - width/2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mvprintw(startY + y, startX + x, " ");
        }
    }
    attroff(COLOR_PAIR(std::stoi(element._color)));
}

void Ncurses::draw()
{
    for (auto &element : this->_elements) {
        switch (element._type) {
            case TEXT:
                draw_text(element);
                break;
            case IMAGE:
                draw_image(element);
                break;
            case CIRCLE:
                draw_circle(element);
                break;
            case RECTANGLE:
                draw_rectangle(element);
                break;
            case BUTTON:
                // To be implemented
                break;
        }
    }
}

std::string Ncurses::update()
{
    draw();
    int ch = getch();

    if (ch == KEY_RESIZE)
        return ("RESIZE");
    return ("");
}
