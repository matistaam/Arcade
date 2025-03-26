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
#include <random>
#include <dirent.h>
#include <fstream>
#include <sstream>
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
#include "Core/IScoreManager.hpp"
#include "Games/IScorableGame.hpp"
#include "IGame.hpp"
#include "AGame.hpp"
#include "Menu.hpp"
#include "Core.hpp"
#include "Nibbler.hpp"
#include "Snake.hpp"
#include "Ncurses.hpp"
#include "Sdl2.hpp"
#include "Sfml.hpp"
