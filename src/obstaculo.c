#include "obstaculo.h"
#include "config.h"

void IniciarCarro(Obstaculo *carro)
{
    IniciarCarroComDados(carro, X_INICIAL_CARRO, Y_INICIAL_CARRO, VELOCIDADE_CARRO, 1);
}

void IniciarCarroComDados(Obstaculo *carro, float x, float y, float velocidade, int direcao)
{
    carro->tipo = TIPO_CARRO;
    carro->corpo = (Rectangle){x, y, LARGURA_CARRO, ALTURA_CARRO};
    carro->velocidade = velocidade;
    carro->direcao = direcao;
    carro->proximo = NULL;
}

void IniciarOnibus(Obstaculo *onibus, float x, float y, int direcao)
{
    onibus->tipo = TIPO_ONIBUS;
    onibus->corpo = (Rectangle){x, y, 110, ALTURA_CARRO};
    onibus->velocidade = VELOCIDADE_CARRO * 0.65f;
    onibus->direcao = direcao;
    onibus->proximo = NULL;
}

Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao)
{
    Obstaculo *novo = (Obstaculo *)malloc(sizeof(Obstaculo));

    if (novo == NULL) {
        return NULL;
    }

    if (tipo == TIPO_ONIBUS) {
        IniciarOnibus(novo, x, y, direcao);
    } else {
        IniciarCarroComDados(novo, x, y, velocidade, direcao);
    }

    return novo;
}

void AtualizarCarro(Obstaculo *carro)
{
    carro->corpo.x += carro->velocidade * carro->direcao * GetFrameTime();

    if (carro->direcao == 1 && carro->corpo.x > LARGURA_TELA) {
        carro->corpo.x = X_INICIAL_CARRO;
    }

    if (carro->direcao == -1 && carro->corpo.x + carro->corpo.width < 0) {
        carro->corpo.x = LARGURA_TELA + LARGURA_CARRO;
    }
}

void DesenharCarro(Obstaculo carro)
{
    DrawRectangle((int)carro.corpo.x + 3, (int)carro.corpo.y + 25, LARGURA_CARRO, 8, Fade(BLACK, 0.25f));
    DrawRectangleRec(carro.corpo, carro.tipo == TIPO_ONIBUS ? ORANGE : RED);
    DrawRectangle((int)carro.corpo.x + 10, (int)carro.corpo.y + 5, 18, 8, SKYBLUE);
    DrawRectangle((int)carro.corpo.x + 40, (int)carro.corpo.y + 5, 18, 8, SKYBLUE);
    DrawCircle((int)carro.corpo.x + 15, (int)carro.corpo.y + 30, 5, BLACK);
    DrawCircle((int)carro.corpo.x + 55, (int)carro.corpo.y + 30, 5, BLACK);
    DrawRectangle((int)carro.corpo.x + 62, (int)carro.corpo.y + 10, 6, 6, YELLOW);
    DrawRectangleLinesEx(carro.corpo, 2, BLACK);
}

bool VerificarColisaoCarro(Obstaculo carro, Rectangle jogador)
{
    return CheckCollisionRecs(carro.corpo, jogador);
}
