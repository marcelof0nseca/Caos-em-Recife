#ifndef JOGO_H
#define JOGO_H

#include <stdbool.h>
#include "jogador.h"
#include "obstaculo.h"

typedef struct {
    Jogador jogador;
    Carro carro;
    Carro carro2;
    bool gameOver;
    bool jogoIniciado;
    bool venceu;
    bool pausado;
} Jogo;

void IniciarJogo(Jogo *jogo);
void AtualizarJogo(Jogo *jogo);
void DesenharJogo(Jogo *jogo);
void DesenharInterface(Jogo *jogo);
void DesenharTelaInicial(void);
void DesenharGameOver(Jogo *jogo);

#endif
