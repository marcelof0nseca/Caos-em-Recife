#include "jogo.h"
#include "mapa.h"
#include "config.h"

void IniciarJogo(Jogo *jogo)
{
    IniciarJogador(&jogo->jogador);
    IniciarCarro(&jogo->carro);
    IniciarCarroComDados(&jogo->carro2, LARGURA_TELA + 80, 360, 130, -1);
    jogo->gameOver = false;
    jogo->jogoIniciado = false;
    jogo->venceu = false;
    jogo->pausado = false;
}

void AtualizarJogo(Jogo *jogo)
{
    if (IsKeyPressed(KEY_P)) {
        jogo->pausado = !jogo->pausado;
    }

    if (jogo->pausado) {
        return;
    }

    AtualizarJogador(&jogo->jogador);
    AtualizarCarro(&jogo->carro);
    AtualizarCarro(&jogo->carro2);

    if (VerificarColisaoCarro(jogo->carro, jogo->jogador.corpo) || VerificarColisaoCarro(jogo->carro2, jogo->jogador.corpo)) {
        VoltarJogadorCheckpoint(&jogo->jogador);
    }

    if (jogo->jogador.linha == 0) {
        jogo->venceu = true;
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
    DesenharCarro(jogo->carro2);
    DesenharJogador(jogo->jogador);

    DesenharInterface(jogo);

    if (jogo->gameOver) {
        DesenharGameOver(jogo);
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
    DrawRectangle(0, 0, LARGURA_TELA, 82, Fade(RAYWHITE, 0.75f));
    DrawText(TextFormat("Score: %d", jogo->jogador.score), X_INTERFACE, Y_SCORE, 20, BLACK);
    DrawText("WASD para mover", X_INTERFACE, Y_DICA_MOVIMENTO, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("R para reiniciar", X_INTERFACE, Y_DICA_REINICIO, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("P para pausar", 220, Y_DICA_REINICIO, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("Evite o carro vermelho", 575, 10, TAM_TEXTO_INTERFACE, BLACK);

    if (jogo->pausado) {
        DrawText("PAUSADO", 335, 285, 34, YELLOW);
    }
}

void DesenharGameOver(Jogo *jogo)
{
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.6f));
    if (jogo->venceu) {
        DrawText("VOCE VENCEU", 250, 250, 40, YELLOW);
    } else {
        DrawText("GAME OVER", 270, 250, 40, RED);
    }
    DrawText("Pressione R para reiniciar", 245, 310, 24, WHITE);
}
