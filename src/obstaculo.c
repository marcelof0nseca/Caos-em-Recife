#include "obstaculo.h"
#include "config.h"

static Texture2D texturaCoqueiro = {0};
static Texture2D texturaGuardaSol = {0};
static Texture2D texturaGuardaChuvaFrevo = {0};
static Texture2D spritesCachorroDireita[4] = {0};
static Texture2D spritesCachorroEsquerda[4] = {0};
static Texture2D spriteCachorroMordendoDireita = {0};
static Texture2D spriteCachorroMordendoEsquerda = {0};
static Texture2D spritesBuraco[4] = {0};

static Texture2D CarregarTexturaObstaculo(const char *caminho)
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

void InicializarTexturasObstaculo(void)
{
    texturaCoqueiro = CarregarTexturaObstaculo("assets/cenario/coqueiro.png");
    texturaGuardaSol = CarregarTexturaObstaculo("assets/cenario/guarda_sol.png");
    texturaGuardaChuvaFrevo = CarregarTexturaObstaculo("assets/cenario/guarda_chuva_frevo.png");
    spritesCachorroDireita[0] = CarregarTexturaObstaculo("assets/cachorro/correndo_direita_1.png");
    spritesCachorroDireita[1] = CarregarTexturaObstaculo("assets/cachorro/correndo_direita_2.png");
    spritesCachorroDireita[2] = CarregarTexturaObstaculo("assets/cachorro/correndo_direita_3.png");
    spritesCachorroDireita[3] = CarregarTexturaObstaculo("assets/cachorro/correndo_direita_4.png");
    spritesCachorroEsquerda[0] = CarregarTexturaObstaculo("assets/cachorro/correndo_esquerda_1.png");
    spritesCachorroEsquerda[1] = CarregarTexturaObstaculo("assets/cachorro/correndo_esquerda_2.png");
    spritesCachorroEsquerda[2] = CarregarTexturaObstaculo("assets/cachorro/correndo_esquerda_3.png");
    spritesCachorroEsquerda[3] = CarregarTexturaObstaculo("assets/cachorro/correndo_esquerda_4.png");
    spriteCachorroMordendoDireita = CarregarTexturaObstaculo("assets/cachorro/mordendo_direita.png");
    spriteCachorroMordendoEsquerda = CarregarTexturaObstaculo("assets/cachorro/mordendo_esquerda.png");
    spritesBuraco[0] = CarregarTexturaObstaculo("assets/itens/buraco_1.png");
    spritesBuraco[1] = CarregarTexturaObstaculo("assets/itens/buraco_2.png");
    spritesBuraco[2] = CarregarTexturaObstaculo("assets/itens/buraco_3.png");
    spritesBuraco[3] = CarregarTexturaObstaculo("assets/itens/buraco_4.png");
}

void FinalizarTexturasObstaculo(void)
{
    int frame;

    if (texturaCoqueiro.id != 0) {
        UnloadTexture(texturaCoqueiro);
        texturaCoqueiro = (Texture2D){0};
    }

    if (texturaGuardaSol.id != 0) {
        UnloadTexture(texturaGuardaSol);
        texturaGuardaSol = (Texture2D){0};
    }

    if (texturaGuardaChuvaFrevo.id != 0) {
        UnloadTexture(texturaGuardaChuvaFrevo);
        texturaGuardaChuvaFrevo = (Texture2D){0};
    }

    for (frame = 0; frame < 4; frame++) {
        if (spritesCachorroDireita[frame].id != 0) {
            UnloadTexture(spritesCachorroDireita[frame]);
            spritesCachorroDireita[frame] = (Texture2D){0};
        }

        if (spritesCachorroEsquerda[frame].id != 0) {
            UnloadTexture(spritesCachorroEsquerda[frame]);
            spritesCachorroEsquerda[frame] = (Texture2D){0};
        }

        if (spritesBuraco[frame].id != 0) {
            UnloadTexture(spritesBuraco[frame]);
            spritesBuraco[frame] = (Texture2D){0};
        }
    }

    if (spriteCachorroMordendoDireita.id != 0) {
        UnloadTexture(spriteCachorroMordendoDireita);
        spriteCachorroMordendoDireita = (Texture2D){0};
    }

    if (spriteCachorroMordendoEsquerda.id != 0) {
        UnloadTexture(spriteCachorroMordendoEsquerda);
        spriteCachorroMordendoEsquerda = (Texture2D){0};
    }
}

void IniciarCarro(Obstaculo *carro)
{
    IniciarCarroComDados(carro, X_INICIAL_CARRO, Y_INICIAL_CARRO, VELOCIDADE_CARRO, 1);
}

void IniciarCarroComDados(Obstaculo *carro, float x, float y, float velocidade, int direcao)
{
    carro->tipo = TIPO_CARRO;
    carro->corpo = (Rectangle){x, y, LARGURA_CARRO, ALTURA_CARRO};
    carro->velocidade = velocidade;
    carro->direcao = direcao;
    carro->mordendo = false;
    carro->variante = 0;
    carro->proximo = NULL;
}

void IniciarOnibus(Obstaculo *onibus, float x, float y, int direcao)
{
    onibus->tipo = TIPO_ONIBUS;
    onibus->corpo = (Rectangle){x, y, 110, ALTURA_CARRO};
    onibus->velocidade = VELOCIDADE_CARRO * 0.65f;
    onibus->direcao = direcao;
    onibus->mordendo = false;
    onibus->variante = 0;
    onibus->proximo = NULL;
}

void IniciarMoto(Obstaculo *moto, float x, float y, int direcao)
{
    moto->tipo = TIPO_MOTO;
    moto->corpo = (Rectangle){x, y + 5, 45, 22};
    moto->velocidade = VELOCIDADE_CARRO * 1.35f;
    moto->direcao = direcao;
    moto->mordendo = false;
    moto->variante = 0;
    moto->proximo = NULL;
}

void IniciarBuraco(Obstaculo *buraco, float x, float y)
{
    int variante = ((int)(x / TAM_BLOCO) + (int)(y / TAM_BLOCO)) % 4;
    float larguras[] = {34, 42, 50, 62};
    float alturas[] = {26, 31, 37, 42};

    buraco->tipo = TIPO_BURACO;
    buraco->corpo = (Rectangle){
        x + (TAM_BLOCO - larguras[variante]) * 0.5f,
        y + (TAM_BLOCO - alturas[variante]) * 0.5f,
        larguras[variante],
        alturas[variante]
    };
    buraco->velocidade = 0;
    buraco->direcao = 0;
    buraco->mordendo = false;
    buraco->variante = variante;
    buraco->proximo = NULL;
}

void IniciarPedra(Obstaculo *pedra, float x, float y)
{
    pedra->tipo = TIPO_PEDRA;
    pedra->corpo = (Rectangle){x + 6, y + 12, 28, 22};
    pedra->velocidade = 0;
    pedra->direcao = 0;
    pedra->mordendo = false;
    pedra->variante = 0;
    pedra->proximo = NULL;
}

void IniciarArvore(Obstaculo *arvore, float x, float y)
{
    arvore->tipo = TIPO_ARVORE;
    arvore->corpo = (Rectangle){x + 5, y + 4, 30, 32};
    arvore->velocidade = 0;
    arvore->direcao = 0;
    arvore->mordendo = false;
    arvore->variante = 0;
    arvore->proximo = NULL;
}

void IniciarGuardaSol(Obstaculo *guardaSol, float x, float y)
{
    guardaSol->tipo = TIPO_GUARDA_SOL;
    guardaSol->corpo = (Rectangle){x + 7, y + 10, 26, 24};
    guardaSol->velocidade = 0;
    guardaSol->direcao = 0;
    guardaSol->mordendo = false;
    guardaSol->variante = 0;
    guardaSol->proximo = NULL;
}

void IniciarGuardaChuvaFrevo(Obstaculo *guardaChuvaFrevo, float x, float y)
{
    guardaChuvaFrevo->tipo = TIPO_GUARDA_CHUVA_FREVO;
    guardaChuvaFrevo->corpo = (Rectangle){x + 7, y + 9, 26, 25};
    guardaChuvaFrevo->velocidade = 0;
    guardaChuvaFrevo->direcao = 0;
    guardaChuvaFrevo->mordendo = false;
    guardaChuvaFrevo->variante = 0;
    guardaChuvaFrevo->proximo = NULL;
}

void IniciarCachorro(Obstaculo *cachorro, float x, float y, int direcao)
{
    cachorro->tipo = TIPO_CACHORRO;
    cachorro->corpo = (Rectangle){x, y + 2, LARGURA_CACHORRO, ALTURA_CACHORRO};
    cachorro->velocidade = VELOCIDADE_CACHORRO;
    cachorro->direcao = direcao;
    cachorro->mordendo = false;
    cachorro->variante = 0;
    cachorro->proximo = NULL;
}

Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao)
{
    Obstaculo *novo = (Obstaculo *)malloc(sizeof(Obstaculo));

    if (novo == NULL) {
        return NULL;
    }

    if (tipo == TIPO_BURACO) {
        IniciarBuraco(novo, x, y);
    } else if (tipo == TIPO_PEDRA) {
        IniciarPedra(novo, x, y);
    } else if (tipo == TIPO_ARVORE) {
        IniciarArvore(novo, x, y);
    } else if (tipo == TIPO_GUARDA_SOL) {
        IniciarGuardaSol(novo, x, y);
    } else if (tipo == TIPO_GUARDA_CHUVA_FREVO) {
        IniciarGuardaChuvaFrevo(novo, x, y);
    } else if (tipo == TIPO_MOTO) {
        IniciarMoto(novo, x, y, direcao);
    } else if (tipo == TIPO_CACHORRO) {
        IniciarCachorro(novo, x, y, direcao);
    } else if (tipo == TIPO_ONIBUS) {
        IniciarOnibus(novo, x, y, direcao);
    } else {
        IniciarCarroComDados(novo, x, y, velocidade, direcao);
    }

    return novo;
}

void AdicionarObstaculo(Obstaculo **lista, Obstaculo *novo)
{
    if (novo == NULL) {
        return;
    }

    novo->proximo = *lista;
    *lista = novo;
}

void LiberarObstaculos(Obstaculo **lista)
{
    Obstaculo *atual = *lista;

    while (atual != NULL) {
        Obstaculo *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    *lista = NULL;
}

void AtualizarCarro(Obstaculo *carro)
{
    if (carro->velocidade == 0) {
        return;
    }

    carro->corpo.x += carro->velocidade * carro->direcao * GetFrameTime();

    if (carro->direcao == 1 && carro->corpo.x > LARGURA_TELA) {
        carro->corpo.x = -carro->corpo.width - 10;
    }

    if (carro->direcao == -1 && carro->corpo.x + carro->corpo.width < 0) {
        carro->corpo.x = LARGURA_TELA + carro->corpo.width;
    }
}

void AtualizarListaObstaculos(Obstaculo *lista)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        AtualizarCarro(atual);
        atual = atual->proximo;
    }
}

static void DesenharCachorro(Obstaculo cachorro)
{
    Texture2D sprite;
    Rectangle origem;
    Rectangle destino;
    int frame = (int)(GetTime() * 10.0) % 4;

    if (cachorro.mordendo) {
        sprite = cachorro.direcao == 1 ? spriteCachorroMordendoDireita : spriteCachorroMordendoEsquerda;
    } else {
        sprite = cachorro.direcao == 1 ? spritesCachorroDireita[frame] : spritesCachorroEsquerda[frame];
    }

    if (sprite.id != 0) {
        float larguraDestino = cachorro.mordendo ? 68.0f : 64.0f;
        float alturaDestino = cachorro.mordendo ? 40.0f : 36.0f;

        origem = (Rectangle){0, 0, (float)sprite.width, (float)sprite.height};
        destino = (Rectangle){
            cachorro.corpo.x + cachorro.corpo.width * 0.5f - larguraDestino * 0.5f,
            cachorro.corpo.y + cachorro.corpo.height - alturaDestino + 4,
            larguraDestino,
            alturaDestino
        };

        DrawTexturePro(sprite, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
        return;
    }

    DrawRectangleRec(cachorro.corpo, BROWN);
    DrawCircle((int)cachorro.corpo.x + (cachorro.direcao == 1 ? 48 : 10), (int)cachorro.corpo.y + 10, 8, GOLD);
    DrawRectangleLinesEx(cachorro.corpo, 1, BLACK);
}

void DesenharCarro(Obstaculo carro)
{
    if (carro.tipo == TIPO_CACHORRO) {
        DesenharCachorro(carro);
        return;
    }

    if (carro.tipo == TIPO_BURACO) {
        int x = (int)carro.corpo.x;
        int y = (int)carro.corpo.y;
        Texture2D sprite = spritesBuraco[carro.variante % 4];

        if (sprite.id != 0) {
            Rectangle origem = {0, 0, (float)sprite.width, (float)sprite.height};
            Rectangle destino = {
                carro.corpo.x - 6,
                carro.corpo.y - 5,
                carro.corpo.width + 12,
                carro.corpo.height + 10
            };

            DrawTexturePro(sprite, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
            return;
        }

        DrawEllipse(x + 16, y + 13, 18, 12, Fade(BLACK, 0.85f));
        DrawEllipse(x + 16, y + 12, 11, 7, DARKGRAY);
        DrawLine(x + 2, y + 11, x - 8, y + 5, BLACK);
        DrawLine(x + 24, y + 4, x + 35, y - 2, BLACK);
        DrawLine(x + 27, y + 20, x + 38, y + 25, BLACK);
        return;
    }

    if (carro.tipo == TIPO_PEDRA) {
        int x = (int)carro.corpo.x;
        int y = (int)carro.corpo.y;

        DrawEllipse(x + 14, y + 13, 16, 12, GRAY);
        DrawEllipse(x + 9, y + 9, 9, 7, LIGHTGRAY);
        DrawCircle(x + 21, y + 8, 5, DARKGRAY);
        DrawLine(x + 5, y + 18, x + 24, y + 19, DARKGRAY);
        return;
    }

    if (carro.tipo == TIPO_ARVORE) {
        int x = (int)carro.corpo.x;
        int y = (int)carro.corpo.y;

        if (texturaCoqueiro.id != 0) {
            Rectangle origem = {0, 0, (float)texturaCoqueiro.width, (float)texturaCoqueiro.height};
            Rectangle destino = {
                carro.corpo.x - 4,
                carro.corpo.y - 14,
                38,
                56
            };

            DrawTexturePro(texturaCoqueiro, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
            return;
        }

        DrawRectangle(x + 12, y + 15, 7, 17, BROWN);
        DrawCircle(x + 15, y + 9, 13, DARKGREEN);
        DrawCircle(x + 6, y + 15, 9, GREEN);
        DrawCircle(x + 24, y + 15, 9, GREEN);
        DrawLine(x + 15, y + 15, x + 7, y + 24, DARKBROWN);
        return;
    }

    if (carro.tipo == TIPO_GUARDA_SOL || carro.tipo == TIPO_GUARDA_CHUVA_FREVO) {
        Texture2D textura = carro.tipo == TIPO_GUARDA_SOL ? texturaGuardaSol : texturaGuardaChuvaFrevo;

        if (textura.id != 0) {
            Rectangle origem = {0, 0, (float)textura.width, (float)textura.height};
            Rectangle destino = {
                carro.corpo.x - 6,
                carro.corpo.y - 11,
                38,
                carro.tipo == TIPO_GUARDA_SOL ? 40 : 41
            };

            DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
            return;
        }

        if (carro.tipo == TIPO_GUARDA_SOL) {
            DrawCircle((int)carro.corpo.x + 13, (int)carro.corpo.y + 8, 11, ORANGE);
            DrawRectangle((int)carro.corpo.x + 12, (int)carro.corpo.y + 17, 2, 14, GRAY);
        } else {
            DrawCircle((int)carro.corpo.x + 13, (int)carro.corpo.y + 8, 11, YELLOW);
            DrawRectangle((int)carro.corpo.x + 12, (int)carro.corpo.y + 17, 2, 14, BROWN);
        }
        return;
    }

    if (carro.tipo == TIPO_MOTO) {
        int x = (int)carro.corpo.x;
        int y = (int)carro.corpo.y;

        DrawRectangle(x + 8, y + 5, 27, 8, BLUE);
        DrawRectangle(x + 19, y, 12, 7, SKYBLUE);
        DrawCircle(x + 9, y + 19, 5, BLACK);
        DrawCircle(x + 36, y + 19, 5, BLACK);
        DrawLine(x + 35, y + 5, x + 44, y + 1, BLACK);
        DrawRectangleLinesEx(carro.corpo, 1, DARKBLUE);
        return;
    }

    DrawRectangle((int)carro.corpo.x + 3, (int)carro.corpo.y + 25, LARGURA_CARRO, 8, Fade(BLACK, 0.25f));
    DrawRectangleRec(carro.corpo, carro.tipo == TIPO_ONIBUS ? ORANGE : RED);
    DrawRectangle((int)carro.corpo.x + 10, (int)carro.corpo.y + 5, 18, 8, SKYBLUE);
    DrawRectangle((int)carro.corpo.x + 40, (int)carro.corpo.y + 5, 18, 8, SKYBLUE);
    DrawCircle((int)carro.corpo.x + 15, (int)carro.corpo.y + 30, 5, BLACK);
    DrawCircle((int)carro.corpo.x + 55, (int)carro.corpo.y + 30, 5, BLACK);
    DrawRectangle((int)carro.corpo.x + 62, (int)carro.corpo.y + 10, 6, 6, YELLOW);
    DrawRectangleLinesEx(carro.corpo, 2, BLACK);
}

void DesenharListaObstaculos(Obstaculo *lista)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        DesenharCarro(*atual);
        atual = atual->proximo;
    }
}

bool VerificarColisaoCarro(Obstaculo carro, Rectangle jogador)
{
    if (carro.tipo == TIPO_PEDRA || carro.tipo == TIPO_ARVORE ||
        carro.tipo == TIPO_GUARDA_SOL || carro.tipo == TIPO_GUARDA_CHUVA_FREVO) {
        return false;
    }

    if (carro.tipo == TIPO_BURACO) {
        Rectangle areaPerigosa = {
            carro.corpo.x + 5,
            carro.corpo.y + 5,
            carro.corpo.width - 10,
            carro.corpo.height - 9
        };

        return CheckCollisionRecs(areaPerigosa, jogador);
    }

    return CheckCollisionRecs(carro.corpo, jogador);
}

bool VerificarColisaoFixaLista(Obstaculo *lista, Rectangle jogador)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        if ((atual->tipo == TIPO_PEDRA || atual->tipo == TIPO_ARVORE ||
             atual->tipo == TIPO_GUARDA_SOL || atual->tipo == TIPO_GUARDA_CHUVA_FREVO) &&
            CheckCollisionRecs(atual->corpo, jogador)) {
            return true;
        }

        atual = atual->proximo;
    }

    return false;
}

bool VerificarColisaoLista(Obstaculo *lista, Rectangle jogador)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        if (atual->tipo == TIPO_CACHORRO) {
            atual->mordendo = CheckCollisionRecs(atual->corpo, jogador);

            if (atual->mordendo) {
                return true;
            }

            atual = atual->proximo;
            continue;
        }

        if (VerificarColisaoCarro(*atual, jogador)) {
            return true;
        }

        atual = atual->proximo;
    }

    return false;
}
