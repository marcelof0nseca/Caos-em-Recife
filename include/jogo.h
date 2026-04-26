#ifndef JOGO_H
#define JOGO_H

#include <stdbool.h>
#include "jogador.h"
#include "obstaculo.h"

typedef struct {
    Jogador jogador;
    Carro carro;
    bool gameOver;
} Jogo;

void IniciarJogo(Jogo *jogo);
void AtualizarJogo(Jogo *jogo);
void DesenharJogo(Jogo *jogo);

#endif
