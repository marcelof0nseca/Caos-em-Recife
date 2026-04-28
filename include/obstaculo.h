#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "raylib.h"
#include <stdlib.h>

typedef enum {
    TIPO_CARRO,
    TIPO_ONIBUS,
    TIPO_BURACO
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
void IniciarBuraco(Obstaculo *buraco, float x, float y);
Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao);
void AdicionarObstaculo(Obstaculo **lista, Obstaculo *novo);
void LiberarObstaculos(Obstaculo **lista);
void AtualizarCarro(Obstaculo *carro);
void AtualizarListaObstaculos(Obstaculo *lista);
void DesenharCarro(Obstaculo carro);
void DesenharListaObstaculos(Obstaculo *lista);
bool VerificarColisaoCarro(Obstaculo carro, Rectangle jogador);
bool VerificarColisaoLista(Obstaculo *lista, Rectangle jogador);

#endif
