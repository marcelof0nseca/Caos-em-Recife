#include "raylib.h"
#include "config.h"
#include "mapa.h"

void DesenharMapa(void)
{
    for (int linha = 0; linha < TOTAL_LINHAS; linha++) {
        Color cor = GREEN;

        if (LinhaEhRua(linha)) {
            cor = DARKGRAY;
        }

        DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, TAM_BLOCO, cor);

        if (cor.r == DARKGRAY.r) {
            DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, ESPESSURA_BORDA_RUA, BLACK);
            DrawRectangle(0, linha * TAM_BLOCO + TAM_BLOCO - ESPESSURA_BORDA_RUA, LARGURA_TELA, ESPESSURA_BORDA_RUA, BLACK);
            DrawLine(0, linha * TAM_BLOCO + 20, LARGURA_TELA, linha * TAM_BLOCO + 20, YELLOW);
        } else {
            for (int coluna = 0; coluna < TOTAL_COLUNAS; coluna++) {
                DrawCircle(coluna * TAM_BLOCO + 12, linha * TAM_BLOCO + 28, 2, DARKGREEN);
                DrawCircle(coluna * TAM_BLOCO + 30, linha * TAM_BLOCO + 10, 2, DARKGREEN);
            }
        }
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
           linha == 33 || linha == 34 ||
           linha == 36 || linha == 37 ||
           linha == 39 || linha == 40 ||
           linha == 42 || linha == 43;
}
