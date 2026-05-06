#include <time.h>

#include "raylib.h"
#include "config.h"
#include "menu.h"
#include "jogo.h"
#include "mapa.h"

static bool telaCheiaSemBorda = false;
static Vector2 posicaoJanela = {0};
static int larguraJanela = LARGURA_TELA;
static int alturaJanela = ALTURA_TELA;

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
    int monitor = GetCurrentMonitor();
    Vector2 posicaoMonitor = GetMonitorPosition(monitor);

    if (telaCheiaSemBorda) {
        ClearWindowState(FLAG_WINDOW_UNDECORATED);
        SetWindowSize(larguraJanela, alturaJanela);
        SetWindowPosition((int)posicaoJanela.x, (int)posicaoJanela.y);
        telaCheiaSemBorda = false;
        return;
    }

    posicaoJanela = GetWindowPosition();
    larguraJanela = GetScreenWidth();
    alturaJanela = GetScreenHeight();
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowPosition((int)posicaoMonitor.x, (int)posicaoMonitor.y);
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    telaCheiaSemBorda = true;
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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Caos em Recife");
    SetExitKey(0);
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));
    ChangeDirectory(GetApplicationDirectory());
    telaVirtual = LoadRenderTexture(LARGURA_TELA, ALTURA_TELA);
    SetTextureFilter(telaVirtual.texture, TEXTURE_FILTER_POINT);

    InicializarMapa();
    InicializarSpritesJogador();
    InicializarTexturaMoeda();
    InicializarTexturasObstaculo();
    IniciarJogo(&jogo);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) {
            AlternarTelaCheia();
        }

        if (telaAtual == TELA_MENU) {
            jogo.jogoIniciado = false;
            if (IsKeyPressed(KEY_ENTER) || MenuClicouJogar()) {
                IniciarPartida(&jogo, &telaAtual);
            } else if (MenuClicouDicas()) {
                telaAtual = TELA_DICAS;
            } else if (MenuClicouJogabilidade()) {
                telaAtual = TELA_JOGABILIDADE;
            }
        } else if (telaAtual == TELA_DICAS || telaAtual == TELA_JOGABILIDADE) {
            jogo.jogoIniciado = false;
            if (IsKeyPressed(KEY_ESCAPE)) {
                telaAtual = TELA_MENU;
            }
        } else {
            if (IsKeyPressed(KEY_R)) {
                IniciarPartida(&jogo, &telaAtual);
            } else if (!jogo.gameOver) {
                AtualizarJogo(&jogo);
            }
        }

        BeginTextureMode(telaVirtual);
        ClearBackground(RAYWHITE);
        if (telaAtual == TELA_MENU) {
            DesenharMenuPrincipal();
        } else if (telaAtual == TELA_DICAS) {
            DesenharTelaDicas();
        } else if (telaAtual == TELA_JOGABILIDADE) {
            DesenharTelaJogabilidade();
        } else {
            DesenharJogo(&jogo);
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
    FinalizarMapa();
    FinalizarTexturasObstaculo();
    FinalizarTexturaMoeda();
    FinalizarSpritesJogador();
    CloseWindow();
    return 0;
}
