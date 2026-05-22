#ifndef JOGO_H
#define JOGO_H

#include <stdbool.h>
#include "jogador.h"
#include "obstaculo.h"
#include "score.h"

/* Struct principal: guarda tudo que muda durante a partida. */
typedef struct {
    Jogador jogador;
    Obstaculo *obstaculos; /* ponteiro para o inicio da lista encadeada */
    int faseAtual;
    bool gameOver;
    bool jogoIniciado;
    bool venceu;
    bool pausado;
    int recorde;
    int topScores[TOTAL_RECORDES];
} Jogo;

void IniciarJogo(Jogo *jogo);
void AtualizarJogo(Jogo *jogo);
void DesenharJogo(Jogo *jogo);
void DesenharInterface(Jogo *jogo);
void DesenharGameOver(Jogo *jogo);

#endif
