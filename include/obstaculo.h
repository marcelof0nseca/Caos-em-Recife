#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "raylib.h"

typedef struct {
    Rectangle corpo;
    float velocidade;
} Carro;

void IniciarCarro(Carro *carro);
void AtualizarCarro(Carro *carro);
void DesenharCarro(Carro carro);

#endif
