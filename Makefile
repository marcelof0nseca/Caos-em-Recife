RAYLIB_DIR = C:/raylib/w64devkit
export PATH := $(RAYLIB_DIR)/bin;$(PATH)

CC = $(RAYLIB_DIR)/bin/gcc
SRC = src/main.c src/jogo.c src/jogador.c src/obstaculo.c src/mapa.c src/score.c
OUT = CrossyRecife.exe
CFLAGS = -Wall -Wextra -Iinclude -I$(RAYLIB_DIR)/include
LFLAGS = -L$(RAYLIB_DIR)/lib -lraylib -lopengl32 -lgdi32 -lwinmm

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LFLAGS)
