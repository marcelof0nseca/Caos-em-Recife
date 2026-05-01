#include "raylib.h"
#include "config.h"
#include "mapa.h"

static Texture2D texturaPista = {0};
static Texture2D texturaGrama = {0};

void InicializarMapa(void)
{
    texturaPista = LoadTexture("assets/pista.png");
    texturaGrama = LoadTexture("assets/grama.png");
}

void FinalizarMapa(void)
{
    if (texturaPista.id != 0) {
        UnloadTexture(texturaPista);
    }

    if (texturaGrama.id != 0) {
        UnloadTexture(texturaGrama);
    }
}

static void DesenharPista(int linha)
{
    if (texturaPista.id != 0) {
        Rectangle origem = {
            0,
            texturaPista.height * 0.36f,
            texturaPista.width,
            texturaPista.height * 0.31f
        };
        Rectangle destino = {
            0,
            linha * TAM_BLOCO,
            LARGURA_TELA,
            TAM_BLOCO
        };

        DrawTexturePro(texturaPista, origem, destino, (Vector2){0, 0}, 0, WHITE);
        return;
    }

    DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, TAM_BLOCO, DARKGRAY);
    DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, ESPESSURA_BORDA_RUA, BLACK);
    DrawRectangle(0, linha * TAM_BLOCO + TAM_BLOCO - ESPESSURA_BORDA_RUA, LARGURA_TELA, ESPESSURA_BORDA_RUA, BLACK);
    DrawLine(0, linha * TAM_BLOCO + 20, LARGURA_TELA, linha * TAM_BLOCO + 20, YELLOW);
}

static void DesenharGrama(int linha)
{
    if (texturaGrama.id != 0) {
        Rectangle origem = {
            0,
            0,
            texturaGrama.width,
            texturaGrama.height
        };
        Rectangle destino = {
            0,
            linha * TAM_BLOCO,
            LARGURA_TELA,
            TAM_BLOCO
        };

        DrawTexturePro(texturaGrama, origem, destino, (Vector2){0, 0}, 0, WHITE);
        return;
    }

    DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, TAM_BLOCO, GREEN);

    for (int coluna = 0; coluna < TOTAL_COLUNAS; coluna++) {
        DrawCircle(coluna * TAM_BLOCO + 12, linha * TAM_BLOCO + 28, 2, DARKGREEN);
        DrawCircle(coluna * TAM_BLOCO + 30, linha * TAM_BLOCO + 10, 2, DARKGREEN);
    }
}

void DesenharMapa(void)
{
    for (int linha = 0; linha < TOTAL_LINHAS; linha++) {
        if (LinhaEhRua(linha)) {
            DesenharPista(linha);
            continue;
        }

        DesenharGrama(linha);
    }
}

bool LinhaEhRua(int linha)
{
    return linha == 2 || linha == 3 ||
           linha == 5 || linha == 6 ||
           linha == 8 || linha == 9 ||
           linha == 11 || linha == 12 ||
           linha == 14 || linha == 15 ||
           linha == 17 || linha == 18 ||
           linha == 20 || linha == 21 ||
           linha == 24 || linha == 25 ||
           linha == 27 || linha == 28 ||
           linha == 30 || linha == 31 ||
           linha == 34 || linha == 35 ||
           linha == 37 || linha == 38 ||
           linha == 40 || linha == 41;
}
