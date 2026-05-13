#include "menu.h"
#include "config.h"
#include "raylib.h"

static Texture2D fundoMenu = {0}, fundoPadrao = {0};

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
    Texto(texto, (int)(botao->x + (botao->width - MeasureText(texto, 24)) * 0.5f), (int)(botao->y + 11), 24, DARKBLUE);
}

static void DesenharLista(const char **textos, int total, int x, int y, int tamanho, Color cor)
{
    for (int i = 0; i < total; i++) Texto(textos[i], x, y + i * 35, tamanho, cor);
}

void DesenharMenuPrincipal(int selecionado)
{
    const char *opcoes[] = {"JOGAR", "DICAS", "JOGABILIDADE"};
    Rectangle botao = {290, 265, 220, 48};

    Fundo(fundoMenu);

    for (int i = 0; i < 3; i++) {
        botao.y = 265 + i * 60;
        Botao(&botao, opcoes[i], selecionado == i);
    }
}

void DesenharTelaDicas(void)
{
    const char *dicas[] = {"- Observe o movimento dos carros.", "- Espere a hora certa para cruzar.", "- No alagamento, fique nas plataformas."};

    Fundo(fundoPadrao);
    Texto("DICAS", 350, 120, 34, RAYWHITE);
    DesenharLista(dicas, 3, 120, 210, 22, RAYWHITE);
    Texto("- ESC volta ao menu principal.", 120, 360, 22, YELLOW);
}

void DesenharTelaJogabilidade(void)
{
    const char *comandos[] = {"WASD ou setas: mover", "ESPACO: atacar", "P: pausar", "R: reiniciar"};

    Fundo(fundoPadrao);
    Texto("JOGABILIDADE", 255, 120, 34, RAYWHITE);
    DesenharLista(comandos, 4, 120, 210, 24, RAYWHITE);
    Texto("ESC volta ao menu principal", 120, 360, 22, YELLOW);
}

void InicializarFundosMenu(void) { fundoMenu = LoadTexture("assets/cenario/imagem de fundo menup.png"); fundoPadrao = LoadTexture("assets/cenario/imagem fundo sem titulo.png"); }

void FinalizarFundosMenu(void) { if (fundoMenu.id != 0) { UnloadTexture(fundoMenu); } if (fundoPadrao.id != 0) { UnloadTexture(fundoPadrao); } }

