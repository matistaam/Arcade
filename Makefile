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
GAMES_DIR				=	games
GRAPH_DIR				=	graphicals

# NCurses library
NCURSES_NAME			=	arcade_ncurses.so
NCURSES_SRC				=	$(shell find src/Graphicals/NCurses -name "*.cpp") \
            				src/Graphicals/AGraphical.cpp
NCURSES_OBJ				=	$(NCURSES_SRC:.cpp=.o)
NCURSES_FLAGS			=	-lncurses

# SDL2 library
SDL_NAME				=	arcade_sdl2.so
SDL_SRC					=	$(shell find src/Graphicals/SDL -name "*.cpp") \
                            src/Graphicals/AGraphical.cpp
SDL_OBJ					=	$(SDL_SRC:.cpp=.o)
SDL_FLAGS				=	-lSDL2 -lSDL2_ttf -lSDL2_image

# SFML library
SFML_NAME				=	arcade_sfml.so
SFML_SRC				=	$(shell find src/Graphicals/SFML -name "*.cpp") \
                            src/Graphicals/AGraphical.cpp
SFML_OBJ				=	$(SFML_SRC:.cpp=.o)
SFML_FLAGS				=	-lsfml-graphics -lsfml-window -lsfml-system

all: directories $(NAME) graphicals games

directories:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(LIB_DIR)/$(GAMES_DIR)
	@mkdir -p $(LIB_DIR)/$(GRAPH_DIR)

$(NAME): $(OBJ_CORE)
	@echo "Linking $(NAME)..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ -ldl
	@echo "Done!"

graphicals: directories $(LIB_DIR)/$(GRAPH_DIR)/$(NCURSES_NAME) $(LIB_DIR)/$(GRAPH_DIR)/$(SFML_NAME) $(LIB_DIR)/$(GRAPH_DIR)/$(SDL_NAME)

$(LIB_DIR)/$(GRAPH_DIR)/$(NCURSES_NAME): $(NCURSES_OBJ)
	@echo "Building NCurses graphical library..."
	@$(CXX) -shared $(CXXFLAGS) $^ -o $@ $(NCURSES_FLAGS)

$(LIB_DIR)/$(GRAPH_DIR)/$(SDL_NAME): $(SDL_OBJ)
	@echo "Building SDL2 graphical library..."
	@$(CXX) -shared $(CXXFLAGS) $^ -o $@ $(SDL_FLAGS)

$(LIB_DIR)/$(GRAPH_DIR)/$(SFML_NAME): $(SFML_OBJ)
	@echo "Building SFML graphical library..."
	@$(CXX) -shared $(CXXFLAGS) $^ -o $@ $(SFML_FLAGS)

games: directories
	@echo "Building games..."
    # Add your games compilation here

%.o: %.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJ_CORE) $(NCURSES_OBJ) $(SDL_OBJ) $(SFML_OBJ)

fclean: clean
	@echo "Cleaning executable and libraries..."
	@$(RM) $(NAME)
	@$(RM) -r $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re games graphicals directories
