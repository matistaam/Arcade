/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** SFML
*/

#include "Includes.hpp"

namespace arc {
    SFML::SFML() : AGraphical(""), _window(nullptr)
    {
        this->_width = 800;
        this->_height = 600;
    }

    SFML::~SFML()
    {
        close();
    }

    void SFML::init()
    {
        this->_window = new sf::RenderWindow(sf::VideoMode(this->_width, this->_height), "Arcade");
        if (!this->_window)
            throw GraphicalError("Window creation failed");
        if (!this->_font.loadFromFile("assets/fonts/ByteBounce.ttf"))
            throw GraphicalError("Font loading failed");
        this->_window->setFramerateLimit(60);
    }

    void SFML::close()
    {
        if (this->_window) {
            this->_window->close();
            delete this->_window;
            this->_window = nullptr;
        }
    }

    void SFML::draw_text(element_t element)
    {
        sf::Text text = sf::Text();
        sf::Color color = sf::Color::White;
        auto [x, y] = convertPositionToPixels(std::get<0>(element._position), std::get<1>(element._position));

        text.setFont(this->_font);
        text.setString(element._text);
        text.setCharacterSize(element._font_size);
        if (element._color == "1")
            color = sf::Color::Red;
        else if (element._color == "2")
            color = sf::Color::Green;
        else if (element._color == "3")
            color = sf::Color::Yellow;
        else if (element._color == "4")
            color = sf::Color::Blue;
        else if (element._color == "5")
            color = sf::Color::Magenta;
        else if (element._color == "6")
            color = sf::Color::Cyan;
        text.setFillColor(color);
        text.setPosition(x, y);
        this->_window->draw(text);
    }

    void SFML::draw_image(element_t element)
    {
        sf::Sprite sprite = sf::Sprite();

        if (element._image_path.empty() || !this->_texture.loadFromFile(element._image_path))
            return;
        sprite.setTexture(this->_texture);
        sprite.setPosition(std::get<1>(element._position), std::get<0>(element._position));
        float scaleX = static_cast<float>(std::get<1>(element._size)) / this->_texture.getSize().x;
        float scaleY = static_cast<float>(std::get<0>(element._size)) / this->_texture.getSize().y;
        sprite.setScale(scaleX, scaleY);
        this->_window->draw(sprite);
    }

    void SFML::draw_circle(element_t element)
    {
        sf::CircleShape circle(std::get<0>(element._size) / 2);
        sf::Color color = sf::Color::White;

        circle.setPosition(std::get<1>(element._position) - std::get<0>(element._size) / 2,
            std::get<0>(element._position) - std::get<0>(element._size) / 2);
        if (element._color == "1")
            color = sf::Color::Red;
        else if (element._color == "2")
            color = sf::Color::Green;
        else if (element._color == "3")
            color = sf::Color::Yellow;
        else if (element._color == "4")
            color = sf::Color::Blue;
        else if (element._color == "5")
            color = sf::Color::Magenta;
        else if (element._color == "6")
            color = sf::Color::Cyan;
        circle.setFillColor(color);
        this->_window->draw(circle);
    }

    void SFML::draw_rectangle(element_t element)
    {
        sf::RectangleShape rectangle(sf::Vector2f(std::get<1>(element._size),
            std::get<0>(element._size)));
        sf::Color color = sf::Color::White;

        rectangle.setPosition(std::get<1>(element._position) - std::get<1>(element._size) / 2,
            std::get<0>(element._position) - std::get<0>(element._size) / 2);
        if (element._color == "1")
            color = sf::Color::Red;
        else if (element._color == "2")
            color = sf::Color::Green;
        else if (element._color == "3")
            color = sf::Color::Yellow;
        else if (element._color == "4")
            color = sf::Color::Blue;
        else if (element._color == "5")
            color = sf::Color::Magenta;
        else if (element._color == "6")
            color = sf::Color::Cyan;
        rectangle.setFillColor(color);
        this->_window->draw(rectangle);
    }

    void SFML::draw()
    {
        try {
            this->_window->clear(sf::Color::Black);
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
            this->_window->display();
        } catch (const std::exception &e) {
            throw GraphicalError("SFML drawing error: " + std::string(e.what()));
        }
    }

    std::string SFML::update()
    {
        sf::Event event;

        while (this->_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                return ("EXIT");
            if (event.type == sf::Event::Resized)
                return ("RESIZE");
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128 && event.text.unicode >= 32)
                    return (std::string(1, static_cast<char>(event.text.unicode)));
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return)
                    return ("ENTER");
                if (event.key.code == sf::Keyboard::Tab)
                    return ("TAB");
                if (event.key.code == sf::Keyboard::BackSpace)
                    return ("BACKSPACE");
                if (event.key.code == sf::Keyboard::Escape)
                    return ("EXIT");
            }
        }
        draw();
        return ("");
    }
}

extern "C" {
    arc::IGraphical *create()
    {
        return (new arc::SFML());
    }

    void destroy(arc::IGraphical *instance)
    {
        delete instance;
    }
}
