/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** IScorableGame
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class IScorableGame {
        public:
            virtual ~IScorableGame() = default;

            virtual void setScoreManager(IScoreManager *scoreManager) = 0;
    };
}