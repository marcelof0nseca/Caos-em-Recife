#include "menu.h"
#include "config.h"
#include "raylib.h"

static Texture2D fundoMenu = {0};
static Texture2D fundoPadrao = {0};
static const Rectangle botaoJogar = {290, 265, 220, 48};
static const Rectangle botaoDicas = {290, 325, 220, 48};
static const Rectangle botaoJogabilidade = {290, 385, 220, 48};

static void Texto(const char *texto, int x, int y, int tamanho, Color cor)
{
    DrawText(texto, x + 1, y + 1, tamanho, Fade(BLACK, 0.65f));
    DrawText(texto, x, y, tamanho, cor);
}

static void Fundo(Texture2D fundo)
{
    if (fundo.id != 0) {
        DrawTexturePro(fundo, (Rectangle){0, 0, (float)fundo.width, (float)fundo.height}, (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA}, (Vector2){0, 0}, 0.0f, WHITE);
    }
}

static void Botao(const Rectangle *botao, const char *texto, bool selecionado)
{
    DrawRectangleRec(*botao, Fade(selecionado ? GOLD : RAYWHITE, 0.94f));
    DrawRectangleLinesEx(*botao, 2, selecionado ? MAROON : DARKBLUE);
    Texto(texto, (int)(botao->x + (botao->width - MeasureText(texto, 24)) * 0.5f),
          (int)(botao->y + 11), 24, DARKBLUE);
}

void DesenharMenuPrincipal(int selecionado)
{
    Fundo(fundoMenu);
    Botao(&botaoJogar, "JOGAR", selecionado == 0);
    Botao(&botaoDicas, "DICAS", selecionado == 1);
    Botao(&botaoJogabilidade, "JOGABILIDADE", selecionado == 2);
}

void DesenharTelaDicas(void)
{
    Fundo(fundoPadrao);
    Texto("DICAS", 350, 120, 34, RAYWHITE);
    Texto("- Observe o movimento dos carros.", 120, 210, 22, RAYWHITE);
    Texto("- Espere a hora certa para cruzar.", 120, 245, 22, RAYWHITE);
    Texto("- No alagamento, fique nas plataformas.", 120, 280, 22, RAYWHITE);
    Texto("- ESC volta ao menu principal.", 120, 360, 22, YELLOW);
}

void DesenharTelaJogabilidade(void)
{
    Fundo(fundoPadrao);
    Texto("JOGABILIDADE", 255, 120, 34, RAYWHITE);
    Texto("WASD ou setas: mover", 120, 210, 24, RAYWHITE);
    Texto("ESPACO: atacar", 120, 245, 24, RAYWHITE);
    Texto("P: pausar", 120, 280, 24, RAYWHITE);
    Texto("R: reiniciar", 120, 315, 24, RAYWHITE);
    Texto("ESC volta ao menu principal", 120, 360, 22, YELLOW);
}

void InicializarFundosMenu(void)
{
    fundoMenu = LoadTexture("assets/cenario/imagem de fundo menup.png");
    fundoPadrao = LoadTexture("assets/cenario/imagem fundo sem titulo.png");
}

void FinalizarFundosMenu(void)
{
    if (fundoMenu.id != 0) UnloadTexture(fundoMenu);
    if (fundoPadrao.id != 0) UnloadTexture(fundoPadrao);
}

