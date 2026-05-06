#include "menu.h"
#include "config.h"
#include "raylib.h"

static const Rectangle botaoJogar = {290, 225, 220, 48};
static const Rectangle botaoDicas = {290, 285, 220, 48};
static const Rectangle botaoJogabilidade = {290, 345, 220, 48};

static int CentroTexto(const char *texto, int tamanho)
{
    return (LARGURA_TELA - MeasureText(texto, tamanho)) / 2;
}

static void Texto(const char *texto, int x, int y, int tamanho, Color cor)
{
    DrawText(texto, x + 1, y + 1, tamanho, Fade(BLACK, 0.65f));
    DrawText(texto, x, y, tamanho, cor);
}

static void Botao(const Rectangle *botao, const char *texto)
{
    DrawRectangleRec(*botao, Fade(RAYWHITE, 0.94f));
    DrawRectangleLinesEx(*botao, 2, DARKBLUE);
    Texto(texto, (int)(botao->x + (botao->width - MeasureText(texto, 24)) * 0.5f),
          (int)(botao->y + 11), 24, DARKBLUE);
}

static bool Clicou(Rectangle botao)
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), botao);
}

void DesenharMenuPrincipal(void)
{
    ClearBackground(DARKGREEN);
    Texto("CAOS EM RECIFE", CentroTexto("CAOS EM RECIFE", 42), 70, 42, RAYWHITE);
    Botao(&botaoJogar, "JOGAR");
    Botao(&botaoDicas, "DICAS");
    Botao(&botaoJogabilidade, "JOGABILIDADE");
}

void DesenharTelaDicas(void)
{
    ClearBackground(DARKBLUE);
    Texto("DICAS", CentroTexto("DICAS", 34), 120, 34, RAYWHITE);
    Texto("- Observe o movimento dos carros.", CentroTexto("- Observe o movimento dos carros.", 22), 210, 22, RAYWHITE);
    Texto("- Espere a hora certa para cruzar.", CentroTexto("- Espere a hora certa para cruzar.", 22), 245, 22, RAYWHITE);
    Texto("- No alagamento, fique nas plataformas.", CentroTexto("- No alagamento, fique nas plataformas.", 22), 280, 22, RAYWHITE);
    Texto("- ESC volta ao menu principal.", CentroTexto("- ESC volta ao menu principal.", 22), 360, 22, YELLOW);
}

void DesenharTelaJogabilidade(void)
{
    ClearBackground(DARKPURPLE);
    Texto("JOGABILIDADE", CentroTexto("JOGABILIDADE", 34), 120, 34, RAYWHITE);
    Texto("WASD ou setas: mover", CentroTexto("WASD ou setas: mover", 24), 210, 24, RAYWHITE);
    Texto("ESPACO: atacar", CentroTexto("ESPACO: atacar", 24), 245, 24, RAYWHITE);
    Texto("P: pausar", CentroTexto("P: pausar", 24), 280, 24, RAYWHITE);
    Texto("R: reiniciar", CentroTexto("R: reiniciar", 24), 315, 24, RAYWHITE);
    Texto("ESC volta ao menu principal", CentroTexto("ESC volta ao menu principal", 22), 360, 22, YELLOW);
}

bool MenuClicouJogar(void) { return Clicou(botaoJogar); }
bool MenuClicouDicas(void) { return Clicou(botaoDicas); }
bool MenuClicouJogabilidade(void) { return Clicou(botaoJogabilidade); }
