#ifndef JOGO_H
#define JOGO_H

#include <stdbool.h>
#include "jogador.h"
#include "obstaculo.h"
#include "moeda.h"

#define TOTAL_FASES_ATUAIS 2
#define MAX_MOEDAS 6

typedef struct {
    Jogador jogador;
    Obstaculo *obstaculos;
    Moeda moedas[MAX_MOEDAS];
    int totalMoedas;
    int faseAtual;
    bool gameOver;
    bool jogoIniciado;
    bool venceu;
    bool pausado;
    bool tocouTopoNestaFase;
    int recorde;
} Jogo;

void IniciarJogo(Jogo *jogo);
void AtualizarJogo(Jogo *jogo);
void DesenharJogo(Jogo *jogo);
void DesenharInterface(Jogo *jogo);
void DesenharTelaInicial(void);
void DesenharGameOver(Jogo *jogo);

#endif
