#include "jogador.h"
#include "config.h"
#include <stdio.h>

static Texture2D spritesIdle[4] = {0};
static Texture2D spritesAndando[4][3] = {0};
static Texture2D spritesMorrendo[4][2] = {0};

#define LARGURA_MAXIMA_SPRITE_JOGADOR 38.0f
#define ALTURA_MAXIMA_SPRITE_JOGADOR 58.0f
#define LARGURA_MAXIMA_SPRITE_MORTE 42.0f
#define ALTURA_MAXIMA_SPRITE_MORTE 24.0f

static const char *nomesDirecao[] = {"frente", "direita", "esquerda", "costas"};

static Texture2D CarregarTexturaSprite(const char *caminho)
{
    Texture2D textura = LoadTexture(caminho);

    if (textura.id != 0) {
        SetTextureFilter(textura, TEXTURE_FILTER_POINT);
    }

    return textura;
}

static void CarregarSprite(Texture2D *sprite, const char *modelo, const char *direcao, int frame)
{
    char caminho[100];
    snprintf(caminho, sizeof(caminho), modelo, direcao, frame);
    *sprite = CarregarTexturaSprite(caminho);
}

static void DescarregarSeCarregada(Texture2D *textura)
{
    if (textura->id != 0) {
        UnloadTexture(*textura);
        *textura = (Texture2D){0};
    }
}

static Texture2D *ObterSpriteAtual(Jogador jogador, bool derrotado)
{
    double agora = GetTime();
    int direcao = (int)jogador.direcao;

    /* Prioridade: morte, andando e por ultimo parado. */
    if (derrotado) {
        int frameMorte = (agora - jogador.tempoInicioMorte) >= 0.25 ? 1 : 0;
        return &spritesMorrendo[direcao][frameMorte];
    }

    if ((agora - jogador.tempoUltimoMovimento) < 0.30) {
        /* Cada passo fica em um frame por 0.10 segundo. */
        int frameAndando = (int)((agora - jogador.tempoUltimoMovimento) / 0.10);
        if (frameAndando > 2) {
            frameAndando = 2;
        }
        return &spritesAndando[direcao][frameAndando];
    }

    return &spritesIdle[direcao];
}

void InicializarSpritesJogador(void)
{
    for (int direcao = 0; direcao < 4; direcao++) {
        CarregarSprite(&spritesIdle[direcao], "assets/personagem/idle_%s.png", nomesDirecao[direcao], 0);

        for (int frame = 0; frame < 3; frame++) {
            CarregarSprite(&spritesAndando[direcao][frame], "assets/personagem/andando_%s_%d.png",
                           nomesDirecao[direcao], frame + 1);
        }

        for (int frame = 0; frame < 2; frame++) {
            CarregarSprite(&spritesMorrendo[direcao][frame], "assets/personagem/morrendo_%s_%d.png",
                           nomesDirecao[direcao], frame + 1);
        }
    }
}

void FinalizarSpritesJogador(void)
{
    for (int direcao = 0; direcao < 4; direcao++) {
        DescarregarSeCarregada(&spritesIdle[direcao]);

        for (int frame = 0; frame < 3; frame++) {
            DescarregarSeCarregada(&spritesAndando[direcao][frame]);
        }

        for (int frame = 0; frame < 2; frame++) {
            DescarregarSeCarregada(&spritesMorrendo[direcao][frame]);
        }
    }
}

void IniciarJogador(Jogador *jogador)
{
    jogador->linha = LINHA_INICIAL_JOGADOR;
    jogador->coluna = COLUNA_INICIAL_JOGADOR;
    jogador->corpo = (Rectangle){jogador->coluna * TAM_BLOCO + MARGEM_JOGADOR, jogador->linha * TAM_BLOCO + MARGEM_JOGADOR, LARGURA_JOGADOR, ALTURA_JOGADOR};
    jogador->score = 0;
    jogador->melhorLinha = jogador->linha;
    jogador->direcao = DIRECAO_FRENTE;
    jogador->tempoUltimoMovimento = -10.0;
    jogador->tempoInicioMorte = -10.0;
}

static bool TentarMover(Jogador *jogador, int tecla1, int tecla2, int linha, int coluna, DirecaoJogador direcao)
{
    if (!IsKeyPressed(tecla1) && !IsKeyPressed(tecla2)) {
        return false;
    }

    jogador->linha += linha;
    jogador->coluna += coluna;
    jogador->direcao = direcao;
    return true;
}

void AtualizarJogador(Jogador *jogador)
{
    bool moveu = TentarMover(jogador, KEY_W, KEY_UP, -1, 0, DIRECAO_COSTAS);
    moveu = TentarMover(jogador, KEY_S, KEY_DOWN, 1, 0, DIRECAO_FRENTE) || moveu;
    moveu = TentarMover(jogador, KEY_A, KEY_LEFT, 0, -1, DIRECAO_ESQUERDA) || moveu;
    moveu = TentarMover(jogador, KEY_D, KEY_RIGHT, 0, 1, DIRECAO_DIREITA) || moveu;

    if (moveu) {
        jogador->tempoUltimoMovimento = GetTime();
    }

    if (moveu) {
        /* Nao deixa o jogador sair dos limites do mapa. */
        jogador->linha = jogador->linha < 0 ? 0 : jogador->linha;
        jogador->linha = jogador->linha > TOTAL_LINHAS - 1 ? TOTAL_LINHAS - 1 : jogador->linha;
        jogador->coluna = jogador->coluna < 0 ? 0 : jogador->coluna;
        jogador->coluna = jogador->coluna > TOTAL_COLUNAS - 1 ? TOTAL_COLUNAS - 1 : jogador->coluna;

        jogador->corpo.x = jogador->coluna * TAM_BLOCO + MARGEM_JOGADOR;
        jogador->corpo.y = jogador->linha * TAM_BLOCO + MARGEM_JOGADOR;
    }
    if (jogador->linha < jogador->melhorLinha) {
        /* O score aumenta quando o jogador chega mais longe. */
        jogador->melhorLinha = jogador->linha;
        jogador->score = (TOTAL_LINHAS - 1) - jogador->melhorLinha;
    }

}

void DesenharJogador(Jogador jogador, bool derrotado)
{
    Texture2D *sprite = ObterSpriteAtual(jogador, derrotado);

    if (sprite->id == 0) {
        return;
    }

    {
        Rectangle origem = {0, 0, (float)sprite->width, (float)sprite->height};
        float larguraMaxima = derrotado ? LARGURA_MAXIMA_SPRITE_MORTE : LARGURA_MAXIMA_SPRITE_JOGADOR;
        float alturaMaxima = derrotado ? ALTURA_MAXIMA_SPRITE_MORTE : ALTURA_MAXIMA_SPRITE_JOGADOR;
        /* Mantem a proporcao do png dentro do tamanho maximo. */
        float escalaLargura = larguraMaxima / (float)sprite->width;
        float escalaAltura = alturaMaxima / (float)sprite->height;
        float escala = escalaLargura < escalaAltura ? escalaLargura : escalaAltura;
        float larguraDestino = (float)sprite->width * escala;
        float alturaDestino = (float)sprite->height * escala;
        Rectangle destino = {
            jogador.corpo.x + jogador.corpo.width * 0.5f - larguraDestino * 0.5f,
            derrotado ? jogador.corpo.y + jogador.corpo.height - alturaDestino * 0.75f
                     : jogador.corpo.y + jogador.corpo.height - alturaDestino,
            larguraDestino,
            alturaDestino
        };

        DrawTexturePro(*sprite, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
    }
}
