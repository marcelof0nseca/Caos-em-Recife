#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"

typedef struct {
    Rectangle corpo;
    int linha;
    int coluna;
    int score;
    int melhorLinha;
    int linhaCheckpoint;
    int colunaCheckpoint;
} Jogador;

void IniciarJogador(Jogador *jogador);
void AtualizarJogador(Jogador *jogador);
void DesenharJogador(Jogador jogador);

#endif
