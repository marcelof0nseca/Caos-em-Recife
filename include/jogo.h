#ifndef JOGO_H
#define JOGO_H

#include <stdbool.h>
#include "jogador.h"
#include "obstaculo.h"
#include "moeda.h"

#define MAX_MOEDAS 12

typedef struct {
    Jogador jogador;
    Obstaculo *obstaculos;
    Moeda moedas[MAX_MOEDAS];
    int totalMoedas;
    int moedasColetadas;
    int faseAtual;
    bool gameOver;
    bool jogoIniciado;
    bool venceu;
    bool pausado;
    int recorde;
} Jogo;

void IniciarJogo(Jogo *jogo);
void AtualizarJogo(Jogo *jogo);
void DesenharJogo(Jogo *jogo);
void DesenharInterface(Jogo *jogo);
void DesenharTelaInicial(void);
void DesenharGameOver(Jogo *jogo);

#endif
