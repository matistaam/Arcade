/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** SDL
*/

#include "Includes.hpp"

namespace arc {
    SDL::SDL() : AGraphical(""), _window(nullptr), _renderer(nullptr), _font(nullptr)
    {
        this->_width = 800;
        this->_height = 600;
    }

    SDL::~SDL()
    {
        close();
    }

    void SDL::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            throw GraphicalError(std::string("SDL initialization failed: ") + SDL_GetError());
        if (TTF_Init() < 0)
            throw GraphicalError(std::string("SDL_ttf initialization failed: ") + TTF_GetError());
        this->_window = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            this->_width, this->_height, SDL_WINDOW_SHOWN);
        if (!this->_window)
            throw GraphicalError(std::string("Window creation failed: ") + SDL_GetError());
        this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
        if (!this->_renderer)
            throw GraphicalError(std::string("Renderer creation failed: ") + SDL_GetError());
        this->_font = TTF_OpenFont("assets/fonts/ByteBounce.ttf", 24);
        if (!this->_font)
            throw GraphicalError(std::string("Font loading failed: ") + TTF_GetError());
    }

    void SDL::close()
    {
        if (this->_font) {
            TTF_CloseFont(this->_font);
            this->_font = nullptr;
        }
        if (this->_renderer) {
            SDL_DestroyRenderer(this->_renderer);
            this->_renderer = nullptr;
        }
        if (this->_window) {
            SDL_DestroyWindow(this->_window);
            this->_window = nullptr;
        }
        TTF_Quit();
        SDL_Quit();
    }

    void SDL::draw_text(element_t element)
    {
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface *surface = nullptr;
        SDL_Texture *texture = nullptr;
        SDL_Rect rect = {0, 0, 0, 0};
        auto [x, y] = convertPositionToPixels(std::get<0>(element._position), std::get<1>(element._position));

        if (!this->_font)
            return;
        TTF_CloseFont(this->_font);
        this->_font = TTF_OpenFont("assets/fonts/ByteBounce.ttf", element._font_size);
        if (!this->_font)
            return;

        if (element._color == "1")
            color = {255, 0, 0, 255};
        else if (element._color == "2")
            color = {0, 255, 0, 255};
        else if (element._color == "3")
            color = {255, 255, 0, 255};
        else if (element._color == "4")
            color = {0, 0, 255, 255};
        else if (element._color == "5")
            color = {255, 0, 255, 255};
        else if (element._color == "6")
            color = {0, 255, 255, 255};
        surface = TTF_RenderText_Solid(this->_font, element._text.c_str(), color);
        if (!surface)
            return;
        texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            return;
        }
        rect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(this->_renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void SDL::draw_image(element_t element)
    {
        SDL_Surface *surface = nullptr;
        SDL_Texture *texture = nullptr;
        SDL_Rect rect = {0, 0, 0, 0};

        if (element._image_path.empty())
            return;
        surface = IMG_Load(element._image_path.c_str());
        if (!surface)
            return;
        texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            return;
        }
        rect = {std::get<1>(element._position), std::get<0>(element._position),
            std::get<1>(element._size), std::get<0>(element._size)};
        SDL_RenderCopy(this->_renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void SDL::draw_circle(element_t element)
    {
        int radius = std::get<0>(element._size) / 2;
        int centerX = std::get<1>(element._position);
        int centerY = std::get<0>(element._position);
        int dx = 0;
        int dy = 0;
        Uint8 r = 255;
        Uint8 g = 255;
        Uint8 b = 255;

        if (element._color == "1")
            { r = 255; g = 0; b = 0; }
        else if (element._color == "2")
            { r = 0; g = 255; b = 0; }
        else if (element._color == "3")
            { r = 255; g = 255; b = 0; }
        else if (element._color == "4")
            { r = 0; g = 0; b = 255; }
        else if (element._color == "5")
            { r = 255; g = 0; b = 255; }
        else if (element._color == "6")
            { r = 0; g = 255; b = 255; }
        SDL_SetRenderDrawColor(this->_renderer, r, g, b, 255);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                dx = radius - w;
                dy = radius - h;
                if ((dx*dx + dy*dy) <= (radius * radius))
                    SDL_RenderDrawPoint(this->_renderer, centerX + dx, centerY + dy);
            }
        }
    }

    void SDL::draw_rectangle(element_t element)
    {
        SDL_Rect rect = {std::get<1>(element._position) - std::get<1>(element._size)/2,
        std::get<0>(element._position) - std::get<0>(element._size)/2,
        std::get<1>(element._size), std::get<0>(element._size)};
        Uint8 r = 255;
        Uint8 g = 255;
        Uint8 b = 255;

        if (element._color == "1")
            { r = 255; g = 0; b = 0; }
        else if (element._color == "2")
            { r = 0; g = 255; b = 0; }
        else if (element._color == "3")
            { r = 255; g = 255; b = 0; }
        else if (element._color == "4")
            { r = 0; g = 0; b = 255; }
        else if (element._color == "5")
            { r = 255; g = 0; b = 255; }
        else if (element._color == "6")
            { r = 0; g = 255; b = 255; }
        SDL_SetRenderDrawColor(this->_renderer, r, g, b, 255);
        SDL_RenderFillRect(this->_renderer, &rect);
    }

    void SDL::draw()
    {
        try {
            SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
            SDL_RenderClear(this->_renderer);
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
            SDL_RenderPresent(this->_renderer);
        } catch (const std::exception &e) {
            throw GraphicalError("SDL drawing error: " + std::string(e.what()));
        }
    }

    std::string SDL::update()
    {
        SDL_Event event;
        char c = 0;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return ("EXIT");
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
                return ("RESIZE");
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN)
                    return ("ENTER");
                if (event.key.keysym.sym == SDLK_TAB)
                    return ("TAB");
                if (event.key.keysym.sym == SDLK_BACKSPACE)
                    return ("BACKSPACE");
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return ("EXIT");
                if (event.key.keysym.sym == SDLK_1)
                    return ("PREV_LIB");
                if (event.key.keysym.sym == SDLK_2)
                    return ("NEXT_LIB");
                if (event.key.keysym.sym == SDLK_UP)
                    return ("UP");
                if (event.key.keysym.sym == SDLK_RIGHT)
                    return ("RIGHT");
                if (event.key.keysym.sym == SDLK_DOWN)
                    return ("DOWN");
                if (event.key.keysym.sym == SDLK_LEFT)
                    return ("LEFT");
                if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z) {
                    c = event.key.keysym.sym;
                    return (std::string(1, c));
                }
            }
        }
        return ("");
    }
}

extern "C" {
    arc::IGraphical *create()
    {
        return (new arc::SDL());
    }

    void destroy(arc::IGraphical *instance)
    {
        delete instance;
    }
}
