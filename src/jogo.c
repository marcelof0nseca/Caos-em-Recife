#include "jogo.h"
#include "mapa.h"
#include "config.h"

void IniciarJogo(Jogo *jogo)
{
    IniciarJogador(&jogo->jogador);
    IniciarCarro(&jogo->carro);
    jogo->gameOver = false;
    jogo->jogoIniciado = false;
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
    if (!jogo->jogoIniciado) {
        DesenharTelaInicial();
        return;
    }

    DesenharMapa();
    DesenharCarro(jogo->carro);
    DesenharJogador(jogo->jogador);

    DesenharInterface(jogo);

    if (jogo->gameOver) {
        DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.6f));
        DrawText("GAME OVER", 270, 250, 40, RED);
        DrawText("Pressione R para reiniciar", 245, 310, 24, WHITE);
    }
}

void DesenharTelaInicial(void)
{
    ClearBackground(DARKGREEN);
    DrawText("CAOS EM RECIFE", 235, 220, 40, RAYWHITE);
    DrawText("Pressione ENTER para jogar", 245, 290, 24, YELLOW);
}

void DesenharInterface(Jogo *jogo)
{
    DrawText(TextFormat("Score: %d", jogo->jogador.score), 10, 10, 20, BLACK);
    DrawText("WASD para mover", 10, 35, 18, BLACK);
    DrawText("R para reiniciar", 10, 58, 18, BLACK);
}
