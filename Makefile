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

# Dynamic library sources
GRAPHICAL_DIRS			=	$(shell find src/Graphicals/* -maxdepth 0 -type d ! -name ".*")
GRAPHICAL_NAMES			=	$(notdir $(GRAPHICAL_DIRS))
GRAPHICAL_LIBS			=	$(addprefix $(LIB_DIR)/$(GRAPH_DIR)/arcade_, $(addsuffix .so, $(shell echo $(GRAPHICAL_NAMES) | tr '[:upper:]' '[:lower:]')))

# Specific folder names for graphics libraries
NCURSES_DIR			=	NCurses
SDL_DIR				=	SDL
SFML_DIR			=	SFML

# Library flags
NCURSES_FLAGS			=	-lncurses
SDL_FLAGS				=	-lSDL2 -lSDL2_ttf -lSDL2_image
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

graphicals: directories $(GRAPHICAL_LIBS)

$(LIB_DIR)/$(GRAPH_DIR)/arcade_%.so:
	@echo "Building $* graphical library..."
	@$(CXX) -shared $(CXXFLAGS) $(shell find src/Graphicals/$($(shell echo $* | tr '[:lower:]' '[:upper:]')_DIR) -name "*.cpp") src/Graphicals/AGraphical.cpp -o $@ $($(shell echo $* | tr '[:lower:]' '[:upper:]')_FLAGS)

games: directories
	@echo "Building games..."
    # Add your games compilation here

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
	@$(RM) -r $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re games graphicals directories
