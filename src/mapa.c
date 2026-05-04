#include "raylib.h"
#include "config.h"
#include "mapa.h"

static Texture2D texturaPista = {0};
static Texture2D texturaCalcadaCanaleta = {0};
static Texture2D texturaCalcadaSimples = {0};
static Texture2D texturaPoste = {0};

void InicializarMapa(void)
{
    texturaPista = LoadTexture("assets/pista.png");
    texturaCalcadaCanaleta = LoadTexture("assets/calcada_canaleta.png");
    texturaCalcadaSimples = LoadTexture("assets/calcada_simples.png");
    texturaPoste = LoadTexture("assets/cenario/poste.png");

    if (texturaCalcadaCanaleta.id != 0) {
        SetTextureFilter(texturaCalcadaCanaleta, TEXTURE_FILTER_POINT);
    }

    if (texturaCalcadaSimples.id != 0) {
        SetTextureFilter(texturaCalcadaSimples, TEXTURE_FILTER_POINT);
    }

    if (texturaPoste.id != 0) {
        SetTextureFilter(texturaPoste, TEXTURE_FILTER_POINT);
    }
}

void FinalizarMapa(void)
{
    if (texturaPista.id != 0) {
        UnloadTexture(texturaPista);
    }

    if (texturaCalcadaCanaleta.id != 0) {
        UnloadTexture(texturaCalcadaCanaleta);
    }

    if (texturaCalcadaSimples.id != 0) {
        UnloadTexture(texturaCalcadaSimples);
    }

    if (texturaPoste.id != 0) {
        UnloadTexture(texturaPoste);
    }
}

static void DesenharPista(int linha)
{
    if (texturaPista.id != 0) {
        Rectangle origem = {
            0,
            texturaPista.height * 0.36f,
            texturaPista.width,
            texturaPista.height * 0.31f
        };
        Rectangle destino = {
            0,
            linha * TAM_BLOCO,
            LARGURA_TELA,
            TAM_BLOCO
        };

        DrawTexturePro(texturaPista, origem, destino, (Vector2){0, 0}, 0, WHITE);
        return;
    }

    DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, TAM_BLOCO, DARKGRAY);
    DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, ESPESSURA_BORDA_RUA, BLACK);
    DrawRectangle(0, linha * TAM_BLOCO + TAM_BLOCO - ESPESSURA_BORDA_RUA, LARGURA_TELA, ESPESSURA_BORDA_RUA, BLACK);
    DrawLine(0, linha * TAM_BLOCO + 20, LARGURA_TELA, linha * TAM_BLOCO + 20, YELLOW);
}

static bool LinhaEhCalcadaIsolada(int linha)
{
    bool temRuaAcima = linha == 0 || LinhaEhRua(linha - 1);
    bool temRuaAbaixo = linha == TOTAL_LINHAS - 1 || LinhaEhRua(linha + 1);

    return temRuaAcima && temRuaAbaixo;
}

static void DesenharCalcada(int linha)
{
    Texture2D textura = LinhaEhCalcadaIsolada(linha) ? texturaCalcadaCanaleta : texturaCalcadaSimples;

    if (textura.id != 0) {
        Rectangle origem = {
            0,
            0,
            textura.width,
            textura.height
        };
        Rectangle destino = {
            0,
            linha * TAM_BLOCO,
            LARGURA_TELA,
            TAM_BLOCO
        };

        DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0, WHITE);
        return;
    }

    DrawRectangle(0, linha * TAM_BLOCO, LARGURA_TELA, TAM_BLOCO, LIGHTGRAY);

    for (int coluna = 0; coluna < TOTAL_COLUNAS; coluna++) {
        DrawRectangleLines(coluna * TAM_BLOCO, linha * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, GRAY);
    }
}

static bool LinhaEhFimFaixaCalcada(int linha)
{
    if (LinhaEhRua(linha)) {
        return false;
    }

    return linha == TOTAL_LINHAS - 1 || LinhaEhRua(linha + 1);
}

static int ObterIndiceFaixaCalcada(int linhaFinal)
{
    int indice = 0;

    for (int linha = 0; linha < linhaFinal; linha++) {
        if (LinhaEhFimFaixaCalcada(linha)) {
            indice++;
        }
    }

    return indice;
}

static void DesenharPoste(int coluna, int linha, int variante)
{
    if (texturaPoste.id != 0) {
        bool espelhar = variante % 2 == 1;
        float largura = 17.0f + (variante % 3);
        float altura = 66.0f + (variante % 4) * 3.0f;
        float deslocamentoX = (float)((variante * 7) % 13) - 6.0f;
        Rectangle origem = {
            espelhar ? (float)texturaPoste.width : 0,
            0,
            espelhar ? -(float)texturaPoste.width : (float)texturaPoste.width,
            texturaPoste.height
        };
        Rectangle destino = {
            coluna * TAM_BLOCO + 12 + deslocamentoX,
            linha * TAM_BLOCO + TAM_BLOCO - altura,
            largura,
            altura
        };

        DrawTexturePro(texturaPoste, origem, destino, (Vector2){0, 0}, 0, WHITE);
        return;
    }

    DrawRectangle(coluna * TAM_BLOCO + 18, linha * TAM_BLOCO - 28, 6, 60, DARKGRAY);
    DrawRectangle(coluna * TAM_BLOCO + 13, linha * TAM_BLOCO + 29, 16, 4, DARKBROWN);
}

static void DesenharPostesMapa(void)
{
    static const int padroes[][4] = {
        {2, 9, 16, -1},
        {5, 13, -1, -1},
        {1, 7, 12, 18},
        {4, 10, 17, -1},
        {3, 15, -1, -1},
        {6, 11, 18, -1},
        {2, 8, 14, -1},
        {5, 16, -1, -1}
    };
    int totalPadroes = sizeof(padroes) / sizeof(padroes[0]);

    for (int linha = 0; linha < TOTAL_LINHAS; linha++) {
        int indiceFaixa;
        int indicePadrao;

        if (!LinhaEhFimFaixaCalcada(linha)) {
            continue;
        }

        indiceFaixa = ObterIndiceFaixaCalcada(linha);
        indicePadrao = indiceFaixa % totalPadroes;

        for (int i = 0; i < 4; i++) {
            int coluna = padroes[indicePadrao][i];

            if (coluna < 0) {
                continue;
            }

            DesenharPoste(coluna, linha, indiceFaixa + i);
        }
    }
}

void DesenharMapa(void)
{
    for (int linha = 0; linha < TOTAL_LINHAS; linha++) {
        if (LinhaEhRua(linha)) {
            DesenharPista(linha);
            continue;
        }

        DesenharCalcada(linha);
    }

    DesenharPostesMapa();
}

bool LinhaEhRua(int linha)
{
    int linhaAntiga = linha - LINHAS_FASE_3;

    if (linha == 2 || linha == 3 ||
        linha == 5 || linha == 6 ||
        linha == 8 || linha == 9 ||
        linha == 11 || linha == 12) {
        return true;
    }

    return linhaAntiga == 2 || linhaAntiga == 3 ||
           linhaAntiga == 5 || linhaAntiga == 6 ||
           linhaAntiga == 8 || linhaAntiga == 9 ||
           linhaAntiga == 11 || linhaAntiga == 12 ||
           linhaAntiga == 14 || linhaAntiga == 15 ||
           linhaAntiga == 17 || linhaAntiga == 18 ||
           linhaAntiga == 20 || linhaAntiga == 21 ||
           linhaAntiga == 24 || linhaAntiga == 25 ||
           linhaAntiga == 27 || linhaAntiga == 28 ||
           linhaAntiga == 30 || linhaAntiga == 31 ||
           linhaAntiga == 34 || linhaAntiga == 35 ||
           linhaAntiga == 37 || linhaAntiga == 38 ||
           linhaAntiga == 40 || linhaAntiga == 41;
}
