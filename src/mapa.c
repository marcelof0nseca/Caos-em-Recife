#include "raylib.h"
#include "config.h"

void DesenharMapa(void)
{
    for (int linha = 0; linha < TOTAL_LINHAS; linha++) {
        Color cor = GREEN;

        if (linha >= 5 && linha <= 9) {
            cor = DARKGRAY;
        }

        DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, TAM_BLOCO, cor);

        if (cor.r == DARKGRAY.r) {
            DrawLine(0, linha * TAM_BLOCO + 20, LARGURA_TELA, linha * TAM_BLOCO + 20, YELLOW);
        } else {
            for (int coluna = 0; coluna < TOTAL_COLUNAS; coluna++) {
                DrawCircle(coluna * TAM_BLOCO + 12, linha * TAM_BLOCO + 28, 2, DARKGREEN);
                DrawCircle(coluna * TAM_BLOCO + 30, linha * TAM_BLOCO + 10, 2, DARKGREEN);
            }
        }
    }
}
