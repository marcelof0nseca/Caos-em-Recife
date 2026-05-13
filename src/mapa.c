#include "raylib.h"
#include "config.h"
#include "mapa.h"

#define QUANTIDADE(lista) ((int)(sizeof(lista) / sizeof((lista)[0])))

static Texture2D texturaPista = {0};
static Texture2D texturaCalcadaCanaleta = {0};
static Texture2D texturaCalcadaSimples = {0};
static Texture2D spritesAlagamento[3] = {0};

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

static void CarregarTexturas(Texture2D *texturas, const char **caminhos, int total)
{
    for (int i = 0; i < total; i++) {
        texturas[i] = CarregarTexturaMapa(caminhos[i]);
    }
}

static void DescarregarTexturas(Texture2D *texturas, int total)
{
    for (int i = 0; i < total; i++) {
        DescarregarTexturaMapa(&texturas[i]);
    }
}

void InicializarMapa(void)
{
    const char *aguas[] = {
        "assets/itens/agua1.png",
        "assets/itens/agua2.png",
        "assets/itens/agua3.png"
    };

    texturaPista = CarregarTexturaMapa("assets/cenario/pista.png");
    texturaCalcadaCanaleta = CarregarTexturaMapa("assets/cenario/calcada_canaleta.png");
    texturaCalcadaSimples = CarregarTexturaMapa("assets/cenario/calcada_simples.png");
    CarregarTexturas(spritesAlagamento, aguas, QUANTIDADE(spritesAlagamento));
}

void FinalizarMapa(void)
{
    DescarregarTexturaMapa(&texturaPista);
    DescarregarTexturaMapa(&texturaCalcadaCanaleta);
    DescarregarTexturaMapa(&texturaCalcadaSimples);
    DescarregarTexturas(spritesAlagamento, QUANTIDADE(spritesAlagamento));
}

static void DesenharFaixa(Texture2D textura, Rectangle origem, int linha)
{
    if (textura.id == 0) {
        return;
    }

    DrawTexturePro(textura, origem, (Rectangle){0, linha * TAM_BLOCO, LARGURA_TELA, TAM_BLOCO},
                   (Vector2){0, 0}, 0, WHITE);
}

static void DesenharPista(int linha)
{
    /* Pego so a parte da imagem que parece pista. */
    Rectangle origem = {0, texturaPista.height * 0.36f, texturaPista.width, texturaPista.height * 0.31f};
    DesenharFaixa(texturaPista, origem, linha);
}

static void DesenharAlagamento(int linha)
{
    int y = linha * TAM_BLOCO;
    float tempo = (float)GetTime();
    /* Vai trocando entre os pngs bons para a agua parecer animada. */
    Texture2D sprite = spritesAlagamento[((int)(tempo * 6.0f) + linha) % QUANTIDADE(spritesAlagamento)];

    if (sprite.id != 0) {
        Rectangle origem = {0, 0, (float)sprite.width, (float)sprite.height};

        for (int x = 0; x < LARGURA_TELA; x += TAM_BLOCO) {
            Rectangle destino = {x, y, TAM_BLOCO, TAM_BLOCO};
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
    Rectangle origem = {0, 0, textura.width, textura.height};

    DesenharFaixa(textura, origem, linha);
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
    int ruasPrimeiraParte[] = {2, 3, 5, 6, 8, 9, 11, 12};
    int ruasParteNova[] = {
        2, 3, 5, 6, 8, 9, 11, 12,
        14, 15, 17, 18, 20, 21, 24, 25,
        27, 28, 30, 31, 34, 35, 37, 38, 40, 41
    };

    if (LinhaEhAlagamento(linha)) {
        return false;
    }

    for (int i = 0; i < QUANTIDADE(ruasPrimeiraParte); i++) {
        if (linha == ruasPrimeiraParte[i]) {
            return true;
        }
    }

    for (int i = 0; i < QUANTIDADE(ruasParteNova); i++) {
        if (linhaAntiga == ruasParteNova[i]) {
            return true;
        }
    }

    return false;
}

bool LinhaEhAlagamento(int linha)
{
    /* Alagamento aparece so na fase 4, em linhas separadas. */
    return linha == 1 || linha == 4 || linha == 7 ||
           linha == 10 || linha == 13;
}
