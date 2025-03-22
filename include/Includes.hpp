/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** Includes
*/

#pragma once

// Standard includes
#include <iostream>
#include <thread>
#include <list>
#include <chrono>
#include <random>
#include <dirent.h>

// System includes
#include <dlfcn.h>

// Graphics libraries includes
#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SFML/Graphics.hpp>

// Project includes - Order matters to avoid circular dependencies
#include "Element.hpp"
#include "ArcadeException.hpp"
#include "IGraphical.hpp"
#include "AGraphical.hpp"
#include "IGame.hpp"
#include "AGame.hpp"
#include "Menu.hpp"
#include "ICore.hpp"
#include "ACore.hpp"
#include "Nibbler.hpp"
#include "Snake.hpp"
#include "NCurses.hpp"
#include "SDL.hpp"
#include "SFML.hpp"
