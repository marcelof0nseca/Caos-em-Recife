#include "jogador.h"
#include "config.h"

static Texture2D spritesIdle[4] = {0};
static Texture2D spritesAndando[4][3] = {0};
static Texture2D spritesAtacando[4] = {0};
static Texture2D spritesMorrendo[4][2] = {0};

#define LARGURA_MAXIMA_SPRITE_JOGADOR 38.0f
#define ALTURA_MAXIMA_SPRITE_JOGADOR 58.0f
#define LARGURA_MAXIMA_SPRITE_MORTE 42.0f
#define ALTURA_MAXIMA_SPRITE_MORTE 24.0f

static Texture2D CarregarTexturaSprite(const char *caminho)
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

static void DesenharJogadorFallback(Jogador jogador)
{
    DrawCircle((int)jogador.corpo.x + 15, (int)jogador.corpo.y + 31, 12, Fade(BLACK, 0.25f));
    DrawRectangle((int)jogador.corpo.x + 7, (int)jogador.corpo.y + 10, 16, 20, YELLOW);
    DrawCircle((int)jogador.corpo.x + 15, (int)jogador.corpo.y + 7, 8, BEIGE);
    DrawCircle((int)jogador.corpo.x + 12, (int)jogador.corpo.y + 6, 2, BLACK);
    DrawCircle((int)jogador.corpo.x + 18, (int)jogador.corpo.y + 6, 2, BLACK);
    DrawRectangle((int)jogador.corpo.x + 7, (int)jogador.corpo.y + 29, 6, 4, BLUE);
    DrawRectangle((int)jogador.corpo.x + 17, (int)jogador.corpo.y + 29, 6, 4, BLUE);
    DrawRectangleLines((int)jogador.corpo.x + 7, (int)jogador.corpo.y + 10, 16, 20, BLACK);
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

    if (derrotado) {
        int frameMorte = (agora - jogador.tempoInicioMorte) >= 0.25 ? 1 : 0;
        return &spritesMorrendo[direcao][frameMorte];
    }

    if ((agora - jogador.tempoUltimoAtaque) < 0.18) {
        return &spritesAtacando[direcao];
    }

    if ((agora - jogador.tempoUltimoMovimento) < 0.30) {
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
    spritesIdle[DIRECAO_FRENTE] = CarregarTexturaSprite("assets/personagem/idle_frente.png");
    spritesIdle[DIRECAO_DIREITA] = CarregarTexturaSprite("assets/personagem/idle_direita.png");
    spritesIdle[DIRECAO_ESQUERDA] = CarregarTexturaSprite("assets/personagem/idle_esquerda.png");
    spritesIdle[DIRECAO_COSTAS] = CarregarTexturaSprite("assets/personagem/idle_costas.png");

    spritesAndando[DIRECAO_FRENTE][0] = CarregarTexturaSprite("assets/personagem/andando_frente_1.png");
    spritesAndando[DIRECAO_FRENTE][1] = CarregarTexturaSprite("assets/personagem/andando_frente_2.png");
    spritesAndando[DIRECAO_FRENTE][2] = CarregarTexturaSprite("assets/personagem/andando_frente_3.png");
    spritesAndando[DIRECAO_DIREITA][0] = CarregarTexturaSprite("assets/personagem/andando_direita_1.png");
    spritesAndando[DIRECAO_DIREITA][1] = CarregarTexturaSprite("assets/personagem/andando_direita_2.png");
    spritesAndando[DIRECAO_DIREITA][2] = CarregarTexturaSprite("assets/personagem/andando_direita_3.png");
    spritesAndando[DIRECAO_ESQUERDA][0] = CarregarTexturaSprite("assets/personagem/andando_esquerda_1.png");
    spritesAndando[DIRECAO_ESQUERDA][1] = CarregarTexturaSprite("assets/personagem/andando_esquerda_2.png");
    spritesAndando[DIRECAO_ESQUERDA][2] = CarregarTexturaSprite("assets/personagem/andando_esquerda_3.png");
    spritesAndando[DIRECAO_COSTAS][0] = CarregarTexturaSprite("assets/personagem/andando_costas_1.png");
    spritesAndando[DIRECAO_COSTAS][1] = CarregarTexturaSprite("assets/personagem/andando_costas_2.png");
    spritesAndando[DIRECAO_COSTAS][2] = CarregarTexturaSprite("assets/personagem/andando_costas_3.png");

    spritesAtacando[DIRECAO_FRENTE] = CarregarTexturaSprite("assets/personagem/atacando_frente.png");
    spritesAtacando[DIRECAO_DIREITA] = CarregarTexturaSprite("assets/personagem/atacando_direita.png");
    spritesAtacando[DIRECAO_ESQUERDA] = CarregarTexturaSprite("assets/personagem/atacando_esquerda.png");
    spritesAtacando[DIRECAO_COSTAS] = CarregarTexturaSprite("assets/personagem/atacando_costas.png");

    spritesMorrendo[DIRECAO_FRENTE][0] = CarregarTexturaSprite("assets/personagem/morrendo_frente_1.png");
    spritesMorrendo[DIRECAO_FRENTE][1] = CarregarTexturaSprite("assets/personagem/morrendo_frente_2.png");
    spritesMorrendo[DIRECAO_DIREITA][0] = CarregarTexturaSprite("assets/personagem/morrendo_direita_1.png");
    spritesMorrendo[DIRECAO_DIREITA][1] = CarregarTexturaSprite("assets/personagem/morrendo_direita_2.png");
    spritesMorrendo[DIRECAO_ESQUERDA][0] = CarregarTexturaSprite("assets/personagem/morrendo_esquerda_1.png");
    spritesMorrendo[DIRECAO_ESQUERDA][1] = CarregarTexturaSprite("assets/personagem/morrendo_esquerda_2.png");
    spritesMorrendo[DIRECAO_COSTAS][0] = CarregarTexturaSprite("assets/personagem/morrendo_costas_1.png");
    spritesMorrendo[DIRECAO_COSTAS][1] = CarregarTexturaSprite("assets/personagem/morrendo_costas_2.png");
}

void FinalizarSpritesJogador(void)
{
    int direcao;
    int frame;

    for (direcao = 0; direcao < 4; direcao++) {
        DescarregarSeCarregada(&spritesIdle[direcao]);
        DescarregarSeCarregada(&spritesAtacando[direcao]);

        for (frame = 0; frame < 3; frame++) {
            DescarregarSeCarregada(&spritesAndando[direcao][frame]);
        }

        for (frame = 0; frame < 2; frame++) {
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
    jogador->tempoUltimoAtaque = -10.0;
    jogador->tempoInicioMorte = -10.0;
}

void AtualizarJogador(Jogador *jogador)
{
    bool moveu = false;

    if (IsKeyPressed(KEY_W)) {
        jogador->linha--;
        jogador->direcao = DIRECAO_COSTAS;
        moveu = true;
    }
    if (IsKeyPressed(KEY_S)) {
        jogador->linha++;
        jogador->direcao = DIRECAO_FRENTE;
        moveu = true;
    }
    if (IsKeyPressed(KEY_A)) {
        jogador->coluna--;
        jogador->direcao = DIRECAO_ESQUERDA;
        moveu = true;
    }
    if (IsKeyPressed(KEY_D)) {
        jogador->coluna++;
        jogador->direcao = DIRECAO_DIREITA;
        moveu = true;
    }

    if (moveu) {
        jogador->tempoUltimoMovimento = GetTime();
    }

    if (IsKeyPressed(KEY_SPACE)) {
        jogador->tempoUltimoAtaque = GetTime();
    }

    if (jogador->linha < 0) jogador->linha = 0;
    if (jogador->linha > TOTAL_LINHAS - 1) jogador->linha = TOTAL_LINHAS - 1;
    if (jogador->coluna < 0) jogador->coluna = 0;
    if (jogador->coluna > TOTAL_COLUNAS - 1) jogador->coluna = TOTAL_COLUNAS - 1;

    jogador->corpo.x = jogador->coluna * TAM_BLOCO + MARGEM_JOGADOR;
    jogador->corpo.y = jogador->linha * TAM_BLOCO + MARGEM_JOGADOR;
    if (jogador->linha < jogador->melhorLinha) {
        jogador->melhorLinha = jogador->linha;
        jogador->score = (TOTAL_LINHAS - 1) - jogador->melhorLinha;
    }

}

void DesenharJogador(Jogador jogador, bool derrotado)
{
    Texture2D *sprite = ObterSpriteAtual(jogador, derrotado);

    if (sprite->id == 0) {
        DesenharJogadorFallback(jogador);
        return;
    }

    {
        Rectangle origem = {0, 0, (float)sprite->width, (float)sprite->height};
        float larguraMaxima = derrotado ? LARGURA_MAXIMA_SPRITE_MORTE : LARGURA_MAXIMA_SPRITE_JOGADOR;
        float alturaMaxima = derrotado ? ALTURA_MAXIMA_SPRITE_MORTE : ALTURA_MAXIMA_SPRITE_JOGADOR;
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
