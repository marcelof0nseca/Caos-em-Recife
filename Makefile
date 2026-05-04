SRC = src/main.c src/jogo.c src/jogador.c src/obstaculo.c src/mapa.c src/score.c src/moeda.c
CFLAGS = -Wall -Wextra -Iinclude
PKG_CONFIG ?= pkg-config

ifeq ($(OS),Windows_NT)
	RAYLIB_DIR = C:/raylib/w64devkit
	export PATH := $(RAYLIB_DIR)/bin;$(PATH)
	CC = $(RAYLIB_DIR)/bin/gcc
	OUT = CrossyRecife.exe
	CFLAGS += -I$(RAYLIB_DIR)/include
	LFLAGS = -L$(RAYLIB_DIR)/lib -lraylib -lopengl32 -lgdi32 -lwinmm
	RUN = ./$(OUT)
else
	CC = gcc
	OUT = CrossyRecife
	CFLAGS += $(shell $(PKG_CONFIG) --cflags raylib 2>/dev/null)
	LFLAGS = $(shell $(PKG_CONFIG) --libs raylib 2>/dev/null)
	ifeq ($(strip $(LFLAGS)),)
		LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	endif
	RUN = ./$(OUT)
endif

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LFLAGS)

run: all
	$(RUN)

clean:
	rm -f CrossyRecife CrossyRecife.exe

.PHONY: all run clean
