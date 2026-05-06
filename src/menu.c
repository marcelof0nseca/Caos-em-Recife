#include "menu.h"
#include "config.h"
#include "raylib.h"

static const Rectangle botaoJogar = {120, 225, 220, 48};
static const Rectangle botaoDicas = {120, 285, 220, 48};
static const Rectangle botaoJogabilidade = {120, 345, 220, 48};

static void Texto(const char *texto, int x, int y, int tamanho, Color cor)
{
    DrawText(texto, x + 1, y + 1, tamanho, Fade(BLACK, 0.65f));
    DrawText(texto, x, y, tamanho, cor);
}

static bool Clicou(Rectangle botao)
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), botao);
}

void DesenharMenuPrincipal(void)
{
    ClearBackground(DARKGREEN);
    Texto("CAOS EM RECIFE", 235, 70, 42, RAYWHITE);
    DrawRectangleRec(botaoJogar, Fade(RAYWHITE, 0.94f));
    DrawRectangleRec(botaoDicas, Fade(RAYWHITE, 0.94f));
    DrawRectangleRec(botaoJogabilidade, Fade(RAYWHITE, 0.94f));
    DrawRectangleLinesEx(botaoJogar, 2, DARKBLUE);
    DrawRectangleLinesEx(botaoDicas, 2, DARKBLUE);
    DrawRectangleLinesEx(botaoJogabilidade, 2, DARKBLUE);
    Texto("JOGAR", 205, 236, 24, DARKBLUE);
    Texto("DICAS", 205, 296, 24, DARKBLUE);
    Texto("JOGABILIDADE", 175, 356, 22, DARKBLUE);
}

void DesenharTelaDicas(void)
{
    ClearBackground(DARKBLUE);
    Texto("DICAS", 350, 120, 34, RAYWHITE);
    Texto("- Observe o movimento dos carros.", 120, 210, 22, RAYWHITE);
    Texto("- Espere a hora certa para cruzar.", 120, 245, 22, RAYWHITE);
    Texto("- No alagamento, fique nas plataformas.", 120, 280, 22, RAYWHITE);
    Texto("- ESC volta ao menu principal.", 120, 360, 22, YELLOW);
}

void DesenharTelaJogabilidade(void)
{
    ClearBackground(DARKPURPLE);
    Texto("JOGABILIDADE", 255, 120, 34, RAYWHITE);
    Texto("WASD ou setas: mover", 120, 210, 24, RAYWHITE);
    Texto("ESPACO: atacar", 120, 245, 24, RAYWHITE);
    Texto("P: pausar", 120, 280, 24, RAYWHITE);
    Texto("R: reiniciar", 120, 315, 24, RAYWHITE);
    Texto("ESC volta ao menu principal", 120, 360, 22, YELLOW);
}

bool MenuClicouJogar(void) { return Clicou(botaoJogar); }
bool MenuClicouDicas(void) { return Clicou(botaoDicas); }
bool MenuClicouJogabilidade(void) { return Clicou(botaoJogabilidade); }
