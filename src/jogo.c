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

    if (VerificarColisaoCarro(jogo->carro, jogo->jogador.corpo)) {
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
    DrawText("CAOS EM RECIFE", X_TITULO_INICIAL, Y_TITULO_INICIAL, 40, RAYWHITE);
    DrawText("Pressione ENTER para jogar", X_TEXTO_INICIAL, Y_TEXTO_INICIAL, 24, YELLOW);
}

void DesenharInterface(Jogo *jogo)
{
    DrawText(TextFormat("Score: %d", jogo->jogador.score), X_INTERFACE, Y_SCORE, 20, BLACK);
    DrawText("WASD para mover", X_INTERFACE, Y_DICA_MOVIMENTO, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("R para reiniciar", X_INTERFACE, Y_DICA_REINICIO, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("Evite o carro vermelho", 575, 10, TAM_TEXTO_INTERFACE, BLACK);
}
