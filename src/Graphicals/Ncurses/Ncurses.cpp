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
        return std::make_tuple((terminalRows * percentX) / 100, (terminalCols * percentY) / 100);
    }

    void Ncurses::draw_text(element_t element)
    {
        int color = std::stoi(element._color);
        auto [x, y] = convertPositionToChar(std::get<0>(element._position), std::get<1>(element._position));
        int textLength = element._text.length();

        if (color < 1 || color > 6)
            color = 0;
        attron(COLOR_PAIR(color));
        mvprintw(y - element._text.length() / 2, x - textLength / 2, "%s", element._text.c_str());
        attroff(COLOR_PAIR(color));
    }

    void Ncurses::draw_image(element_t element)
    {
        (void)element;
    }

    void Ncurses::draw_circle(element_t element)
    {
        int color = std::stoi(element._color);
        auto [centerY, centerX] = convertPositionToChar(std::get<0>(element._position), std::get<1>(element._position));
        int radius = std::get<0>(element._size) / 40; // Ajuster la taille pour l'affichage en caractères
        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                if (x*x + y*y <= radius*radius)
                    mvprintw(centerY + y, centerX + x, " ");
            }
        }
        attroff(COLOR_PAIR(color));
    }

    void Ncurses::draw_rectangle(element_t element)
    {
        int color = std::stoi(element._color);
        int width = std::get<1>(element._size);
        int height = std::get<0>(element._size);
        int centerY = std::get<0>(element._position);
        int centerX = std::get<1>(element._position);
        int startY = centerY - height/2;
        int startX = centerX - width/2;

        if (color < 1 || color > 6)
            color = 0;
        attron(COLOR_PAIR(color));
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++)
                mvprintw(startY + y, startX + x, " ");
        }
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
