#include "moeda.h"
#include "config.h"

void IniciarMoeda(Moeda *moeda, int coluna, int linha)
{
    moeda->corpo = (Rectangle){
        coluna * TAM_BLOCO + 12,
        linha * TAM_BLOCO + 12,
        16,
        16
    };
    moeda->coletada = false;
    moeda->valor = 1;
}

void DesenharMoeda(Moeda moeda)
{
    if (moeda.coletada) {
        return;
    }

    DrawCircle((int)moeda.corpo.x + 8, (int)moeda.corpo.y + 8, 8, GOLD);
    DrawCircleLines((int)moeda.corpo.x + 8, (int)moeda.corpo.y + 8, 8, ORANGE);
    DrawCircle((int)moeda.corpo.x + 8, (int)moeda.corpo.y + 8, 3, YELLOW);
}

bool ColetarMoeda(Moeda *moeda, Rectangle jogador)
{
    if (moeda->coletada || !CheckCollisionRecs(moeda->corpo, jogador)) {
        return false;
    }

    moeda->coletada = true;
    return true;
}
