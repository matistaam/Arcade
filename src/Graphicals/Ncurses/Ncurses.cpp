/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Ncurses
*/

#include "Includes.hpp"

namespace arc {
    Ncurses::Ncurses() : AGraphical("")
    {
    }

    Ncurses::~Ncurses()
    {
        close();
    }

    void Ncurses::init()
    {
        if (!initscr())
            throw GraphicalError("NCurses initialization failed");
        if (noecho() == ERR)
            throw GraphicalError("NCurses echo mode configuration failed");
        if (curs_set(0) == ERR)
            throw GraphicalError("NCurses cursor configuration failed");
        if (keypad(stdscr, TRUE) == ERR)
            throw GraphicalError("NCurses keypad configuration failed");
        if (nodelay(stdscr, TRUE) == ERR)
            throw GraphicalError("NCurses nodelay configuration failed");
        if (start_color() == ERR)
            throw GraphicalError("NCurses color initialization failed");
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        refresh();
    }

    void Ncurses::close()
    {
        endwin();
    }

    std::tuple<int, int> Ncurses::convertPositionToChar(int percentX, int percentY)
    {
        int terminalRows = 0;
        int terminalCols = 0;
        getmaxyx(stdscr, terminalRows, terminalCols);
        return std::make_tuple((terminalCols * percentX) / 100, (terminalRows * percentY) / 100);
    }

    void Ncurses::draw_text(element_t element)
    {
        int color = std::stoi(element._color);
        auto [x, y] = convertPositionToChar(std::get<0>(element._position), std::get<1>(element._position));
        int textLength = element._text.length();

        if (color < 1 || color > 6)
            color = 0;
        attron(COLOR_PAIR(color));
        mvprintw(y, x - textLength / 2, "%s", element._text.c_str());
        attroff(COLOR_PAIR(color));
    }

    void Ncurses::draw_image(element_t element)
    {
        (void)element;
    }

    void Ncurses::draw_circle(element_t element)
    {
        element._text = "O";
        int color = std::stoi(element._color);
        if (color < 1 || color > 6)
            color = 0;
        attron(COLOR_PAIR(color));
        mvprintw(std::get<1>(element._position), std::get<0>(element._position), "%s", element._text.c_str());
        attroff(COLOR_PAIR(color));
    }

    void Ncurses::draw_rectangle(element_t element)
    {
        element._text = "#";
        int color = std::stoi(element._color);
        if (color < 1 || color > 6)
            color = 0;
        attron(COLOR_PAIR(color));
        mvprintw(std::get<1>(element._position), std::get<0>(element._position), "%s", element._text.c_str());
        attroff(COLOR_PAIR(color));
    }

    void Ncurses::draw()
    {
        try {
            clear();
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
                        draw_rectangle(element);
                        draw_text(element);
                        break;
                }
            }
            refresh();
        } catch (const std::exception &e) {
            throw GraphicalError("NCurses drawing error: " + std::string(e.what()));
        }
    }

    std::string Ncurses::update()
    {
        int ch = getch();

        if (ch == ERR)
            return ("");
        if (ch == KEY_RESIZE)
            return ("RESIZE");
        switch (ch) {
            case 27:
                return ("EXIT");
            case '\n':
                return ("ENTER");
            case '\t':
                return ("TAB");
            case KEY_BACKSPACE:
            case 127:
                return ("BACKSPACE");
            case '1':
                return ("PREV_LIB");
            case '2':
                return ("NEXT_LIB");
            case KEY_UP:
                return ("UP");
            case KEY_DOWN:
                return ("DOWN");
            case KEY_LEFT:
                return ("LEFT");
            case KEY_RIGHT:
                return ("RIGHT");
            default:
                if (ch >= 32 && ch <= 126)
                    return (std::string(1, (char)ch));
                return ("");
        }
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

    const char* get_type()
    {
        return ("graphical");
    }
}
