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
    TIPO_CACHORRO
} TipoObstaculo;

typedef struct Obstaculo {
    TipoObstaculo tipo;
    Rectangle corpo;
    float velocidade;
    int direcao;
    bool mordendo;
    struct Obstaculo *proximo;
} Obstaculo;

void InicializarTexturasObstaculo(void);
void FinalizarTexturasObstaculo(void);
void IniciarCarro(Obstaculo *carro);
void IniciarCarroComDados(Obstaculo *carro, float x, float y, float velocidade, int direcao);
void IniciarOnibus(Obstaculo *onibus, float x, float y, int direcao);
void IniciarMoto(Obstaculo *moto, float x, float y, int direcao);
void IniciarBuraco(Obstaculo *buraco, float x, float y);
void IniciarPedra(Obstaculo *pedra, float x, float y);
void IniciarArvore(Obstaculo *arvore, float x, float y);
void IniciarGuardaSol(Obstaculo *guardaSol, float x, float y);
void IniciarGuardaChuvaFrevo(Obstaculo *guardaChuvaFrevo, float x, float y);
void IniciarCachorro(Obstaculo *cachorro, float x, float y, int direcao);
Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao);
void AdicionarObstaculo(Obstaculo **lista, Obstaculo *novo);
void LiberarObstaculos(Obstaculo **lista);
void AtualizarCarro(Obstaculo *carro);
void AtualizarListaObstaculos(Obstaculo *lista);
void DesenharCarro(Obstaculo carro);
void DesenharListaObstaculos(Obstaculo *lista);
bool VerificarColisaoCarro(Obstaculo carro, Rectangle jogador);
bool VerificarColisaoLista(Obstaculo *lista, Rectangle jogador);
bool VerificarColisaoFixaLista(Obstaculo *lista, Rectangle jogador);

#endif
