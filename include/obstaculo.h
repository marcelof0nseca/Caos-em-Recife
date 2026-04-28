#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "raylib.h"

typedef enum {
    TIPO_CARRO
} TipoObstaculo;

typedef struct {
    TipoObstaculo tipo;
    Rectangle corpo;
    float velocidade;
    int direcao;
} Obstaculo;

void IniciarCarro(Obstaculo *carro);
void IniciarCarroComDados(Obstaculo *carro, float x, float y, float velocidade, int direcao);
void AtualizarCarro(Obstaculo *carro);
void DesenharCarro(Obstaculo carro);
bool VerificarColisaoCarro(Obstaculo carro, Rectangle jogador);

#endif
