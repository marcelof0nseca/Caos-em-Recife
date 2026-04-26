#include "raylib.h"
#include "config.h"
#include "jogo.h"

int main(void)
{
    Jogo jogo;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Caos em Recife");
    SetTargetFPS(60);

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

    CloseWindow();
    return 0;
}
