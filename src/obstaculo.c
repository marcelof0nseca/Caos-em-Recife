#include "obstaculo.h"
#include "config.h"
#include "mapa.h"
#include <stdio.h>

#define QTD(v) ((int)(sizeof(v) / sizeof((v)[0])))
#define ESQ 0
#define DIR 1

typedef struct {
    Texture2D lado[2];
} SpriteVeiculo;

typedef struct {
    float dx;
    float dy;
    float largura;
    float altura;
    float velocidade;
} ConfigObstaculo;

static Texture2D decoracao[4];
static Texture2D cachorro[2][4]; /* matriz [lado][frame] da animacao do cachorro */
static Texture2D cachorroMordendo[2];
static Texture2D buraco[4];
static Texture2D lixoGrande[6];
static SpriteVeiculo carros[5];
static SpriteVeiculo onibus[4];
static SpriteVeiculo motos[2];
static int proximoLixo = 0;
static int proximoCarro = 0;
static int proximoOnibus = 0;
static int proximaMoto = 0;
/* Vetor/tabela principal: cada indice do enum aponta para a configuracao do tipo de obstaculo. */
static const ConfigObstaculo config[] = {
    [TIPO_CARRO] = {0, 0, LARGURA_CARRO, ALTURA_CARRO, 0},

    [TIPO_ONIBUS] = {0, 0, 110, ALTURA_CARRO, VELOCIDADE_CARRO * 0.65f},
    [TIPO_MOTO] = {0, 5, 45, 22, VELOCIDADE_CARRO * 1.35f},
    [TIPO_BURACO] = {0, 0, TAM_BLOCO, TAM_BLOCO, 0},
    [TIPO_ARVORE] = {5, 4, 30, 32, 0},
    [TIPO_GUARDA_SOL] = {7, 10, 26, 24, 0},
    [TIPO_GUARDA_CHUVA_FREVO] = {7, 9, 26, 25, 0},
    [TIPO_CACHORRO] = {0, 2, LARGURA_CACHORRO, ALTURA_CACHORRO, VELOCIDADE_CACHORRO},
    [TIPO_POSTE] = {15, 6, 10, 30, 0},
    [TIPO_LIXO_GRANDE] = {2, 6, 76, 28, 0},
};
static Texture2D Carregar(const char *caminho, int filtro)
{
    Texture2D t = LoadTexture(caminho);

    if (t.id != 0) {
        SetTextureFilter(t, filtro);
    }

    return t;
}

static void Descarregar(Texture2D *t)
{
    if (t->id != 0) {
        UnloadTexture(*t);
        *t = (Texture2D){0};
    }
}

static void CarregarNumeradas(Texture2D v[], int total, const char *modelo)
{
    char caminho[128];
    for (int i = 0; i < total; i++) {
        snprintf(caminho, sizeof(caminho), modelo, i + 1);
        v[i] = Carregar(caminho, TEXTURE_FILTER_POINT);
    }
}
static void CarregarNomeadas(Texture2D v[], int total, const char *modelo, const char *nomes[])
{
    char caminho[128];
    for (int i = 0; i < total; i++) {
        snprintf(caminho, sizeof(caminho), modelo, nomes[i]);
        v[i] = Carregar(caminho, TEXTURE_FILTER_POINT);
    }
}
static void DescarregarVetor(Texture2D v[], int total)
{
    for (int i = 0; i < total; i++) {
        Descarregar(&v[i]);
    }
}

static void CarregarVeiculos(SpriteVeiculo v[], int total, const char *pasta, const char *tipo, const char *nomes[])
{
    char caminho[128];
    for (int i = 0; i < total; i++) {
        snprintf(caminho, sizeof(caminho), "%s/%s_%s_esquerda.png", pasta, tipo, nomes[i]);
        v[i].lado[ESQ] = Carregar(caminho, TEXTURE_FILTER_BILINEAR);
        snprintf(caminho, sizeof(caminho), "%s/%s_%s_direita.png", pasta, tipo, nomes[i]);
        v[i].lado[DIR] = Carregar(caminho, TEXTURE_FILTER_BILINEAR);
    }
}
static void DescarregarVeiculos(SpriteVeiculo v[], int total)
{
    for (int i = 0; i < total; i++) {
        Descarregar(&v[i].lado[ESQ]);
        Descarregar(&v[i].lado[DIR]);
    }
}
static Texture2D TexturaVeiculo(SpriteVeiculo s, int direcao)
{
    return s.lado[direcao == -1 ? ESQ : DIR];
}
static int ProximoVeiculo(SpriteVeiculo v[], int total, int *proximo)
{
    for (int i = 0; i < total; i++) {
        int indice = (*proximo + i) % total;
        if (v[indice].lado[ESQ].id != 0 || v[indice].lado[DIR].id != 0) {
            *proximo = (indice + 1) % total;
            return indice;
        }
    }

    return 0;
}
void ResetarVarianteLixoGrande(void)
{
    proximoLixo = 0;
    proximoCarro = 0;
    proximoOnibus = 0;
    proximaMoto = 0;
}
void InicializarTexturasObstaculo(void)
{
    const char *nomesCarros[] = {"taxi", "rocam", "pm", "cttu", "firma"};
    const char *nomesOnibus[] = {"borborema", "sport", "nautico", "santa_cruz"};
    const char *nomesMotos[] = {"ifood", "dupla"};
    const char *nomesLixo[] = {"madeira", "caixa", "sofa", "concreto", "cacamba", "barril"};
    decoracao[0] = Carregar("assets/cenario/coqueiro.png", TEXTURE_FILTER_POINT);
    decoracao[1] = Carregar("assets/cenario/guarda_sol.png", TEXTURE_FILTER_POINT);
    decoracao[2] = Carregar("assets/cenario/guarda_chuva_frevo.png", TEXTURE_FILTER_POINT);
    decoracao[3] = Carregar("assets/cenario/poste.png", TEXTURE_FILTER_POINT);
    CarregarVeiculos(carros, QTD(carros), "assets/veiculos/carros", "carro", nomesCarros);
    CarregarVeiculos(onibus, QTD(onibus), "assets/veiculos/onibus", "onibus", nomesOnibus);
    CarregarVeiculos(motos, QTD(motos), "assets/veiculos/motos", "moto", nomesMotos);
    CarregarNumeradas(cachorro[DIR], QTD(cachorro[DIR]), "assets/cachorro/cachorro_correndo_direita_%d.png");
    CarregarNumeradas(cachorro[ESQ], QTD(cachorro[ESQ]), "assets/cachorro/cachorro_correndo_esquerda_%d.png");
    CarregarNumeradas(buraco, QTD(buraco), "assets/itens/buraco_%d.png");
    CarregarNomeadas(lixoGrande, QTD(lixoGrande), "assets/itens/plataforma_%s.png", nomesLixo);
    cachorroMordendo[DIR] = Carregar("assets/cachorro/cachorro_mordendo_direita.png", TEXTURE_FILTER_POINT);
    cachorroMordendo[ESQ] = Carregar("assets/cachorro/cachorro_mordendo_esquerda.png", TEXTURE_FILTER_POINT);
}
void FinalizarTexturasObstaculo(void)
{
    DescarregarVetor(decoracao, QTD(decoracao));
    DescarregarVeiculos(carros, QTD(carros));
    DescarregarVeiculos(onibus, QTD(onibus));
    DescarregarVeiculos(motos, QTD(motos));
    DescarregarVetor(cachorro[DIR], QTD(cachorro[DIR]));
    DescarregarVetor(cachorro[ESQ], QTD(cachorro[ESQ]));
    DescarregarVetor(cachorroMordendo, QTD(cachorroMordendo));
    DescarregarVetor(buraco, QTD(buraco));
    DescarregarVetor(lixoGrande, QTD(lixoGrande));
}
static Rectangle Retangulo(float x, float y, ConfigObstaculo c)
{
    return (Rectangle){x + c.dx, y + c.dy, c.largura, c.altura};
}
static Rectangle RetanguloBuraco(float x, float y, int variante)
{
    /* Matriz [variante][largura/altura] para deixar explicito o uso de matrizes. */
    static const float tamanho[4][2] = {{34, 26}, {42, 31}, {50, 37}, {62, 42}};
    float largura = tamanho[variante][0];
    float altura = tamanho[variante][1];

    return (Rectangle){
        x + (TAM_BLOCO - largura) * 0.5f,
        y + (TAM_BLOCO - altura) * 0.5f,
        largura,
        altura
    };
}
static int Variante(TipoObstaculo tipo, int base)
{
    switch (tipo) {
        case TIPO_CARRO:
            return ProximoVeiculo(carros, QTD(carros), &proximoCarro);
        case TIPO_ONIBUS:
            return ProximoVeiculo(onibus, QTD(onibus), &proximoOnibus);
        case TIPO_MOTO:
            return ProximoVeiculo(motos, QTD(motos), &proximaMoto);
        case TIPO_LIXO_GRANDE: {
            int atual = proximoLixo;
            proximoLixo = (proximoLixo + 1) % QTD(lixoGrande);
            return atual;
        }
        case TIPO_BURACO:
            return base % QTD(buraco);
        case TIPO_POSTE:
            return base % 6;
        default:
            return 0;
    }
}
Obstaculo *CriarObstaculo(TipoObstaculo tipo, float x, float y, float velocidade, int direcao)
{
    /* Alocacao dinamica: cada obstaculo nasce em tempo de execucao e vira um no da lista. */
    Obstaculo *novo = malloc(sizeof(Obstaculo));
    int base = (int)(x / TAM_BLOCO) + (int)(y / TAM_BLOCO);
    ConfigObstaculo c = config[tipo];

    if (novo == NULL) {
        return NULL;
    }

    novo->tipo = tipo;
    novo->corpo = tipo == TIPO_BURACO ? RetanguloBuraco(x, y, base % QTD(buraco)) : Retangulo(x, y, c);
    novo->velocidade = c.velocidade != 0 ? c.velocidade : velocidade;
    novo->direcao = novo->velocidade == 0 ? 0 : direcao;
    novo->mordendo = false;
    novo->variante = Variante(tipo, base);
    novo->proximo = NULL;
    return novo;
}
void AdicionarObstaculo(Obstaculo **lista, Obstaculo *novo)
{
    if (novo != NULL) {
        /* Encadeamento: o novo no aponta para o antigo inicio da lista. */
        novo->proximo = *lista;
        *lista = novo;
    }
}
void LiberarObstaculos(Obstaculo **lista)
{
    while (*lista != NULL) {
        Obstaculo *proximo = (*lista)->proximo;
        /* Libera a memoria que foi reservada com malloc em CriarObstaculo(). */
        free(*lista);
        *lista = proximo;
    }
}
void AtualizarListaObstaculos(Obstaculo *lista)
{
    for (Obstaculo *o = lista; o != NULL; o = o->proximo) {
        o->corpo.x += o->velocidade * o->direcao * GetFrameTime();

        if (o->direcao == 1 && o->corpo.x > LARGURA_TELA) {
            o->corpo.x = -o->corpo.width - 10;
        }

        if (o->direcao == -1 && o->corpo.x + o->corpo.width < 0) {
            o->corpo.x = LARGURA_TELA + o->corpo.width;
        }
    }
}
static bool NoAlagamento(Obstaculo o)
{
    int linha = (int)((o.corpo.y + o.corpo.height * 0.5f) / TAM_BLOCO);
    return LinhaEhAlagamento(linha);
}
static Rectangle Ajustar(Rectangle r, float x, float y, float largura, float altura)
{
    return (Rectangle){r.x + x, r.y + y, r.width + largura, r.height + altura};
}
static Rectangle Centralizar(Rectangle r, float largura, float altura, float y)
{
    return (Rectangle){
        r.x + r.width * 0.5f - largura * 0.5f,
        r.y + r.height - altura + y,
        largura,
        altura
    };
}
static void Desenhar(Texture2D textura, Rectangle destino)
{
    if (textura.id != 0) {
        DrawTexturePro(textura, (Rectangle){0, 0, (float)textura.width, (float)textura.height},
                       destino, (Vector2){0, 0}, 0, WHITE);
    }
}
static void DesenharPoste(Obstaculo o)
{
    Texture2D t = decoracao[3];
    bool espelhar = o.variante % 2 == 1;
    float largura = 17.0f + (o.variante % 3);
    float altura = 66.0f + (o.variante % 4) * 3.0f;
    Rectangle origem = {
        espelhar ? (float)t.width : 0,
        0,
        espelhar ? -(float)t.width : (float)t.width,
        (float)t.height
    };

    if (t.id != 0) {
        DrawTexturePro(t, origem, Centralizar(o.corpo, largura, altura, 4), (Vector2){0, 0}, 0, WHITE);
    }
}
static void DesenharCachorro(Obstaculo o)
{
    int lado = o.direcao == -1 ? ESQ : DIR;
    int frame = (int)(GetTime() * 10.0) % QTD(cachorro[0]);
    Texture2D t = o.mordendo ? cachorroMordendo[lado] : cachorro[lado][frame];
    Desenhar(t, Centralizar(o.corpo, o.mordendo ? 68.0f : 64.0f, o.mordendo ? 40.0f : 36.0f, 4));
}
static void DesenharMoto(Obstaculo o)
{
    int v = o.variante % QTD(motos);
    float escala = v == 0 ? 1.18f : 1.0f;
    float largura = (o.corpo.width + 12) * escala;
    float altura = (o.corpo.height + 24) * escala;

    Desenhar(TexturaVeiculo(motos[v], o.direcao),
             (Rectangle){o.corpo.x - 6 - (largura - o.corpo.width - 12) * 0.5f,
                         o.corpo.y - 12 - (altura - o.corpo.height - 24) * 0.5f, largura, altura});
}
static void DesenharObstaculo(Obstaculo o)
{
    switch (o.tipo) {
        case TIPO_CACHORRO:
            DesenharCachorro(o);
            break;
        case TIPO_LIXO_GRANDE:
            Desenhar(lixoGrande[o.variante % QTD(lixoGrande)], o.corpo);
            break;
        case TIPO_POSTE:
            DesenharPoste(o);
            break;
        case TIPO_ARVORE:
            Desenhar(decoracao[0], (Rectangle){o.corpo.x - 4, o.corpo.y - 14, 38, 56});
            break;
        case TIPO_GUARDA_SOL:
            Desenhar(decoracao[1], (Rectangle){o.corpo.x - 6, o.corpo.y - 11, 38, 40});
            break;
        case TIPO_GUARDA_CHUVA_FREVO:
            Desenhar(decoracao[2], (Rectangle){o.corpo.x - 6, o.corpo.y - 11, 38, 41});
            break;
        case TIPO_MOTO:
            DesenharMoto(o);
            break;
        case TIPO_ONIBUS:
            Desenhar(TexturaVeiculo(onibus[o.variante % QTD(onibus)], o.direcao),
                     Ajustar(o.corpo, -8, -15, 16, 26));
            break;
        case TIPO_BURACO:
            if (!NoAlagamento(o)) {
                Desenhar(buraco[o.variante % QTD(buraco)],
                         Centralizar(o.corpo, o.corpo.width + 34, o.corpo.height + 34, 8));
            }
            break;
        default:
            Desenhar(TexturaVeiculo(carros[o.variante % QTD(carros)], o.direcao),
                     Ajustar(o.corpo, -4, -7, 8, 14));
            break;
    }
}
void DesenharListaObstaculos(Obstaculo *lista)
{
    for (Obstaculo *o = lista; o != NULL; o = o->proximo) {
        DesenharObstaculo(*o);
    }
}
static bool Fixo(TipoObstaculo tipo)
{
    return tipo == TIPO_ARVORE || tipo == TIPO_GUARDA_SOL || tipo == TIPO_GUARDA_CHUVA_FREVO || tipo == TIPO_POSTE;
}
bool VerificarColisaoFixaLista(Obstaculo *lista, Rectangle jogador)
{
    for (Obstaculo *o = lista; o != NULL; o = o->proximo) {
        if (Fixo(o->tipo) && CheckCollisionRecs(o->corpo, jogador)) {
            return true;
        }
    }

    return false;
}

float ObterVelocidadeApoioAlagamento(Obstaculo *lista, Rectangle jogador)
{
    for (Obstaculo *o = lista; o != NULL; o = o->proximo) {
        if (o->tipo == TIPO_LIXO_GRANDE && CheckCollisionRecs(o->corpo, jogador)) {
            return o->velocidade * o->direcao;
        }
    }

    return 0.0f;
}

static bool Colide(Obstaculo o, Rectangle jogador)
{
    if (Fixo(o.tipo) || o.tipo == TIPO_LIXO_GRANDE || (o.tipo == TIPO_BURACO && NoAlagamento(o))) {
        return false;
    }

    if (o.tipo == TIPO_BURACO) {
        return CheckCollisionRecs(Ajustar(o.corpo, 5, 5, -10, -9), jogador);
    }

    return CheckCollisionRecs(o.corpo, jogador);
}

bool VerificarColisaoLista(Obstaculo *lista, Rectangle jogador)
{
    for (Obstaculo *o = lista; o != NULL; o = o->proximo) {
        if (o->tipo == TIPO_CACHORRO) {
            o->mordendo = CheckCollisionRecs(o->corpo, jogador);
            if (o->mordendo) {
                return true;
            }
        } else if (Colide(*o, jogador)) {
            return true;
        }
    }
    return false;
}
        
