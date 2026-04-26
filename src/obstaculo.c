#include "obstaculo.h"
#include "config.h"

void IniciarCarro(Carro *carro)
{
    carro->corpo = (Rectangle){-80, 280, 70, 30};
    carro->velocidade = 180;
}

void AtualizarCarro(Carro *carro)
{
    carro->corpo.x += carro->velocidade * GetFrameTime();

    if (carro->corpo.x > LARGURA_TELA) {
        carro->corpo.x = -80;
    }
}

void DesenharCarro(Carro carro)
{
    DrawRectangleRec(carro.corpo, RED);
}
