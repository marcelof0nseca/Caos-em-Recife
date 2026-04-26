#include "jogo.h"
#include "mapa.h"
#include "config.h"

void IniciarJogo(Jogo *jogo)
{
    IniciarJogador(&jogo->jogador);
    IniciarCarro(&jogo->carro);
    jogo->gameOver = false;
}

void AtualizarJogo(Jogo *jogo)
{
    AtualizarJogador(&jogo->jogador);
    AtualizarCarro(&jogo->carro);

    if (CheckCollisionRecs(jogo->jogador.corpo, jogo->carro.corpo)) {
        jogo->gameOver = true;
    }
}

void DesenharJogo(Jogo *jogo)
{
    DesenharMapa();
    DesenharCarro(jogo->carro);
    DesenharJogador(jogo->jogador);

    DrawText(TextFormat("Score: %d", jogo->jogador.score), 10, 10, 20, BLACK);

    if (jogo->gameOver) {
        DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.6f));
        DrawText("GAME OVER", 270, 250, 40, RED);
        DrawText("Pressione R para reiniciar", 245, 310, 24, WHITE);
    }
}
