#ifndef MOEDA_H
#define MOEDA_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Rectangle corpo;
    bool coletada;
    int valor;
} Moeda;

void IniciarMoeda(Moeda *moeda, int coluna, int linha);
void DesenharMoeda(Moeda moeda);
bool ColetarMoeda(Moeda *moeda, Rectangle jogador);

#endif
