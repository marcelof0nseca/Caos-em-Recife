#include "jogador.h"
#include "config.h"

void IniciarJogador(Jogador *jogador)
{
    jogador->linha = 13;
    jogador->coluna = 10;
    jogador->corpo = (Rectangle){400, 520, 30, 30};
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

    jogador->corpo.x = jogador->coluna * TAM_BLOCO + 5;
    jogador->corpo.y = jogador->linha * TAM_BLOCO + 5;
    jogador->score = (TOTAL_LINHAS - 1) - jogador->linha;
}

void DesenharJogador(Jogador jogador)
{
    DrawRectangle((int)jogador.corpo.x + 7, (int)jogador.corpo.y + 10, 16, 20, YELLOW);
    DrawCircle((int)jogador.corpo.x + 15, (int)jogador.corpo.y + 7, 8, BEIGE);
    DrawCircle((int)jogador.corpo.x + 12, (int)jogador.corpo.y + 6, 2, BLACK);
    DrawCircle((int)jogador.corpo.x + 18, (int)jogador.corpo.y + 6, 2, BLACK);
    DrawRectangleLines((int)jogador.corpo.x + 7, (int)jogador.corpo.y + 10, 16, 20, BLACK);
}
