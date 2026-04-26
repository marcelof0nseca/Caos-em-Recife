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
    DrawRectangle((int)carro.corpo.x + 10, (int)carro.corpo.y + 5, 18, 8, SKYBLUE);
    DrawRectangle((int)carro.corpo.x + 40, (int)carro.corpo.y + 5, 18, 8, SKYBLUE);
    DrawCircle((int)carro.corpo.x + 15, (int)carro.corpo.y + 30, 5, BLACK);
    DrawCircle((int)carro.corpo.x + 55, (int)carro.corpo.y + 30, 5, BLACK);
    DrawRectangleLinesEx(carro.corpo, 2, BLACK);
}
