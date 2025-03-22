/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** AGame
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class AGame : public IGame {
        public:
            AGame();
            ~AGame();

            virtual void setScoreManager(IScoreManager *scoreManager);

        protected:
            IScoreManager *_scoreManager;
    };
}
