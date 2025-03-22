/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** IScoreManager
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class IScoreManager {
        public:
            virtual ~IScoreManager() = default;

            virtual void updateHighScore(const std::string &game, int score) = 0;
            virtual int getHighScore(const std::string &game) const = 0;
    };
}
