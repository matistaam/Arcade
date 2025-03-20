/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** ArcadeException
*/

#include "Includes.hpp"

namespace arc {
    ArcadeException::ArcadeException(const std::string &message) : _message(message)
    {
    }

    const char *ArcadeException::what() const noexcept
    {
        return (this->_message.c_str());
    }

    LibraryError::LibraryError(const std::string &path) : ArcadeException("Cannot load library '" + path + "'")
    {
    }

    InvalidLibraryError::InvalidLibraryError(const std::string &path) : ArcadeException("'" + path + "' not a graphical library")
    {
    }

    GameError::GameError(const std::string &message) : ArcadeException(message)
    {
    }

    GraphicalError::GraphicalError(const std::string &message) : ArcadeException(message)
    {
    }
}