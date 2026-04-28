#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "raylib.h"

typedef enum {
    TIPO_CARRO,
    TIPO_ONIBUS
} TipoObstaculo;

typedef struct {
    TipoObstaculo tipo;
    Rectangle corpo;
    float velocidade;
    int direcao;
    struct Obstaculo *proximo;
} Obstaculo;

void IniciarCarro(Obstaculo *carro);
void IniciarCarroComDados(Obstaculo *carro, float x, float y, float velocidade, int direcao);
void IniciarOnibus(Obstaculo *onibus, float x, float y, int direcao);
void AtualizarCarro(Obstaculo *carro);
void DesenharCarro(Obstaculo carro);
bool VerificarColisaoCarro(Obstaculo carro, Rectangle jogador);

#endif
