#include <time.h>

#include "raylib.h"
#include "config.h"
#include "debug_mode.h"
#include "menu.h"
#include "jogo.h"
#include "mapa.h"

typedef enum { TELA_MENU, TELA_DICAS, TELA_JOGABILIDADE, TELA_JOGO } TelaApp;

static Rectangle CalcularDestinoTela(void)
{
    float escalaX = (float)GetScreenWidth() / LARGURA_TELA;
    float escalaY = (float)GetScreenHeight() / ALTURA_TELA;
    float escala = escalaX < escalaY ? escalaX : escalaY;
    float largura = LARGURA_TELA * escala;
    float altura = ALTURA_TELA * escala;

    return (Rectangle){
        (GetScreenWidth() - largura) * 0.5f,
        (GetScreenHeight() - altura) * 0.5f,
        largura,
        altura
    };
}

static void AlternarTelaCheia(void)
{
    ToggleFullscreen();
}

static void IniciarPartida(Jogo *jogo, TelaApp *tela)
{
    IniciarJogo(jogo);
    jogo->jogoIniciado = true;
    *tela = TELA_JOGO;
}

int main(void)
{
    Jogo jogo = {0};
    RenderTexture2D telaVirtual;
    TelaApp telaAtual = TELA_MENU;
    int menuSelecionado = 0;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Caos em Recife");
    SetExitKey(0);
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));
    ChangeDirectory(GetApplicationDirectory());
    telaVirtual = LoadRenderTexture(LARGURA_TELA, ALTURA_TELA);
    SetTextureFilter(telaVirtual.texture, TEXTURE_FILTER_POINT);

    InicializarFundosMenu();
    InicializarMapa();
    InicializarSpritesJogador();
    InicializarTexturasObstaculo();
    IniciarJogo(&jogo);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) {
            AlternarTelaCheia();
        }

        if (IsKeyPressed(KEY_F10)) {
            DebugAlternarInvencibilidade();
        }

        if (telaAtual == TELA_MENU) {
            jogo.jogoIniciado = false;
            if (IsKeyPressed(KEY_DOWN)) menuSelecionado = (menuSelecionado + 1) % 3;
            if (IsKeyPressed(KEY_UP)) menuSelecionado = (menuSelecionado + 2) % 3;
            if (IsKeyPressed(KEY_ENTER)) {
                if (menuSelecionado == 0) IniciarPartida(&jogo, &telaAtual);
                else telaAtual = menuSelecionado == 1 ? TELA_DICAS : TELA_JOGABILIDADE;
            }
        } else if (telaAtual == TELA_DICAS || telaAtual == TELA_JOGABILIDADE) {
            jogo.jogoIniciado = false;
            if (IsKeyPressed(KEY_ESCAPE)) telaAtual = TELA_MENU;
        } else if (IsKeyPressed(KEY_R)) {
            IniciarPartida(&jogo, &telaAtual);
        } else if (!jogo.gameOver) {
            AtualizarJogo(&jogo);
        }

        BeginTextureMode(telaVirtual);
        ClearBackground(RAYWHITE);
        if (telaAtual == TELA_MENU) DesenharMenuPrincipal(menuSelecionado);
        else if (telaAtual == TELA_DICAS) DesenharTelaDicas();
        else if (telaAtual == TELA_JOGABILIDADE) DesenharTelaJogabilidade();
        else DesenharJogo(&jogo);

        if (DebugInvencibilidadeAtiva() && telaAtual == TELA_JOGO) {
            DrawText("DEBUG: INVENCIVEL", 520, 8, 16, YELLOW);
        }
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            telaVirtual.texture,
            (Rectangle){0, 0, LARGURA_TELA, -ALTURA_TELA},
            CalcularDestinoTela(),
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        EndDrawing();
    }

    UnloadRenderTexture(telaVirtual);
    FinalizarFundosMenu();
    FinalizarMapa();
    FinalizarTexturasObstaculo();
    FinalizarSpritesJogador();
    CloseWindow();
    return 0;
}
