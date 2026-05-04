#include "raylib.h"
#include "config.h"
#include "jogo.h"
#include "mapa.h"

int main(void)
{
    Jogo jogo = {0};

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Caos em Recife");
    SetTargetFPS(60);
    ChangeDirectory(GetApplicationDirectory());

    InicializarMapa();
    InicializarSpritesJogador();
    InicializarTexturaMoeda();
    InicializarTexturasObstaculo();
    IniciarJogo(&jogo);

    while (!WindowShouldClose()) {
        if (!jogo.jogoIniciado) {
            if (IsKeyPressed(KEY_ENTER)) {
                jogo.jogoIniciado = true;
            }
        } else if (IsKeyPressed(KEY_R)) {
            IniciarJogo(&jogo);
            jogo.jogoIniciado = true;
        } else if (!jogo.gameOver) {
            AtualizarJogo(&jogo);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DesenharJogo(&jogo);
        EndDrawing();
    }

    FinalizarMapa();
    FinalizarTexturasObstaculo();
    FinalizarTexturaMoeda();
    FinalizarSpritesJogador();
    CloseWindow();
    return 0;
}
