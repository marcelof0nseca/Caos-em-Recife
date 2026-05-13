#include "obstaculo.h"
#include "config.h"
#include "mapa.h"
#include <stdio.h>

#define QUANTIDADE(lista) ((int)(sizeof(lista) / sizeof((lista)[0])))

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

static Texture2D CarregarTextura(const char *caminho, int filtro)
{
    Texture2D textura = {0};
    textura = LoadTexture(caminho);

    if (textura.id != 0) {
        SetTextureFilter(textura, filtro);
    }

    return textura;
}

static Texture2D CarregarTexturaFormatada(const char *modelo, const char *texto, int numero, int filtro)
{
    char caminho[128];

    if (texto != NULL) {
        snprintf(caminho, sizeof(caminho), modelo, texto);
    } else {
        snprintf(caminho, sizeof(caminho), modelo, numero);
    }

    return CarregarTextura(caminho, filtro);
}

static SpriteVeiculo CarregarSpriteVeiculo(const char *pasta, const char *tipo, const char *nome)
{
    SpriteVeiculo sprite = {0};
    char esquerda[128];
    char direita[128];

    snprintf(esquerda, sizeof(esquerda), "%s/%s_%s_esquerda.png", pasta, tipo, nome);
    snprintf(direita, sizeof(direita), "%s/%s_%s_direita.png", pasta, tipo, nome);
    sprite.esquerda = CarregarTextura(esquerda, TEXTURE_FILTER_BILINEAR);
    sprite.direita = CarregarTextura(direita, TEXTURE_FILTER_BILINEAR);
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

static void CarregarTexturasPorNumero(Texture2D *texturas, const char *modelo, int total)
{
    for (int i = 0; i < total; i++) {
        texturas[i] = CarregarTexturaFormatada(modelo, NULL, i + 1, TEXTURE_FILTER_POINT);
    }
}

static void CarregarTexturasPorNome(Texture2D *texturas, const char *modelo, const char **nomes, int total)
{
    for (int i = 0; i < total; i++) {
        texturas[i] = CarregarTexturaFormatada(modelo, nomes[i], 0, TEXTURE_FILTER_POINT);
    }
}

static void DescarregarTexturas(Texture2D *texturas, int total)
{
    for (int i = 0; i < total; i++) {
        DescarregarTextura(&texturas[i]);
    }
}

void ResetarVarianteLixoGrande(void)
{
    proximaVarianteLixoGrande = 0;
}

static void CarregarSpritesVeiculo(SpriteVeiculo *sprites, const char *pasta, const char *tipo, const char **nomes, int total)
{
    for (int i = 0; i < total; i++) {
        sprites[i] = CarregarSpriteVeiculo(pasta, tipo, nomes[i]);
    }
}

static void DescarregarSpritesVeiculo(SpriteVeiculo *sprites, int total)
{
    for (int i = 0; i < total; i++) {
        DescarregarSpriteVeiculo(&sprites[i]);
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

static Rectangle RetanguloAjustado(Rectangle base, float x, float y, float largura, float altura)
{
    return (Rectangle){base.x + x, base.y + y, base.width + largura, base.height + altura};
}

static Rectangle RetanguloCentralizado(Rectangle base, float largura, float altura, float y)
{
    return (Rectangle){
        base.x + base.width * 0.5f - largura * 0.5f,
        base.y + base.height - altura + y,
        largura,
        altura
    };
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
    const char *carros[] = {"taxi", "rocam", "pm", "cttu", "firma"};
    const char *onibus[] = {"borborema", "sport", "nautico", "santa_cruz"};
    const char *motos[] = {"ifood", "dupla"};
    const char *plataformas[] = {"madeira", "caixa", "sofa", "concreto", "cacamba", "barril"};

    texturaCoqueiro = CarregarTextura("assets/cenario/coqueiro.png", TEXTURE_FILTER_POINT);
    texturaGuardaSol = CarregarTextura("assets/cenario/guarda_sol.png", TEXTURE_FILTER_POINT);
    texturaGuardaChuvaFrevo = CarregarTextura("assets/cenario/guarda_chuva_frevo.png", TEXTURE_FILTER_POINT);
    texturaPoste = CarregarTextura("assets/cenario/poste.png", TEXTURE_FILTER_POINT);
    CarregarSpritesVeiculo(spritesCarro, "assets/veiculos/carros", "carro", carros, QUANTIDADE(spritesCarro));
    CarregarSpritesVeiculo(spritesOnibus, "assets/veiculos/onibus", "onibus", onibus, QUANTIDADE(spritesOnibus));
    CarregarSpritesVeiculo(spritesMoto, "assets/veiculos/motos", "moto", motos, QUANTIDADE(spritesMoto));
    CarregarTexturasPorNumero(spritesCachorroDireita, "assets/cachorro/cachorro_correndo_direita_%d.png", QUANTIDADE(spritesCachorroDireita));
    CarregarTexturasPorNumero(spritesCachorroEsquerda, "assets/cachorro/cachorro_correndo_esquerda_%d.png", QUANTIDADE(spritesCachorroEsquerda));
    CarregarTexturasPorNumero(spritesBuraco, "assets/itens/buraco_%d.png", QUANTIDADE(spritesBuraco));
    CarregarTexturasPorNome(spritesLixoGrande, "assets/itens/plataforma_%s.png", plataformas, QUANTIDADE(spritesLixoGrande));
    spriteCachorroMordendoDireita = CarregarTextura("assets/cachorro/cachorro_mordendo_direita.png", TEXTURE_FILTER_POINT);
    spriteCachorroMordendoEsquerda = CarregarTextura("assets/cachorro/cachorro_mordendo_esquerda.png", TEXTURE_FILTER_POINT);
}

void FinalizarTexturasObstaculo(void)
{
    DescarregarTextura(&texturaCoqueiro);
    DescarregarTextura(&texturaGuardaSol);
    DescarregarTextura(&texturaGuardaChuvaFrevo);
    DescarregarTextura(&texturaPoste);
    DescarregarSpritesVeiculo(spritesCarro, QUANTIDADE(spritesCarro));
    DescarregarSpritesVeiculo(spritesOnibus, QUANTIDADE(spritesOnibus));
    DescarregarSpritesVeiculo(spritesMoto, QUANTIDADE(spritesMoto));
    DescarregarTexturas(spritesCachorroDireita, QUANTIDADE(spritesCachorroDireita));
    DescarregarTexturas(spritesCachorroEsquerda, QUANTIDADE(spritesCachorroEsquerda));
    DescarregarTexturas(spritesBuraco, QUANTIDADE(spritesBuraco));
    DescarregarTexturas(spritesLixoGrande, QUANTIDADE(spritesLixoGrande));
    DescarregarTextura(&spriteCachorroMordendoDireita);
    DescarregarTextura(&spriteCachorroMordendoEsquerda);
}

static void ConfigurarObstaculo(Obstaculo *obstaculo, TipoObstaculo tipo, Rectangle corpo,
                                float velocidade, int direcao, int variante)
{
    obstaculo->tipo = tipo;
    obstaculo->corpo = corpo;
    obstaculo->velocidade = velocidade;
    obstaculo->direcao = direcao;
    obstaculo->mordendo = false;
    obstaculo->variante = variante;
    obstaculo->proximo = NULL;
}

static Rectangle CriarCorpoBuraco(float x, float y, int variante)
{
    /* O buraco muda um pouco de tamanho para nao ficar tudo igual. */
    float larguras[] = {34, 42, 50, 62};
    float alturas[] = {26, 31, 37, 42};

    return (Rectangle){
        x + (TAM_BLOCO - larguras[variante]) * 0.5f,
        y + (TAM_BLOCO - alturas[variante]) * 0.5f,
        larguras[variante],
        alturas[variante]
    };
}

Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao)
{
    Obstaculo *novo = (Obstaculo *)malloc(sizeof(Obstaculo));
    int variante = ((int)(x / TAM_BLOCO) + (int)(y / TAM_BLOCO));

    if (novo == NULL) {
        return NULL;
    }

    switch (tipo) {
        case TIPO_BURACO:
            ConfigurarObstaculo(novo, tipo, CriarCorpoBuraco(x, y, variante % 4), 0, 0, variante % 4);
            break;
        case TIPO_ARVORE:
            ConfigurarObstaculo(novo, tipo, (Rectangle){x + 5, y + 4, 30, 32}, 0, 0, 0);
            break;
        case TIPO_GUARDA_SOL:
            ConfigurarObstaculo(novo, tipo, (Rectangle){x + 7, y + 10, 26, 24}, 0, 0, 0);
            break;
        case TIPO_GUARDA_CHUVA_FREVO:
            ConfigurarObstaculo(novo, tipo, (Rectangle){x + 7, y + 9, 26, 25}, 0, 0, 0);
            break;
        case TIPO_MOTO:
            ConfigurarObstaculo(novo, tipo, (Rectangle){x, y + 5, 45, 22}, VELOCIDADE_CARRO * 1.35f,
                                direcao, SortearVarianteVeiculo(spritesMoto, QUANTIDADE(spritesMoto)));
            break;
        case TIPO_CACHORRO:
            ConfigurarObstaculo(novo, tipo, (Rectangle){x, y + 2, LARGURA_CACHORRO, ALTURA_CACHORRO},
                                VELOCIDADE_CACHORRO, direcao, 0);
            break;
        case TIPO_POSTE:
            ConfigurarObstaculo(novo, tipo, (Rectangle){x + 15, y + 6, 10, 30}, 0, 0, variante % 6);
            break;
        case TIPO_LIXO_GRANDE:
            ConfigurarObstaculo(novo, tipo, (Rectangle){x + 2, y + 6, 76, 28}, velocidade,
                                direcao, proximaVarianteLixoGrande);
            proximaVarianteLixoGrande = (proximaVarianteLixoGrande + 1) % QUANTIDADE(spritesLixoGrande);
            break;
        case TIPO_ONIBUS:
            ConfigurarObstaculo(novo, tipo, (Rectangle){x, y, 110, ALTURA_CARRO}, VELOCIDADE_CARRO * 0.65f,
                                direcao, SortearVarianteVeiculo(spritesOnibus, QUANTIDADE(spritesOnibus)));
            break;
        default:
            ConfigurarObstaculo(novo, TIPO_CARRO, (Rectangle){x, y, LARGURA_CARRO, ALTURA_CARRO},
                                velocidade, direcao, SortearVarianteVeiculo(spritesCarro, QUANTIDADE(spritesCarro)));
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

    DesenharSprite(sprite, RetanguloCentralizado(cachorro.corpo, larguraDestino, alturaDestino, 4));
}

static void DesenharCarro(Obstaculo carro)
{
    if (carro.tipo == TIPO_CACHORRO) {
        DesenharCachorro(carro);
        return;
    }

    if (carro.tipo == TIPO_LIXO_GRANDE) {
        DesenharSprite(spritesLixoGrande[carro.variante % QUANTIDADE(spritesLixoGrande)], carro.corpo);
        return;
    }

    if (carro.tipo == TIPO_BURACO) {
        if (ObstaculoEstaNoAlagamento(carro)) {
            return;
        }

        Texture2D sprite = spritesBuraco[carro.variante % QUANTIDADE(spritesBuraco)];
        DesenharSprite(sprite, RetanguloCentralizado(carro.corpo, carro.corpo.width + 34, carro.corpo.height + 34, 8));
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
        DesenharSprite(texturaCoqueiro, (Rectangle){carro.corpo.x - 4, carro.corpo.y - 14, 38, 56});
        return;
    }

    if (carro.tipo == TIPO_GUARDA_SOL || carro.tipo == TIPO_GUARDA_CHUVA_FREVO) {
        Texture2D textura = carro.tipo == TIPO_GUARDA_SOL ? texturaGuardaSol : texturaGuardaChuvaFrevo;

        DesenharSprite(textura, (Rectangle){carro.corpo.x - 6, carro.corpo.y - 11, 38,
                                            carro.tipo == TIPO_GUARDA_SOL ? 40 : 41});
        return;
    }

    if (carro.tipo == TIPO_MOTO) {
        int varianteMoto = carro.variante % QUANTIDADE(spritesMoto);
        Texture2D sprite = ObterTexturaVeiculo(spritesMoto[varianteMoto], carro.direcao);
        /* A primeira moto e menor no arquivo, entao aumento um pouco. */
        float escala = varianteMoto == 0 ? 1.18f : 1.0f;

        float larguraDestinoBase = carro.corpo.width + 12;
        float alturaDestinoBase = carro.corpo.height + 24;
        float larguraDestino = larguraDestinoBase * escala;
        float alturaDestino = alturaDestinoBase * escala;

        DesenharSprite(sprite, (Rectangle){carro.corpo.x - 6 - (larguraDestino - larguraDestinoBase) * 0.5f,
                                           carro.corpo.y - 12 - (alturaDestino - alturaDestinoBase) * 0.5f,
                                           larguraDestino, alturaDestino});
        return;
    }

    if (carro.tipo == TIPO_CARRO) {
        Texture2D sprite = ObterTexturaVeiculo(spritesCarro[carro.variante % QUANTIDADE(spritesCarro)], carro.direcao);
        DesenharSprite(sprite, RetanguloAjustado(carro.corpo, -4, -7, 8, 14));
        return;
    }

    if (carro.tipo == TIPO_ONIBUS) {
        Texture2D sprite = ObterTexturaVeiculo(spritesOnibus[carro.variante % QUANTIDADE(spritesOnibus)], carro.direcao);
        DesenharSprite(sprite, RetanguloAjustado(carro.corpo, -8, -15, 16, 26));
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
