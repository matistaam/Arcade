/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** AGame
*/

#include "Includes.hpp"

namespace arc {
    AGame::AGame() : _scoreManager(nullptr)
    {
    }

    AGame::~AGame()
    {
    }

    void AGame::setScoreManager(IScoreManager *scoreManager)
    {
        this->_scoreManager = scoreManager;
    }
}
