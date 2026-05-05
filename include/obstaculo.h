#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "raylib.h"
#include <stdlib.h>

typedef enum {
    TIPO_CARRO,
    TIPO_ONIBUS,
    TIPO_MOTO,
    TIPO_BURACO,
    TIPO_PEDRA,
    TIPO_ARVORE,
    TIPO_GUARDA_SOL,
    TIPO_GUARDA_CHUVA_FREVO,
    TIPO_CACHORRO,
    TIPO_POSTE,
    TIPO_LIXO_GRANDE
} TipoObstaculo;

typedef struct Obstaculo {
    TipoObstaculo tipo;
    Rectangle corpo;
    float velocidade;
    int direcao;
    bool mordendo;
    int variante;
    struct Obstaculo *proximo;
} Obstaculo;

void InicializarTexturasObstaculo(void);
void FinalizarTexturasObstaculo(void);
Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao);
void AdicionarObstaculo(Obstaculo **lista, Obstaculo *novo);
void LiberarObstaculos(Obstaculo **lista);
void AtualizarListaObstaculos(Obstaculo *lista);
void DesenharListaObstaculos(Obstaculo *lista);
bool VerificarColisaoLista(Obstaculo *lista, Rectangle jogador);
bool VerificarColisaoFixaLista(Obstaculo *lista, Rectangle jogador);
bool VerificarApoioAlagamento(Obstaculo *lista, Rectangle jogador);
float ObterVelocidadeApoioAlagamento(Obstaculo *lista, Rectangle jogador);

#endif
