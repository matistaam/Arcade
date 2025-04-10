##
## EPITECH PROJECT, 2025
## B-OOP-400-LYN-4-1-arcade-youssef.mehili
## File description:
## Makefile
##

INCLUDE_DIRS			=	$(shell find include -type d)
INCLUDE_FLAGS			=	$(addprefix -I, $(INCLUDE_DIRS))

CXX						=	g++
CXXFLAGS				=	-std=c++20 -Wall -Wextra -Werror -g3 $(INCLUDE_FLAGS) -fPIC -fno-gnu-unique

# Main executable
NAME					=	arcade
SRC_CORE				=	$(shell find src/Core -name "*.cpp") \
					 		src/ArcadeException.cpp \
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
Nibbler_LIB			=	$(LIB_DIR)/arcade_Nibbler.so
GAMES_LIBS				=	$(SNAKE_LIB) $(Nibbler_LIB)

# Source files for libraries
NCURSES_SRC				=	$(shell find src/Graphicals/Ncurses -name "*.cpp") src/Graphicals/AGraphical.cpp src/ArcadeException.cpp
SDL_SRC					=	$(shell find src/Graphicals/Sdl2 -name "*.cpp") src/Graphicals/AGraphical.cpp src/ArcadeException.cpp
SFML_SRC				=	$(shell find src/Graphicals/Sfml -name "*.cpp") src/Graphicals/AGraphical.cpp src/ArcadeException.cpp
SNAKE_SRC				=	$(shell find src/Games/Snake -name "*.cpp") src/Games/AGame.cpp src/ArcadeException.cpp src/Core/Menu.cpp
Nibbler_SRC			=	$(shell find src/Games/Nibbler -name "*.cpp") src/Games/AGame.cpp src/ArcadeException.cpp src/Core/Menu.cpp

# Object files for libraries
NCURSES_OBJ				=	$(NCURSES_SRC:.cpp=.o)
SDL_OBJ					=	$(SDL_SRC:.cpp=.o)
SFML_OBJ				=	$(SFML_SRC:.cpp=.o)
SNAKE_OBJ				=	$(SNAKE_SRC:.cpp=.o)
Nibbler_OBJ			=	$(Nibbler_SRC:.cpp=.o)

# Library flags
NCURSES_FLAGS			=	-lncurses
SDL2_FLAGS				=	-lSDL2 -lSDL2_ttf -lSDL2_image
SFML_FLAGS				=	-lsfml-graphics -lsfml-window -lsfml-system

all: directory core graphicals games

core: directory $(OBJ_CORE)
	@echo "Linking core components..."
	@$(CXX) $(CXXFLAGS) $(OBJ_CORE) -o $(NAME) -ldl
	@echo "Core components built successfully!"

directory:
	@mkdir -p $(LIB_DIR)

$(NAME): $(OBJ_CORE)
	@echo "Linking $(NAME)..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ -ldl
	@echo "Done!"

graphicals: directory $(GRAPHICAL_LIBS)

$(NCURSES_LIB): $(NCURSES_OBJ)
	@echo "Linking Ncurses library..."
	@$(CXX) -shared $^ -o $@ $(NCURSES_FLAGS)

$(SDL_LIB): $(SDL_OBJ)
	@echo "Linking Sdl2 library..."
	@$(CXX) -shared $^ -o $@ $(SDL2_FLAGS)

$(SFML_LIB): $(SFML_OBJ)
	@echo "Linking Sfml library..."
	@$(CXX) -shared $^ -o $@ $(SFML_FLAGS)

games: directory $(GAMES_LIBS)

$(SNAKE_LIB): $(SNAKE_OBJ)
	@echo "Linking Snake game..."
	@$(CXX) -shared $^ -o $@

$(Nibbler_LIB): $(Nibbler_OBJ)
	@echo "Linking Nibbler game..."
	@$(CXX) -shared $^ -o $@

%.o: %.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJ_CORE) $(NCURSES_OBJ) $(SDL_OBJ) $(SFML_OBJ) $(SNAKE_OBJ) $(Nibbler_OBJ)

fclean: clean
	@echo "Cleaning executable and libraries..."
	@$(RM) $(NAME)
	@$(RM) -r $(LIB_DIR)/arcade_*.so

re: fclean all

.PHONY: all clean fclean re games graphicals directory
