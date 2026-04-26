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
    DrawRectangleRec(jogador.corpo, YELLOW);
    DrawRectangleLinesEx(jogador.corpo, 2, BLACK);
}
