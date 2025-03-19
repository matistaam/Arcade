/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** NCurses
*/

#include "NCurses.hpp"

namespace arc {
    Ncurses::Ncurses() : AGraphical("")
    {
    }

    Ncurses::~Ncurses()
    {
    }

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
        int radius = 0;
        int centerY = 0;
        int centerX = 0;
        int x = 0;
        int y = 0;

        attron(COLOR_PAIR(std::stoi(element._color)));
        radius = std::get<0>(element._size) / 2;
        centerY = std::get<0>(element._position);
        centerX = std::get<1>(element._position);
        for (y = -radius; y <= radius; y++) {
            for (x = -radius; x <= radius; x++) {
                if (x*x + y*y <= radius*radius)
                    mvprintw(centerY + y, centerX + x, " ");
            }
        }
        attroff(COLOR_PAIR(std::stoi(element._color)));
    }

    void Ncurses::draw_rectangle(element_t element)
    {
        int width = 0;
        int height = 0;
        int centerY = 0;
        int centerX = 0;
        int startY = 0;
        int startX = 0;
        int x = 0;
        int y = 0;

        attron(COLOR_PAIR(std::stoi(element._color)));
        width = std::get<1>(element._size);
        height = std::get<0>(element._size);
        centerY = std::get<0>(element._position);
        centerX = std::get<1>(element._position);
        startY = centerY - height/2;
        startX = centerX - width/2;
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
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
        int ch = 0;

        draw();
        ch = getch();
        if (ch == KEY_RESIZE)
            return ("RESIZE");
        return ("");
    }
}

extern "C" {
    arc::IGraphical *create()
    {
        return (new arc::Ncurses());
    }

    void destroy(arc::IGraphical *instance)
    {
        delete instance;
    }
}
