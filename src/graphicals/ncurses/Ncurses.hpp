/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** Ncurses
*/

#ifndef NCURSES_HPP_
#define NCURSES_HPP_

#include "../AGraphical.hpp"
#include <ncurses.h>

class Ncurses : public AGraphical
{
public:
    Ncurses();
    ~Ncurses();

    void init() {
        initscr();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        start_color();
        refresh();
    }

    void close() {
        endwin();
    }


    void draw_text(element_s element) {
        attron(COLOR_PAIR(std::stoi(element.color)));
        mvprintw(std::get<0>(element.position), std::get<1>(element.position), element.text.c_str());
        attroff(COLOR_PAIR(std::stoi(element.color)));
    }

    void draw_image(element_s element) {

    }

    void draw_circle(element_s element) {
        attron(COLOR_PAIR(std::stoi(element.color)));
        int radius = std::get<0>(element.size) / 2;
        int centerY = std::get<0>(element.position);
        int centerX = std::get<1>(element.position);

        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                if (x*x + y*y <= radius*radius) {
                    mvprintw(centerY + y, centerX + x, " ");
                }
            }
        }
        attroff(COLOR_PAIR(std::stoi(element.color)));
    }

    void draw_rectangle(element_s element) {
        attron(COLOR_PAIR(std::stoi(element.color)));
        int width = std::get<1>(element.size);
        int height = std::get<0>(element.size);
        int centerY = std::get<0>(element.position);
        int centerX = std::get<1>(element.position);

        int startY = centerY - height/2;
        int startX = centerX - width/2;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                mvprintw(startY + y, startX + x, " ");
            }
        }
        attroff(COLOR_PAIR(std::stoi(element.color)));
    }

    void draw() {
        for (auto &element : _elements) {
            switch (element.type) {
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
            }
        }
    }

    std::string update() {
        draw();
        int ch = getch();
        if (ch == KEY_RESIZE) {
            return "RESIZE";
        }
        return "";
    }
};

#endif /* !NCURSES_HPP_ */
