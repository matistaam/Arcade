CC = g++
CFLAGS = -Wall -Wextra -fno-gnu-unique -g -std=c++17 -I./include

SRC_CORE_DIR = src/core
SRC_GAMES_DIR = src/games
SRC_GRAPHICALS_DIR = src/graphicals
BUILD_DIR = build
LIB_DIR = lib

CORE_SRC = $(SRC_CORE_DIR)/main.cpp
CORE_OBJ = $(BUILD_DIR)/main.o
CORE_BIN = arcade

GAMES_SRC = $(SRC_GAMES_DIR)/snake/snake.cpp \
			#  $(SRC_GAMES_DIR)/nibbler/nibbler.cpp \
			#  $(SRC_GAMES_DIR)/pacman/pacman.cpp \
			#  $(SRC_GAMES_DIR)/qix/qix.cpp \
			#  $(SRC_GAMES_DIR)/centipede/centipede.cpp \
			#  $(SRC_GAMES_DIR)/solarfox/solarfox.cpp

GAMES_OBJ = $(patsubst $(SRC_GAMES_DIR)/%/%.cpp, $(BUILD_DIR)/%/%.o, $(GAMES_SRC))
GAMES_LIB = $(patsubst $(SRC_GAMES_DIR)/%/%.cpp, $(LIB_DIR)/arcade_%.so, $(GAMES_SRC))

GRAPHICALS_SRC = $(SRC_GRAPHICALS_DIR)/ncurses/ncurses.cpp \
				#   $(SRC_GRAPHICALS_DIR)/sdl2/sdl2.cpp \
				#   $(SRC_GRAPHICALS_DIR)/ndk++/ndk++.cpp \
				#   $(SRC_GRAPHICALS_DIR)/aalib/aalib.cpp \
				#   $(SRC_GRAPHICALS_DIR)/libcaca/libcaca.cpp \
				#   $(SRC_GRAPHICALS_DIR)/allegro5/allegro5.cpp \
				#   $(SRC_GRAPHICALS_DIR)/xlib/xlib.cpp \
				#   $(SRC_GRAPHICALS_DIR)/gtk+/gtk+.cpp \
				#   $(SRC_GRAPHICALS_DIR)/sfml/sfml.cpp \
				#   $(SRC_GRAPHICALS_DIR)/irrlicht/irrlicht.cpp \
				#   $(SRC_GRAPHICALS_DIR)/opengl/opengl.cpp \
				#   $(SRC_GRAPHICALS_DIR)/vulkan/vulkan.cpp \
				#   $(SRC_GRAPHICALS_DIR)/qt5/qt5.cpp

GRAPHICALS_OBJ = $(patsubst $(SRC_GRAPHICALS_DIR)/%/%.cpp, $(BUILD_DIR)/%/%.o, $(GRAPHICALS_SRC))
GRAPHICALS_LIB = $(patsubst $(SRC_GRAPHICALS_DIR)/%/%.cpp, $(LIB_DIR)/arcade_%.so, $(GRAPHICALS_SRC))

all: core graphicals games

core: $(CORE_BIN)

games: $(LIB_DIR) $(GAMES_LIB)

graphicals: $(LIB_DIR) $(GRAPHICALS_LIB)

$(CORE_BIN): $(CORE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(LIB_DIR)/arcade_%.so: $(BUILD_DIR)/%/%.o
	$(CC) $(CFLAGS) -shared -o $@ $< -ldl

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

## Clean rules
clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(CORE_BIN)
	rm -rf $(LIB_DIR)

re: fclean all

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

.PHONY: all core games graphicals clean fclean re