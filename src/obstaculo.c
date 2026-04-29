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

void IniciarMoto(Obstaculo *moto, float x, float y, int direcao)
{
    moto->tipo = TIPO_MOTO;
    moto->corpo = (Rectangle){x, y + 5, 45, 22};
    moto->velocidade = VELOCIDADE_CARRO * 1.35f;
    moto->direcao = direcao;
    moto->proximo = NULL;
}

void IniciarBuraco(Obstaculo *buraco, float x, float y)
{
    buraco->tipo = TIPO_BURACO;
    buraco->corpo = (Rectangle){x + 4, y + 7, 32, 26};
    buraco->velocidade = 0;
    buraco->direcao = 0;
    buraco->proximo = NULL;
}

Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao)
{
    Obstaculo *novo = (Obstaculo *)malloc(sizeof(Obstaculo));

    if (novo == NULL) {
        return NULL;
    }

    if (tipo == TIPO_BURACO) {
        IniciarBuraco(novo, x, y);
    } else if (tipo == TIPO_MOTO) {
        IniciarMoto(novo, x, y, direcao);
    } else if (tipo == TIPO_ONIBUS) {
        IniciarOnibus(novo, x, y, direcao);
    } else {
        IniciarCarroComDados(novo, x, y, velocidade, direcao);
    }

    return novo;
}

void AdicionarObstaculo(Obstaculo **lista, Obstaculo *novo)
{
    if (novo == NULL) {
        return;
    }

    novo->proximo = *lista;
    *lista = novo;
}

void LiberarObstaculos(Obstaculo **lista)
{
    Obstaculo *atual = *lista;

    while (atual != NULL) {
        Obstaculo *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    *lista = NULL;
}

void AtualizarCarro(Obstaculo *carro)
{
    if (carro->tipo == TIPO_BURACO) {
        return;
    }

    carro->corpo.x += carro->velocidade * carro->direcao * GetFrameTime();

    if (carro->direcao == 1 && carro->corpo.x > LARGURA_TELA) {
        carro->corpo.x = X_INICIAL_CARRO;
    }

    if (carro->direcao == -1 && carro->corpo.x + carro->corpo.width < 0) {
        carro->corpo.x = LARGURA_TELA + LARGURA_CARRO;
    }
}

void AtualizarListaObstaculos(Obstaculo *lista)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        AtualizarCarro(atual);
        atual = atual->proximo;
    }
}

void DesenharCarro(Obstaculo carro)
{
    if (carro.tipo == TIPO_BURACO) {
        int x = (int)carro.corpo.x;
        int y = (int)carro.corpo.y;

        DrawEllipse(x + 16, y + 13, 18, 12, Fade(BLACK, 0.85f));
        DrawEllipse(x + 16, y + 12, 11, 7, DARKGRAY);
        DrawLine(x + 2, y + 11, x - 8, y + 5, BLACK);
        DrawLine(x + 24, y + 4, x + 35, y - 2, BLACK);
        DrawLine(x + 27, y + 20, x + 38, y + 25, BLACK);
        return;
    }

    if (carro.tipo == TIPO_MOTO) {
        int x = (int)carro.corpo.x;
        int y = (int)carro.corpo.y;

        DrawRectangle(x + 8, y + 5, 27, 8, BLUE);
        DrawRectangle(x + 19, y, 12, 7, SKYBLUE);
        DrawCircle(x + 9, y + 19, 5, BLACK);
        DrawCircle(x + 36, y + 19, 5, BLACK);
        DrawLine(x + 35, y + 5, x + 44, y + 1, BLACK);
        DrawRectangleLinesEx(carro.corpo, 1, DARKBLUE);
        return;
    }

    DrawRectangle((int)carro.corpo.x + 3, (int)carro.corpo.y + 25, LARGURA_CARRO, 8, Fade(BLACK, 0.25f));
    DrawRectangleRec(carro.corpo, carro.tipo == TIPO_ONIBUS ? ORANGE : RED);
    DrawRectangle((int)carro.corpo.x + 10, (int)carro.corpo.y + 5, 18, 8, SKYBLUE);
    DrawRectangle((int)carro.corpo.x + 40, (int)carro.corpo.y + 5, 18, 8, SKYBLUE);
    DrawCircle((int)carro.corpo.x + 15, (int)carro.corpo.y + 30, 5, BLACK);
    DrawCircle((int)carro.corpo.x + 55, (int)carro.corpo.y + 30, 5, BLACK);
    DrawRectangle((int)carro.corpo.x + 62, (int)carro.corpo.y + 10, 6, 6, YELLOW);
    DrawRectangleLinesEx(carro.corpo, 2, BLACK);
}

void DesenharListaObstaculos(Obstaculo *lista)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        DesenharCarro(*atual);
        atual = atual->proximo;
    }
}

bool VerificarColisaoCarro(Obstaculo carro, Rectangle jogador)
{
    if (carro.tipo == TIPO_BURACO) {
        Rectangle areaPerigosa = {
            carro.corpo.x + 5,
            carro.corpo.y + 5,
            carro.corpo.width - 10,
            carro.corpo.height - 9
        };

        return CheckCollisionRecs(areaPerigosa, jogador);
    }

    return CheckCollisionRecs(carro.corpo, jogador);
}

bool VerificarColisaoLista(Obstaculo *lista, Rectangle jogador)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        if (VerificarColisaoCarro(*atual, jogador)) {
            return true;
        }

        atual = atual->proximo;
    }

    return false;
}
