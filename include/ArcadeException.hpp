/*
** EPITECH PROJECT, 2025
** B-OOP-400-LYN-4-1-arcade-youssef.mehili
** File description:
** ArcadeException
*/

#pragma once
#include "Includes.hpp"

namespace arc {
    class ArcadeException : public std::exception {
        public:
            ArcadeException(const std::string &message);
            const char *what() const noexcept override;

        private:
            std::string _message;
    };

    class LibraryError : public ArcadeException {
        public:
            LibraryError(const std::string &path);
    };

    class InvalidLibraryError : public ArcadeException {
        public:
            InvalidLibraryError(const std::string &path);
    };

    class GameError : public ArcadeException {
        public:
            GameError(const std::string &message);
    };

    class GraphicalError : public ArcadeException {
        public:
            GraphicalError(const std::string &message);
    };
}
