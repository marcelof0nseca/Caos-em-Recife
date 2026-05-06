#include "obstaculo.h"
#include "config.h"
#include "mapa.h"

static Texture2D texturaCoqueiro = {0};
static Texture2D texturaGuardaSol = {0};
static Texture2D texturaGuardaChuvaFrevo = {0};
static Texture2D texturaPoste = {0};
static Texture2D spritesCachorroDireita[4] = {0};
static Texture2D spritesCachorroEsquerda[4] = {0};
static Texture2D spriteCachorroMordendoDireita = {0};
static Texture2D spriteCachorroMordendoEsquerda = {0};
static Texture2D spritesBuraco[4] = {0};
static Texture2D spritesLixoGrande[6] = {0};
static int proximaVarianteLixoGrande = 0;

typedef struct {
    Texture2D esquerda;
    Texture2D direita;
} SpriteVeiculo;

static SpriteVeiculo spritesCarro[5] = {0};
static SpriteVeiculo spritesOnibus[4] = {0};
static SpriteVeiculo spritesMoto[2] = {0};

#define TOTAL_SPRITES_CARRO ((int)(sizeof(spritesCarro) / sizeof(spritesCarro[0])))
#define TOTAL_SPRITES_ONIBUS ((int)(sizeof(spritesOnibus) / sizeof(spritesOnibus[0])))
#define TOTAL_SPRITES_MOTO ((int)(sizeof(spritesMoto) / sizeof(spritesMoto[0])))

typedef struct {
    const char *esquerda;
    const char *direita;
} CaminhosVeiculo;

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

static Texture2D CarregarTexturaVeiculo(const char *caminho)
{
    Texture2D textura = {0};
    textura = LoadTexture(caminho);

    if (textura.id != 0) {
        SetTextureFilter(textura, TEXTURE_FILTER_BILINEAR);
    }

    return textura;
}

static SpriteVeiculo CarregarSpriteVeiculo(const char *caminhoEsquerda, const char *caminhoDireita)
{
    SpriteVeiculo sprite = {0};

    sprite.esquerda = CarregarTexturaVeiculo(caminhoEsquerda);
    sprite.direita = CarregarTexturaVeiculo(caminhoDireita);
    return sprite;
}

static void DescarregarSpriteVeiculo(SpriteVeiculo *sprite)
{
    if (sprite->esquerda.id != 0) {
        UnloadTexture(sprite->esquerda);
        sprite->esquerda = (Texture2D){0};
    }

    if (sprite->direita.id != 0) {
        UnloadTexture(sprite->direita);
        sprite->direita = (Texture2D){0};
    }
}

static void DescarregarTextura(Texture2D *textura)
{
    if (textura->id != 0) {
        UnloadTexture(*textura);
        *textura = (Texture2D){0};
    }
}

void ResetarVarianteLixoGrande(void)
{
    proximaVarianteLixoGrande = 0;
}

static void CarregarSpritesVeiculo(SpriteVeiculo *sprites, const CaminhosVeiculo *caminhos, int total)
{
    for (int i = 0; i < total; i++) {
        sprites[i] = CarregarSpriteVeiculo(caminhos[i].esquerda, caminhos[i].direita);
    }
}

static Texture2D ObterTexturaVeiculo(SpriteVeiculo sprite, int direcao)
{
    /* Cada veiculo tem uma imagem para esquerda e outra para direita. */
    return direcao == -1 ? sprite.esquerda : sprite.direita;
}

static void DesenharSprite(Texture2D sprite, Rectangle destino)
{
    /* Sem desenho manual: se o png nao carregou, nao desenha nada. */
    if (sprite.id == 0) {
        return;
    }

    DrawTexturePro(sprite, (Rectangle){0, 0, (float)sprite.width, (float)sprite.height},
                   destino, (Vector2){0, 0}, 0.0f, WHITE);
}

static bool ObstaculoEstaNoAlagamento(Obstaculo obstaculo)
{
    int linha = (int)((obstaculo.corpo.y + obstaculo.corpo.height * 0.5f) / TAM_BLOCO);
    return LinhaEhAlagamento(linha);
}

static int SortearVarianteVeiculo(SpriteVeiculo *sprites, int total)
{
    int varianteInicial = GetRandomValue(0, total - 1);

    /* Tenta sortear um sprite carregado, para nao escolher imagem vazia. */
    if (ObterTexturaVeiculo(sprites[varianteInicial], 1).id != 0 ||
        ObterTexturaVeiculo(sprites[varianteInicial], -1).id != 0) {
        return varianteInicial;
    }

    for (int variante = 0; variante < total; variante++) {
        if (ObterTexturaVeiculo(sprites[variante], 1).id != 0 ||
            ObterTexturaVeiculo(sprites[variante], -1).id != 0) {
            return variante;
        }
    }

    return varianteInicial;
}

void InicializarTexturasObstaculo(void)
{
    CaminhosVeiculo carros[] = {
        {"assets/veiculos/prontos/carros/taxi_esquerda.png", "assets/veiculos/prontos/carros/taxi_direita.png"},
        {"assets/veiculos/prontos/carros/rocam_esquerda.png", "assets/veiculos/prontos/carros/rocam_direita.png"},
        {"assets/veiculos/prontos/carros/pm_esquerda.png", "assets/veiculos/prontos/carros/pm_direita.png"},
        {"assets/veiculos/prontos/carros/cttu_esquerda.png", "assets/veiculos/prontos/carros/cttu_direita.png"},
        {"assets/veiculos/prontos/carros/firma_esquerda.png", "assets/veiculos/prontos/carros/firma_direita.png"}
    };
    CaminhosVeiculo onibus[] = {
        {"assets/veiculos/prontos/onibus/borborema_esquerda.png", "assets/veiculos/prontos/onibus/borborema_direita.png"},
        {"assets/veiculos/prontos/onibus/sport_esquerda.png", "assets/veiculos/prontos/onibus/sport_direita.png"},
        {"assets/veiculos/prontos/onibus/nautico_esquerda.png", "assets/veiculos/prontos/onibus/nautico_direita.png"},
        {"assets/veiculos/prontos/onibus/santa_cruz_esquerda.png", "assets/veiculos/prontos/onibus/santa_cruz_direita.png"}
    };
    CaminhosVeiculo motos[] = {
        {"assets/veiculos/prontos/motos/ifood_esquerda.png", "assets/veiculos/prontos/motos/ifood_direita.png"},
        {"assets/veiculos/prontos/motos/dupla_esquerda.png", "assets/veiculos/prontos/motos/dupla_direita.png"}
    };

    texturaCoqueiro = CarregarTexturaObstaculo("assets/cenario/coqueiro.png");
    texturaGuardaSol = CarregarTexturaObstaculo("assets/cenario/guarda_sol.png");
    texturaGuardaChuvaFrevo = CarregarTexturaObstaculo("assets/cenario/guarda_chuva_frevo.png");
    texturaPoste = CarregarTexturaObstaculo("assets/cenario/poste.png");
    CarregarSpritesVeiculo(spritesCarro, carros, TOTAL_SPRITES_CARRO);
    CarregarSpritesVeiculo(spritesOnibus, onibus, TOTAL_SPRITES_ONIBUS);
    CarregarSpritesVeiculo(spritesMoto, motos, TOTAL_SPRITES_MOTO);
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
    spritesLixoGrande[0] = CarregarTexturaObstaculo("assets/itens/plataforma_madeira.png");
    spritesLixoGrande[1] = CarregarTexturaObstaculo("assets/itens/plataforma_caixa.png");
    spritesLixoGrande[2] = CarregarTexturaObstaculo("assets/itens/plataforma_sofa.png");
    spritesLixoGrande[3] = CarregarTexturaObstaculo("assets/itens/plataforma_concreto.png");
    spritesLixoGrande[4] = CarregarTexturaObstaculo("assets/itens/plataforma_cacamba.png");
    spritesLixoGrande[5] = CarregarTexturaObstaculo("assets/itens/plataforma_barril.png");
}

void FinalizarTexturasObstaculo(void)
{
    int frame;
    int variante;

    DescarregarTextura(&texturaCoqueiro);
    DescarregarTextura(&texturaGuardaSol);
    DescarregarTextura(&texturaGuardaChuvaFrevo);
    DescarregarTextura(&texturaPoste);

    for (variante = 0; variante < TOTAL_SPRITES_CARRO; variante++) {
        DescarregarSpriteVeiculo(&spritesCarro[variante]);
    }

    for (variante = 0; variante < TOTAL_SPRITES_ONIBUS; variante++) {
        DescarregarSpriteVeiculo(&spritesOnibus[variante]);
    }

    for (variante = 0; variante < TOTAL_SPRITES_MOTO; variante++) {
        DescarregarSpriteVeiculo(&spritesMoto[variante]);
    }

    for (frame = 0; frame < 4; frame++) {
        DescarregarTextura(&spritesCachorroDireita[frame]);
        DescarregarTextura(&spritesCachorroEsquerda[frame]);
        DescarregarTextura(&spritesBuraco[frame]);
    }

    for (frame = 0; frame < 6; frame++) {
        DescarregarTextura(&spritesLixoGrande[frame]);
    }

    DescarregarTextura(&spriteCachorroMordendoDireita);
    DescarregarTextura(&spriteCachorroMordendoEsquerda);
}

static void ConfigurarObstaculo(Obstaculo *obstaculo, TipoObstaculo tipo, Rectangle corpo, float velocidade, int direcao, int variante)
{
    obstaculo->tipo = tipo;
    obstaculo->corpo = corpo;
    obstaculo->velocidade = velocidade;
    obstaculo->direcao = direcao;
    obstaculo->mordendo = false;
    obstaculo->variante = variante;
    obstaculo->proximo = NULL;
}

static void IniciarCarroComDados(Obstaculo *carro, float x, float y, float velocidade, int direcao)
{
    ConfigurarObstaculo(carro, TIPO_CARRO, (Rectangle){x, y, LARGURA_CARRO, ALTURA_CARRO},
                        velocidade, direcao, SortearVarianteVeiculo(spritesCarro, TOTAL_SPRITES_CARRO));
}

static void IniciarOnibus(Obstaculo *onibus, float x, float y, int direcao)
{
    ConfigurarObstaculo(onibus, TIPO_ONIBUS, (Rectangle){x, y, 110, ALTURA_CARRO},
                        VELOCIDADE_CARRO * 0.65f, direcao, SortearVarianteVeiculo(spritesOnibus, TOTAL_SPRITES_ONIBUS));
}

static void IniciarMoto(Obstaculo *moto, float x, float y, int direcao)
{
    ConfigurarObstaculo(moto, TIPO_MOTO, (Rectangle){x, y + 5, 45, 22},
                        VELOCIDADE_CARRO * 1.35f, direcao, SortearVarianteVeiculo(spritesMoto, TOTAL_SPRITES_MOTO));
}

static void IniciarBuraco(Obstaculo *buraco, float x, float y)
{
    /* O buraco muda um pouco de tamanho para nao ficar tudo igual. */
    int variante = ((int)(x / TAM_BLOCO) + (int)(y / TAM_BLOCO)) % 4;
    float larguras[] = {34, 42, 50, 62};
    float alturas[] = {26, 31, 37, 42};

    ConfigurarObstaculo(buraco, TIPO_BURACO, (Rectangle){
        x + (TAM_BLOCO - larguras[variante]) * 0.5f,
        y + (TAM_BLOCO - alturas[variante]) * 0.5f,
        larguras[variante],
        alturas[variante]
    }, 0, 0, variante);
}

static void IniciarFixo(Obstaculo *obstaculo, TipoObstaculo tipo, Rectangle corpo)
{
    ConfigurarObstaculo(obstaculo, tipo, corpo, 0, 0, 0);
}

static void IniciarArvore(Obstaculo *arvore, float x, float y)
{
    IniciarFixo(arvore, TIPO_ARVORE, (Rectangle){x + 5, y + 4, 30, 32});
}

static void IniciarGuardaSol(Obstaculo *guardaSol, float x, float y)
{
    IniciarFixo(guardaSol, TIPO_GUARDA_SOL, (Rectangle){x + 7, y + 10, 26, 24});
}

static void IniciarGuardaChuvaFrevo(Obstaculo *guardaChuvaFrevo, float x, float y)
{
    IniciarFixo(guardaChuvaFrevo, TIPO_GUARDA_CHUVA_FREVO, (Rectangle){x + 7, y + 9, 26, 25});
}

static void IniciarCachorro(Obstaculo *cachorro, float x, float y, int direcao)
{
    ConfigurarObstaculo(cachorro, TIPO_CACHORRO, (Rectangle){x, y + 2, LARGURA_CACHORRO, ALTURA_CACHORRO},
                        VELOCIDADE_CACHORRO, direcao, 0);
}

static void IniciarPoste(Obstaculo *poste, float x, float y)
{
    ConfigurarObstaculo(poste, TIPO_POSTE, (Rectangle){x + 15, y + 6, 10, 30},
                        0, 0, ((int)(x / TAM_BLOCO) + (int)(y / TAM_BLOCO)) % 6);
}

static void IniciarLixoGrande(Obstaculo *lixo, float x, float y, float velocidade, int direcao)
{
    int variante = proximaVarianteLixoGrande;
    proximaVarianteLixoGrande = (proximaVarianteLixoGrande + 1) % 6;

    ConfigurarObstaculo(lixo, TIPO_LIXO_GRANDE, (Rectangle){x + 2, y + 6, 76, 28},
                        velocidade, direcao, variante);
}

Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao)
{
    Obstaculo *novo = (Obstaculo *)malloc(sizeof(Obstaculo));

    if (novo == NULL) {
        return NULL;
    }

    if (tipo == TIPO_BURACO) {
        IniciarBuraco(novo, x, y);
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
    } else if (tipo == TIPO_POSTE) {
        IniciarPoste(novo, x, y);
    } else if (tipo == TIPO_LIXO_GRANDE) {
        IniciarLixoGrande(novo, x, y, velocidade, direcao);
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

static void AtualizarCarro(Obstaculo *carro)
{
    if (carro->velocidade == 0) {
        return;
    }

    carro->corpo.x += carro->velocidade * carro->direcao * GetFrameTime();

    /* Quando sai da tela, volta pelo outro lado para continuar o fluxo. */
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
    /* Troca de frame pelo tempo para o cachorro parecer correndo. */
    int frame = (int)(GetTime() * 10.0) % 4;

    if (cachorro.mordendo) {
        sprite = cachorro.direcao == 1 ? spriteCachorroMordendoDireita : spriteCachorroMordendoEsquerda;
    } else {
        sprite = cachorro.direcao == 1 ? spritesCachorroDireita[frame] : spritesCachorroEsquerda[frame];
    }

    float larguraDestino = cachorro.mordendo ? 68.0f : 64.0f;
    float alturaDestino = cachorro.mordendo ? 40.0f : 36.0f;

    DesenharSprite(sprite, (Rectangle){
        cachorro.corpo.x + cachorro.corpo.width * 0.5f - larguraDestino * 0.5f,
        cachorro.corpo.y + cachorro.corpo.height - alturaDestino + 4,
        larguraDestino,
        alturaDestino
    });
}

static void DesenharCarro(Obstaculo carro)
{
    if (carro.tipo == TIPO_CACHORRO) {
        DesenharCachorro(carro);
        return;
    }

    if (carro.tipo == TIPO_LIXO_GRANDE) {
        Texture2D sprite = spritesLixoGrande[carro.variante % 6];

        DesenharSprite(sprite, (Rectangle){
            carro.corpo.x,
            carro.corpo.y,
            carro.corpo.width,
            carro.corpo.height
        });
        return;
    }

    if (carro.tipo == TIPO_BURACO) {
        if (ObstaculoEstaNoAlagamento(carro)) {
            return;
        }

        Texture2D sprite = spritesBuraco[carro.variante % 4];

        float larguraDestino = carro.corpo.width + 34;
        float alturaDestino = carro.corpo.height + 34;

        DesenharSprite(sprite, (Rectangle){
            carro.corpo.x + carro.corpo.width * 0.5f - larguraDestino * 0.5f,
            carro.corpo.y + carro.corpo.height - alturaDestino + 8,
            larguraDestino,
            alturaDestino
        });
        return;
    }

    if (carro.tipo == TIPO_POSTE) {
        if (texturaPoste.id == 0) {
            return;
        }

        /* Espelha e varia um pouco o poste para nao repetir igual. */
        bool espelhar = carro.variante % 2 == 1;
        float largura = 17.0f + (carro.variante % 3);
        float altura = 66.0f + (carro.variante % 4) * 3.0f;
        Rectangle origem = {
            espelhar ? (float)texturaPoste.width : 0,
            0,
            espelhar ? -(float)texturaPoste.width : (float)texturaPoste.width,
            (float)texturaPoste.height
        };
        Rectangle destino = {
            carro.corpo.x + carro.corpo.width * 0.5f - largura * 0.5f,
            carro.corpo.y + carro.corpo.height - altura + 4,
            largura,
            altura
        };

        DrawTexturePro(texturaPoste, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
        return;
    }

    if (carro.tipo == TIPO_ARVORE) {
        DesenharSprite(texturaCoqueiro, (Rectangle){
            carro.corpo.x - 4,
            carro.corpo.y - 14,
            38,
            56
        });
        return;
    }

    if (carro.tipo == TIPO_GUARDA_SOL || carro.tipo == TIPO_GUARDA_CHUVA_FREVO) {
        Texture2D textura = carro.tipo == TIPO_GUARDA_SOL ? texturaGuardaSol : texturaGuardaChuvaFrevo;

        DesenharSprite(textura, (Rectangle){
            carro.corpo.x - 6,
            carro.corpo.y - 11,
            38,
            carro.tipo == TIPO_GUARDA_SOL ? 40 : 41
        });
        return;
    }

    if (carro.tipo == TIPO_MOTO) {
        int varianteMoto = carro.variante % TOTAL_SPRITES_MOTO;
        Texture2D sprite = ObterTexturaVeiculo(spritesMoto[varianteMoto], carro.direcao);
        /* A primeira moto e menor no arquivo, entao aumento um pouco. */
        float escala = varianteMoto == 0 ? 1.18f : 1.0f;

        float larguraDestinoBase = carro.corpo.width + 12;
        float alturaDestinoBase = carro.corpo.height + 24;
        float larguraDestino = larguraDestinoBase * escala;
        float alturaDestino = alturaDestinoBase * escala;

        DesenharSprite(sprite, (Rectangle){
            carro.corpo.x - 6 - (larguraDestino - larguraDestinoBase) * 0.5f,
            carro.corpo.y - 12 - (alturaDestino - alturaDestinoBase) * 0.5f,
            larguraDestino,
            alturaDestino
        });
        return;
    }

    if (carro.tipo == TIPO_CARRO) {
        Texture2D sprite = ObterTexturaVeiculo(spritesCarro[carro.variante % TOTAL_SPRITES_CARRO], carro.direcao);

        DesenharSprite(sprite, (Rectangle){
            carro.corpo.x - 4,
            carro.corpo.y - 7,
            carro.corpo.width + 8,
            carro.corpo.height + 14
        });
        return;
    }

    if (carro.tipo == TIPO_ONIBUS) {
        Texture2D sprite = ObterTexturaVeiculo(spritesOnibus[carro.variante % TOTAL_SPRITES_ONIBUS], carro.direcao);

        DesenharSprite(sprite, (Rectangle){
            carro.corpo.x - 8,
            carro.corpo.y - 15,
            carro.corpo.width + 16,
            carro.corpo.height + 26
        });
        return;
    }
}

void DesenharListaObstaculos(Obstaculo *lista)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        DesenharCarro(*atual);
        atual = atual->proximo;
    }
}

static bool EhObstaculoFixo(TipoObstaculo tipo)
{
    return tipo == TIPO_ARVORE || tipo == TIPO_GUARDA_SOL || tipo == TIPO_GUARDA_CHUVA_FREVO ||
           tipo == TIPO_POSTE;
}

static bool VerificarColisaoCarro(Obstaculo carro, Rectangle jogador)
{
    /* Obstaculos parados bloqueiam o caminho em outra funcao. */
    if (EhObstaculoFixo(carro.tipo) || carro.tipo == TIPO_LIXO_GRANDE) {
        return false;
    }

    if (carro.tipo == TIPO_BURACO) {
        if (ObstaculoEstaNoAlagamento(carro)) {
            return false;
        }

        /* A colisao do buraco e menor que o sprite para ficar mais justo. */
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
        if (EhObstaculoFixo(atual->tipo) && CheckCollisionRecs(atual->corpo, jogador)) {
            return true;
        }

        atual = atual->proximo;
    }

    return false;
}

float ObterVelocidadeApoioAlagamento(Obstaculo *lista, Rectangle jogador)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        /* Retorna a velocidade da plataforma que esta debaixo do jogador. */
        if (atual->tipo == TIPO_LIXO_GRANDE && CheckCollisionRecs(atual->corpo, jogador)) {
            return atual->velocidade * atual->direcao;
        }

        atual = atual->proximo;
    }

    return 0.0f;
}

bool VerificarColisaoLista(Obstaculo *lista, Rectangle jogador)
{
    Obstaculo *atual = lista;

    while (atual != NULL) {
        if (atual->tipo == TIPO_CACHORRO) {
            /* Quando encosta no jogador, troca para o sprite mordendo. */
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
