#include "raylib.h"
#include "config.h"
#include "mapa.h"

static Texture2D texturaPista = {0};
static Texture2D texturaCalcadaCanaleta = {0};
static Texture2D texturaCalcadaSimples = {0};
static Texture2D spritesAlagamento[4] = {0};

static Texture2D CarregarTexturaMapa(const char *caminho)
{
    Texture2D textura = LoadTexture(caminho);

    if (textura.id != 0) {
        SetTextureFilter(textura, TEXTURE_FILTER_POINT);
    }

    return textura;
}

static void DescarregarTexturaMapa(Texture2D *textura)
{
    if (textura->id != 0) {
        UnloadTexture(*textura);
        *textura = (Texture2D){0};
    }
}

void InicializarMapa(void)
{
    texturaPista = CarregarTexturaMapa("assets/cenario/pista.png");
    texturaCalcadaCanaleta = CarregarTexturaMapa("assets/cenario/calcada_canaleta.png");
    texturaCalcadaSimples = CarregarTexturaMapa("assets/cenario/calcada_simples.png");
    spritesAlagamento[0] = CarregarTexturaMapa("assets/itens/agua1.png");
    spritesAlagamento[1] = CarregarTexturaMapa("assets/itens/agua2.png");
    spritesAlagamento[2] = CarregarTexturaMapa("assets/itens/agua3.png");
    spritesAlagamento[3] = CarregarTexturaMapa("assets/itens/agua4.png");

}

void FinalizarMapa(void)
{
    DescarregarTexturaMapa(&texturaPista);
    DescarregarTexturaMapa(&texturaCalcadaCanaleta);
    DescarregarTexturaMapa(&texturaCalcadaSimples);

    for (int i = 0; i < 4; i++) {
        DescarregarTexturaMapa(&spritesAlagamento[i]);
    }

}

static void DesenharPista(int linha)
{
    if (texturaPista.id != 0) {
        /* Pego so a parte da imagem que parece pista. */
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
}

static void DesenharAlagamento(int linha)
{
    int y = linha * TAM_BLOCO;
    float tempo = (float)GetTime();
    /* Vai trocando entre os 4 pngs para a agua parecer animada. */
    Texture2D sprite = spritesAlagamento[((int)(tempo * 6.0f) + linha) % 4];

    if (sprite.id != 0) {
        Rectangle origem = {0, 0, (float)sprite.width, (float)sprite.height};

        for (int x = 0; x < LARGURA_TELA; x += TAM_BLOCO * 2) {
            Rectangle destino = {x, y, TAM_BLOCO * 2, TAM_BLOCO};
            DrawTexturePro(sprite, origem, destino, (Vector2){0, 0}, 0, WHITE);
        }

        return;
    }
}

static bool LinhaEhCalcadaIsolada(int linha)
{
    bool temRuaAcima = linha == 0 || LinhaEhRua(linha - 1);
    bool temRuaAbaixo = linha == TOTAL_LINHAS - 1 || LinhaEhRua(linha + 1);

    /* Quando a calcada fica entre duas ruas, uso a textura com canaleta. */
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
}

void DesenharMapa(void)
{
    for (int linha = 0; linha < TOTAL_LINHAS; linha++) {
        if (LinhaEhAlagamento(linha)) {
            DesenharAlagamento(linha);
            continue;
        }

        if (LinhaEhRua(linha)) {
            DesenharPista(linha);
            continue;
        }

        DesenharCalcada(linha);
    }
}

bool LinhaEhRua(int linha)
{
    /* A parte nova do mapa usa linhas antigas deslocadas pela fase 3. */
    int linhaAntiga = linha - LINHAS_FASE_3;

    if (LinhaEhAlagamento(linha)) {
        return false;
    }

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

bool LinhaEhAlagamento(int linha)
{
    /* Alagamento aparece so na fase 4, em linhas separadas. */
    return linha == 1 || linha == 4 || linha == 7 ||
           linha == 10 || linha == 13;
}
