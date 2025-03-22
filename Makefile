##
## EPITECH PROJECT, 2025
## B-OOP-400-LYN-4-1-arcade-youssef.mehili
## File description:
## Makefile
##

INCLUDE_DIRS			=	$(shell find include -type d)
INCLUDE_FLAGS			=	$(addprefix -I, $(INCLUDE_DIRS))

CXX						=	g++
CXXFLAGS				=	-std=c++20 -Wall -Wextra -Werror -g3 $(INCLUDE_FLAGS) -fPIC

# Main executable
NAME					=	arcade
SRC_CORE				=	$(shell find src/Core -name "*.cpp") \
							src/main.cpp
OBJ_CORE				=	$(SRC_CORE:.cpp=.o)

# Libraries
LIB_DIR					=	lib

# Graphics libraries configuration
NCURSES_LIB				=	$(LIB_DIR)/arcade_ncurses.so
SDL_LIB					=	$(LIB_DIR)/arcade_sdl2.so
SFML_LIB				=	$(LIB_DIR)/arcade_sfml.so

GRAPHICAL_LIBS			=	$(NCURSES_LIB) $(SDL_LIB) $(SFML_LIB)

# Games configuration
SNAKE_LIB				=	$(LIB_DIR)/arcade_snake.so
NIBBLER_LIB				=	$(LIB_DIR)/arcade_nibbler.so

GAMES_LIBS				=	$(SNAKE_LIB) $(NIBBLER_LIB)

# Specific folder names for graphics libraries
NCURSES_DIR			=	NCurses
SDL2_DIR			=	SDL
SFML_DIR			=	SFML

# Library flags
NCURSES_FLAGS			=	-lncurses
SDL2_FLAGS				=	-lSDL2 -lSDL2_ttf -lSDL2_image
SFML_FLAGS				=	-lsfml-graphics -lsfml-window -lsfml-system

all: directory $(NAME) graphicals games

directory:
	@mkdir -p $(LIB_DIR)

$(NAME): $(OBJ_CORE)
	@echo "Linking $(NAME)..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ -ldl
	@echo "Done!"

graphicals: directory $(GRAPHICAL_LIBS)

$(NCURSES_LIB):
	@echo "Building NCurses library..."
	@$(CXX) -shared $(CXXFLAGS) $(shell find src/Graphicals/NCurses -name "*.cpp") src/Graphicals/AGraphical.cpp src/Core/ArcadeException.cpp -o $@ $(NCURSES_FLAGS)

$(SDL_LIB):
	@echo "Building SDL2 library..."
	@$(CXX) -shared $(CXXFLAGS) $(shell find src/Graphicals/SDL -name "*.cpp") src/Graphicals/AGraphical.cpp src/Core/ArcadeException.cpp -o $@ $(SDL2_FLAGS)

$(SFML_LIB):
	@echo "Building SFML library..."
	@$(CXX) -shared $(CXXFLAGS) $(shell find src/Graphicals/SFML -name "*.cpp") src/Graphicals/AGraphical.cpp src/Core/ArcadeException.cpp -o $@ $(SFML_FLAGS)

games: directory $(GAMES_LIBS)

$(SNAKE_LIB):
	@echo "Building Snake game..."
	@$(CXX) -shared $(CXXFLAGS) $(shell find src/Games/Snake -name "*.cpp") src/Games/AGame.cpp src/Core/ArcadeException.cpp src/Core/Menu.cpp -o $@

$(NIBBLER_LIB):
	@echo "Building Nibbler game..."
	@$(CXX) -shared $(CXXFLAGS) $(shell find src/Games/Nibbler -name "*.cpp") src/Games/AGame.cpp src/Core/ArcadeException.cpp -o $@

%.o: %.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJ_CORE)
	@find . -name "*.o" -type f -delete

fclean: clean
	@echo "Cleaning executable and libraries..."
	@$(RM) $(NAME)
	@$(RM) -r $(LIB_DIR)/arcade_*.so

re: fclean all

.PHONY: all clean fclean re games graphicals directory
