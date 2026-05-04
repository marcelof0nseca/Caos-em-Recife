#include "moeda.h"
#include "config.h"

static Texture2D texturaMoeda = {0};

static Texture2D CarregarTexturaMoedaArquivo(const char *caminho)
{
    Texture2D textura = {0};
    Image imagem = LoadImage(caminho);

    if (imagem.data == 0) {
        return textura;
    }

    textura = LoadTextureFromImage(imagem);
    UnloadImage(imagem);

    if (textura.id != 0) {
        SetTextureFilter(textura, TEXTURE_FILTER_POINT);
    }

    return textura;
}

void InicializarTexturaMoeda(void)
{
    texturaMoeda = CarregarTexturaMoedaArquivo("assets/itens/moeda.png");
}

void FinalizarTexturaMoeda(void)
{
    if (texturaMoeda.id != 0) {
        UnloadTexture(texturaMoeda);
        texturaMoeda = (Texture2D){0};
    }
}

void IniciarMoeda(Moeda *moeda, int coluna, int linha)
{
    moeda->corpo = (Rectangle){
        coluna * TAM_BLOCO + 12,
        linha * TAM_BLOCO + 12,
        16,
        16
    };
    moeda->coletada = false;
    moeda->valor = 1;
}

void DesenharMoeda(Moeda moeda)
{
    if (moeda.coletada) {
        return;
    }

    if (texturaMoeda.id != 0) {
        Rectangle origem = {0, 0, (float)texturaMoeda.width, (float)texturaMoeda.height};
        Rectangle destino = {
            moeda.corpo.x - 1,
            moeda.corpo.y - 1,
            moeda.corpo.width + 2,
            moeda.corpo.height + 2
        };

        DrawTexturePro(texturaMoeda, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
        return;
    }

    DrawCircle((int)moeda.corpo.x + 8, (int)moeda.corpo.y + 8, 8, GOLD);
    DrawCircleLines((int)moeda.corpo.x + 8, (int)moeda.corpo.y + 8, 8, ORANGE);
    DrawCircle((int)moeda.corpo.x + 8, (int)moeda.corpo.y + 8, 3, YELLOW);
}

bool ColetarMoeda(Moeda *moeda, Rectangle jogador)
{
    if (moeda->coletada || !CheckCollisionRecs(moeda->corpo, jogador)) {
        return false;
    }

    moeda->coletada = true;
    return true;
}
