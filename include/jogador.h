#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdbool.h>
#include "raylib.h"

typedef enum {
    DIRECAO_FRENTE,
    DIRECAO_DIREITA,
    DIRECAO_ESQUERDA,
    DIRECAO_COSTAS
} DirecaoJogador;

typedef struct {
    Rectangle corpo;
    int linha;
    int coluna;
    int score;
    int melhorLinha;
    DirecaoJogador direcao;
    double tempoUltimoMovimento;
    double tempoUltimoAtaque;
    double tempoInicioMorte;
} Jogador;

void InicializarSpritesJogador(void);
void FinalizarSpritesJogador(void);
void IniciarJogador(Jogador *jogador);
void AtualizarJogador(Jogador *jogador);
void DesenharJogador(Jogador jogador, bool derrotado);

#endif
