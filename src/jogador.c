#include "jogador.h"
#include "config.h"

void IniciarJogador(Jogador *jogador)
{
    jogador->linha = LINHA_INICIAL_JOGADOR;
    jogador->coluna = COLUNA_INICIAL_JOGADOR;
    jogador->corpo = (Rectangle){jogador->coluna * TAM_BLOCO + MARGEM_JOGADOR, jogador->linha * TAM_BLOCO + MARGEM_JOGADOR, LARGURA_JOGADOR, ALTURA_JOGADOR};
    jogador->score = 0;
}

void AtualizarJogador(Jogador *jogador)
{
    if (IsKeyPressed(KEY_W)) jogador->linha--;
    if (IsKeyPressed(KEY_S)) jogador->linha++;
    if (IsKeyPressed(KEY_A)) jogador->coluna--;
    if (IsKeyPressed(KEY_D)) jogador->coluna++;

    if (jogador->linha < 0) jogador->linha = 0;
    if (jogador->linha > TOTAL_LINHAS - 1) jogador->linha = TOTAL_LINHAS - 1;
    if (jogador->coluna < 0) jogador->coluna = 0;
    if (jogador->coluna > TOTAL_COLUNAS - 1) jogador->coluna = TOTAL_COLUNAS - 1;

    jogador->corpo.x = jogador->coluna * TAM_BLOCO + MARGEM_JOGADOR;
    jogador->corpo.y = jogador->linha * TAM_BLOCO + MARGEM_JOGADOR;
    jogador->score = (TOTAL_LINHAS - 1) - jogador->linha;
}

void DesenharJogador(Jogador jogador)
{
    DrawCircle((int)jogador.corpo.x + 15, (int)jogador.corpo.y + 31, 12, Fade(BLACK, 0.25f));
    DrawRectangle((int)jogador.corpo.x + 7, (int)jogador.corpo.y + 10, 16, 20, YELLOW);
    DrawCircle((int)jogador.corpo.x + 15, (int)jogador.corpo.y + 7, 8, BEIGE);
    DrawCircle((int)jogador.corpo.x + 12, (int)jogador.corpo.y + 6, 2, BLACK);
    DrawCircle((int)jogador.corpo.x + 18, (int)jogador.corpo.y + 6, 2, BLACK);
    DrawRectangle((int)jogador.corpo.x + 7, (int)jogador.corpo.y + 29, 6, 4, BLUE);
    DrawRectangle((int)jogador.corpo.x + 17, (int)jogador.corpo.y + 29, 6, 4, BLUE);
    DrawRectangleLines((int)jogador.corpo.x + 7, (int)jogador.corpo.y + 10, 16, 20, BLACK);
}
