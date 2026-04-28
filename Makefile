CC = gcc
SRC = src/main.c src/jogo.c src/jogador.c src/obstaculo.c src/mapa.c
OUT = CrossyRecife.exe
CFLAGS = -Wall -Wextra -Iinclude -IC:/raylib/w64devkit/include
LFLAGS = -LC:/raylib/w64devkit/lib -lraylib -lopengl32 -lgdi32 -lwinmm

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LFLAGS)
