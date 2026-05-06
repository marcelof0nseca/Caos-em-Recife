#include "jogo.h"
#include "mapa.h"
#include "config.h"
#include "score.h"

typedef struct {
    int coluna;
    int linha;
    bool usaDeslocamentoFase;
} PosicaoMapa;

typedef struct {
    TipoObstaculo tipo;
    float x;
    int linha;
    float velocidade;
    int direcao;
    bool usaDeslocamentoFase;
    bool xDepoisDaTela;
} ObstaculoMovel;

typedef struct {
    TipoObstaculo tipo;
    int coluna;
    int linha;
    bool usaDeslocamentoFase;
} ObstaculoFixo;

static float LimitarFloat(float valor, float minimo, float maximo)
{
    if (valor < minimo) {
        return minimo;
    }

    if (valor > maximo) {
        return maximo;
    }

    return valor;
}

static void AtualizarRecorde(Jogo *jogo)
{
    if (jogo->jogador.score > jogo->recorde) {
        jogo->recorde = jogo->jogador.score;
        SalvarRecorde(jogo->recorde);
    }
}

static void AdicionarMoedaMapa(Jogo *jogo, int coluna, int linha)
{
    if (jogo->totalMoedas >= MAX_MOEDAS) {
        return;
    }

    IniciarMoeda(&jogo->moedas[jogo->totalMoedas], coluna, linha);
    jogo->totalMoedas++;
}

static void AdicionarMoedaFase(Jogo *jogo, int coluna, int linha)
{
    AdicionarMoedaMapa(jogo, coluna, linha + LINHAS_FASE_3);
}

static void ConfigurarMoedas(Jogo *jogo)
{
    PosicaoMapa moedas[] = {
        {10, 44, true}, {15, 39, true}, {6, 36, true}, {13, 33, true},
        {8, 29, true}, {18, 23, true}, {2, 19, true}, {14, 16, true},
        {4, 13, true}, {12, 10, true}, {6, 7, true}, {17, 4, true},
        {4, 13, false}, {15, 10, false}, {7, 7, false}, {13, 4, false}
    };

    jogo->totalMoedas = 0;

    for (int i = 0; i < (int)(sizeof(moedas) / sizeof(moedas[0])); i++) {
        if (moedas[i].usaDeslocamentoFase) {
            AdicionarMoedaFase(jogo, moedas[i].coluna, moedas[i].linha);
        } else {
            AdicionarMoedaMapa(jogo, moedas[i].coluna, moedas[i].linha);
        }
    }
}

static void AdicionarObstaculoNaLinhaMapa(Jogo *jogo, TipoObstaculo tipo, float x, int linha, float velocidade, int direcao)
{
    AdicionarObstaculo(
        &jogo->obstaculos,
        CriarObstaculo(tipo, x, linha * TAM_BLOCO -3 , velocidade, direcao)
    );
}

static void AdicionarObstaculoNaLinha(Jogo *jogo, TipoObstaculo tipo, float x, int linha, float velocidade, int direcao)
{
    AdicionarObstaculoNaLinhaMapa(jogo, tipo, x, linha + LINHAS_FASE_3, velocidade, direcao);
}

static void AdicionarObstaculoFixoMapa(Jogo *jogo, TipoObstaculo tipo, int coluna, int linha)
{
    /* Evita colocar lixo fora da agua e obstaculo fixo no meio da rua. */
    if ((tipo == TIPO_LIXO_GRANDE && !LinhaEhAlagamento(linha)) ||
        (tipo != TIPO_LIXO_GRANDE && (LinhaEhRua(linha) || LinhaEhAlagamento(linha)))) {
        return;
    }

    AdicionarObstaculo(
        &jogo->obstaculos,
        CriarObstaculo(tipo, coluna * TAM_BLOCO, linha * TAM_BLOCO, 0, 0)
    );
}

static void AdicionarBuracoMapa(Jogo *jogo, int coluna, int linha)
{
    /* Buraco fica so na calcada, nunca em rua ou alagamento. */
    if (linha < LINHAS_FASE_3 || LinhaEhRua(linha) || LinhaEhAlagamento(linha)) {
        return;
    }

    AdicionarObstaculo(
        &jogo->obstaculos,
        CriarObstaculo(TIPO_BURACO, coluna * TAM_BLOCO, linha * TAM_BLOCO, 0, 0)
    );
}

static void ConfigurarObstaculos(Jogo *jogo)
{
    /* Aqui ficam as posicoes principais dos obstaculos.
       usaDeslocamentoFase = true joga a linha para a parte nova do mapa. */
    ObstaculoMovel moveis[] = {
        {TIPO_CARRO, -120, 12, 235, 1, false, false}, {TIPO_ONIBUS, -540, 12, 0, 1, false, false},
        {TIPO_MOTO, 130, 11, 0, -1, false, true}, {TIPO_CARRO, 480, 11, 225, -1, false, true},
        {TIPO_MOTO, -180, 9, 0, 1, false, false}, {TIPO_CARRO, -520, 9, 245, 1, false, false},
        {TIPO_CARRO, 160, 8, 235, -1, false, true}, {TIPO_ONIBUS, 560, 8, 0, -1, false, true},
        {TIPO_CARRO, -140, 6, 250, 1, false, false}, {TIPO_MOTO, -500, 6, 0, 1, false, false},
        {TIPO_MOTO, 170, 5, 0, -1, false, true}, {TIPO_CARRO, 520, 5, 240, -1, false, true},
        {TIPO_CARRO, -170, 3, 255, 1, false, false}, {TIPO_ONIBUS, -610, 3, 0, 1, false, false},
        {TIPO_MOTO, 190, 2, 0, -1, false, true}, {TIPO_CARRO, 540, 2, 245, -1, false, true},
        {TIPO_CARRO, -380, 12, 255, 1, false, false}, {TIPO_MOTO, 420, 8, 0, -1, false, true},
        {TIPO_ONIBUS, -820, 6, 0, 1, false, false}, {TIPO_CARRO, 760, 3, 260, -1, false, true},
        {TIPO_LIXO_GRANDE, -120, 13, 95, 1, false, false}, {TIPO_LIXO_GRANDE, -560, 13, 95, 1, false, false},
        {TIPO_LIXO_GRANDE, 120, 10, 105, -1, false, true}, {TIPO_LIXO_GRANDE, 560, 10, 105, -1, false, true},
        {TIPO_LIXO_GRANDE, -180, 7, 115, 1, false, false}, {TIPO_LIXO_GRANDE, -650, 7, 115, 1, false, false},
        {TIPO_LIXO_GRANDE, 160, 4, 125, -1, false, true}, {TIPO_LIXO_GRANDE, 620, 4, 125, -1, false, true},
        {TIPO_LIXO_GRANDE, -220, 1, 135, 1, false, false}, {TIPO_LIXO_GRANDE, -760, 1, 135, 1, false, false},

        {TIPO_CARRO, -100, 41, 165, 1, true, false}, {TIPO_CARRO, -460, 41, 165, 1, true, false},
        {TIPO_CARRO, 80, 40, 155, -1, true, true}, {TIPO_ONIBUS, 430, 40, 0, -1, true, true},
        {TIPO_ONIBUS, -220, 38, 0, 1, true, false}, {TIPO_CARRO, -620, 38, 175, 1, true, false},
        {TIPO_CARRO, 120, 37, 170, -1, true, true}, {TIPO_CARRO, 520, 37, 170, -1, true, true},
        {TIPO_CARRO, -160, 35, 185, 1, true, false}, {TIPO_ONIBUS, -560, 35, 0, 1, true, false},
        {TIPO_CARRO, 200, 34, 175, -1, true, true}, {TIPO_CARRO, 610, 34, 175, -1, true, true},
        {TIPO_CARRO, -140, 31, 190, 1, true, false}, {TIPO_CARRO, -520, 31, 190, 1, true, false},
        {TIPO_ONIBUS, 220, 30, 0, -1, true, true}, {TIPO_CARRO, 650, 30, 185, -1, true, true},
        {TIPO_CARRO, -180, 28, 200, 1, true, false}, {TIPO_ONIBUS, -640, 28, 0, 1, true, false},
        {TIPO_CARRO, 160, 27, 190, -1, true, true}, {TIPO_CARRO, 540, 27, 190, -1, true, true},
        {TIPO_ONIBUS, -220, 25, 0, 1, true, false}, {TIPO_CARRO, -660, 25, 205, 1, true, false},
        {TIPO_CARRO, 100, 24, 195, -1, true, true}, {TIPO_ONIBUS, 500, 24, 0, -1, true, true},
        {TIPO_MOTO, -840, 41, 0, 1, true, false}, {TIPO_CARRO, 820, 37, 185, -1, true, true},
        {TIPO_MOTO, -880, 31, 0, 1, true, false}, {TIPO_CARRO, 900, 27, 205, -1, true, true},
        {TIPO_ONIBUS, -980, 24, 0, 1, true, false},

        {TIPO_CARRO, -120, 21, 205, 1, true, false}, {TIPO_MOTO, -480, 21, 0, 1, true, false},
        {TIPO_CARRO, 140, 20, 195, -1, true, true}, {TIPO_MOTO, 500, 20, 0, -1, true, true},
        {TIPO_MOTO, -180, 18, 0, 1, true, false}, {TIPO_CARRO, -560, 18, 215, 1, true, false},
        {TIPO_CARRO, 160, 17, 205, -1, true, true}, {TIPO_MOTO, 540, 17, 0, -1, true, true},
        {TIPO_CARRO, -150, 15, 220, 1, true, false}, {TIPO_MOTO, -510, 15, 0, 1, true, false},
        {TIPO_MOTO, 150, 14, 0, -1, true, true}, {TIPO_CARRO, 520, 14, 215, -1, true, true},
        {TIPO_CARRO, -160, 12, 225, 1, true, false}, {TIPO_MOTO, -530, 12, 0, 1, true, false},
        {TIPO_CARRO, 170, 11, 215, -1, true, true}, {TIPO_MOTO, 540, 11, 0, -1, true, true},
        {TIPO_MOTO, -190, 9, 0, 1, true, false}, {TIPO_CARRO, -560, 9, 230, 1, true, false},
        {TIPO_CARRO, 170, 8, 220, -1, true, true}, {TIPO_MOTO, 530, 8, 0, -1, true, true},
        {TIPO_CARRO, -160, 6, 230, 1, true, false}, {TIPO_MOTO, -520, 6, 0, 1, true, false},
        {TIPO_MOTO, 170, 5, 0, -1, true, true}, {TIPO_CARRO, 540, 5, 225, -1, true, true},
        {TIPO_CARRO, -160, 3, 235, 1, true, false}, {TIPO_MOTO, -540, 3, 0, 1, true, false},
        {TIPO_CARRO, 180, 2, 225, -1, true, true}, {TIPO_MOTO, 540, 2, 0, -1, true, true},
        {TIPO_MOTO, -850, 18, 0, 1, true, false}, {TIPO_CARRO, 880, 15, 240, -1, true, true},
        {TIPO_ONIBUS, -940, 12, 0, 1, true, false}, {TIPO_CARRO, 870, 8, 245, -1, true, true},
        {TIPO_MOTO, -900, 3, 0, 1, true, false},

        {TIPO_CACHORRO, -520, 43, 0, 1, true, false},
        {TIPO_CACHORRO, 640, 39, 0, -1, true, true},
        {TIPO_CACHORRO, -760, 32, 0, 1, true, false},
        {TIPO_CACHORRO, 820, 26, 0, -1, true, true},
        {TIPO_CACHORRO, -980, 44, 0, 1, true, false},
        {TIPO_CACHORRO, 1040, 36, 0, -1, true, true}
    };
    PosicaoMapa buracos[] = {
        {3, 0, true}, {11, 1, true}, {17, 4, true},
        {2, 7, true}, {14, 10, true}, {6, 13, true},
        {12, 16, true}, {4, 19, true}, {16, 22, true}
    };
    PosicaoMapa postes[] = {
        {2, 14, false}, {8, 13, false}, {14, 10, false}, {6, 7, false},
        {3, 4, false}, {6, 1, false}, {6, 42, true}, {13, 43, true},
        {4, 39, true}, {15, 36, true}, {6, 32, true}, {13, 29, true},
        {10, 23, true}, {16, 19, true}, {8, 13, true}, {17, 10, true},
        {2, 7, true}, {11, 4, true}
    };
    ObstaculoFixo fixos[] = {
        {TIPO_ARVORE, 3, 42, true}, {TIPO_GUARDA_SOL, 12, 42, true},
        {TIPO_ARVORE, 16, 39, true}, {TIPO_GUARDA_CHUVA_FREVO, 5, 36, true},
        {TIPO_ARVORE, 14, 32, true}, {TIPO_GUARDA_SOL, 7, 29, true},
        {TIPO_ARVORE, 17, 26, true}, {TIPO_GUARDA_CHUVA_FREVO, 4, 23, true},
        {TIPO_ARVORE, 15, 19, true}, {TIPO_GUARDA_SOL, 6, 16, true},
        {TIPO_ARVORE, 13, 13, true}, {TIPO_GUARDA_CHUVA_FREVO, 3, 10, true},
        {TIPO_ARVORE, 16, 7, true}, {TIPO_GUARDA_SOL, 8, 4, true},
        {TIPO_ARVORE, 2, 44, true}, {TIPO_GUARDA_CHUVA_FREVO, 6, 44, true},
        {TIPO_ARVORE, 15, 44, true}, {TIPO_GUARDA_SOL, 18, 43, true},
        {TIPO_GUARDA_CHUVA_FREVO, 9, 42, true}, {TIPO_GUARDA_SOL, 10, 39, true},
        {TIPO_ARVORE, 1, 35, true}, {TIPO_GUARDA_CHUVA_FREVO, 11, 35, true},
        {TIPO_GUARDA_SOL, 2, 32, true}, {TIPO_ARVORE, 9, 26, true},
        {TIPO_GUARDA_CHUVA_FREVO, 12, 20, true}, {TIPO_GUARDA_SOL, 1, 19, true},
        {TIPO_ARVORE, 10, 16, true}, {TIPO_GUARDA_CHUVA_FREVO, 18, 13, true},
        {TIPO_GUARDA_SOL, 11, 10, true}, {TIPO_ARVORE, 5, 7, true},
        {TIPO_GUARDA_CHUVA_FREVO, 14, 4, true},
        {TIPO_ARVORE, 19, 42, true}, {TIPO_GUARDA_SOL, 4, 39, true},
        {TIPO_GUARDA_CHUVA_FREVO, 16, 32, true}, {TIPO_ARVORE, 7, 20, true},
        {TIPO_GUARDA_SOL, 15, 7, true}
    };

    LiberarObstaculos(&jogo->obstaculos);
    jogo->obstaculos = NULL;

    for (int i = 0; i < (int)(sizeof(moveis) / sizeof(moveis[0])); i++) {
        /* Alguns carros comecam depois da tela para nao aparecer tudo junto. */
        float x = moveis[i].xDepoisDaTela ? LARGURA_TELA + moveis[i].x : moveis[i].x;

        if (moveis[i].usaDeslocamentoFase) {
            AdicionarObstaculoNaLinha(jogo, moveis[i].tipo, x, moveis[i].linha, moveis[i].velocidade, moveis[i].direcao);
        } else {
            AdicionarObstaculoNaLinhaMapa(jogo, moveis[i].tipo, x, moveis[i].linha, moveis[i].velocidade, moveis[i].direcao);
        }
    }

    for (int i = 0; i < (int)(sizeof(buracos) / sizeof(buracos[0])); i++) {
        if (buracos[i].usaDeslocamentoFase) {
            AdicionarBuracoMapa(jogo, buracos[i].coluna, buracos[i].linha + LINHAS_FASE_3);
        } else {
            AdicionarBuracoMapa(jogo, buracos[i].coluna, buracos[i].linha);
        }
    }

    for (int i = 0; i < (int)(sizeof(fixos) / sizeof(fixos[0])); i++) {
        int linha = fixos[i].usaDeslocamentoFase ? fixos[i].linha + LINHAS_FASE_3 : fixos[i].linha;
        AdicionarObstaculoFixoMapa(jogo, fixos[i].tipo, fixos[i].coluna, linha);
    }

    for (int i = 0; i < (int)(sizeof(postes) / sizeof(postes[0])); i++) {
        int linha = postes[i].usaDeslocamentoFase ? postes[i].linha + LINHAS_FASE_3 : postes[i].linha;
        AdicionarObstaculoFixoMapa(jogo, TIPO_POSTE, postes[i].coluna, linha);
    }
}

static void ConfigurarFase(Jogo *jogo)
{
    ConfigurarObstaculos(jogo);
    ConfigurarMoedas(jogo);
}

static void AtualizarMoedas(Jogo *jogo)
{
    for (int i = 0; i < jogo->totalMoedas; i++) {
        if (ColetarMoeda(&jogo->moedas[i], jogo->jogador.corpo)) {
            jogo->jogador.score += jogo->moedas[i].valor;
            jogo->moedasColetadas++;
            AtualizarRecorde(jogo);
        }
    }
}

static void DesenharMoedasJogo(Jogo *jogo)
{
    for (int i = 0; i < jogo->totalMoedas; i++) {
        DesenharMoeda(jogo->moedas[i]);
    }
}

static void AtualizarFase(Jogo *jogo)
{
    /* A fase atual depende da linha em que o jogador esta no mapa grande. */
    if (jogo->jogador.linha < LINHAS_FASE_3) {
        jogo->faseAtual = 4;
        return;
    }

    if (jogo->jogador.linha <= 22 + LINHAS_FASE_3) {
        jogo->faseAtual = 3;
        return;
    }

    jogo->faseAtual = jogo->jogador.linha <= 44 ? 2 : 1;
}

static void PerderJogo(Jogo *jogo)
{
    jogo->venceu = false;
    jogo->gameOver = true;
    jogo->jogador.tempoInicioMorte = GetTime();
}

static void AtualizarCaronaAlagamento(Jogo *jogo)
{
    float velocidadeApoio;

    /* Na agua o jogador precisa estar em cima de uma plataforma. */
    if (!LinhaEhAlagamento(jogo->jogador.linha)) {
        return;
    }

    velocidadeApoio = ObterVelocidadeApoioAlagamento(jogo->obstaculos, jogo->jogador.corpo);

    if (velocidadeApoio == 0.0f) {
        PerderJogo(jogo);
        return;
    }

    /* A plataforma leva o jogador junto, tipo uma esteira. */
    jogo->jogador.corpo.x += velocidadeApoio * GetFrameTime();
    jogo->jogador.coluna = (int)((jogo->jogador.corpo.x - MARGEM_JOGADOR + TAM_BLOCO * 0.5f) / TAM_BLOCO);

    /* Se a plataforma carregar o jogador para fora da tela, ele perde. */
    if (jogo->jogador.corpo.x + jogo->jogador.corpo.width < 0 ||
        jogo->jogador.corpo.x > LARGURA_TELA) {
        PerderJogo(jogo);
    }
}

void IniciarJogo(Jogo *jogo)
{
    IniciarJogador(&jogo->jogador);
    jogo->faseAtual = 1;
    jogo->moedasColetadas = 0;
    ConfigurarFase(jogo);
    jogo->gameOver = false;
    jogo->jogoIniciado = false;
    jogo->venceu = false;
    jogo->pausado = false;
    jogo->recorde = CarregarRecorde();
}

void AtualizarJogo(Jogo *jogo)
{
    Jogador jogadorAntes;
    DirecaoJogador direcaoAposEntrada;
    double tempoAtaqueAposEntrada;

    if (IsKeyPressed(KEY_P)) {
        jogo->pausado = !jogo->pausado;
    }

    if (jogo->pausado) {
        return;
    }

    jogadorAntes = jogo->jogador;
    AtualizarJogador(&jogo->jogador);
    direcaoAposEntrada = jogo->jogador.direcao;
    tempoAtaqueAposEntrada = jogo->jogador.tempoUltimoAtaque;

    /* Se bater em arvore/poste/etc, volta para onde estava antes do movimento. */
    if (VerificarColisaoFixaLista(jogo->obstaculos, jogo->jogador.corpo)) {
        jogo->jogador.linha = jogadorAntes.linha;
        jogo->jogador.coluna = jogadorAntes.coluna;
        jogo->jogador.corpo = jogadorAntes.corpo;
        jogo->jogador.score = jogadorAntes.score;
        jogo->jogador.melhorLinha = jogadorAntes.melhorLinha;
        jogo->jogador.tempoUltimoMovimento = jogadorAntes.tempoUltimoMovimento;
        jogo->jogador.direcao = direcaoAposEntrada;
        jogo->jogador.tempoUltimoAtaque = tempoAtaqueAposEntrada;
    }

    AtualizarRecorde(jogo);
    AtualizarMoedas(jogo);

    AtualizarListaObstaculos(jogo->obstaculos);

    if (VerificarColisaoLista(jogo->obstaculos, jogo->jogador.corpo)) {
        PerderJogo(jogo);
    }

    AtualizarCaronaAlagamento(jogo);

    AtualizarFase(jogo);

    if (jogo->jogador.linha == 0) {
        jogo->venceu = true;
        jogo->gameOver = true;
    }
}

void DesenharJogo(Jogo *jogo)
{
    Camera2D camera = {0};
    float alturaMapa = TOTAL_LINHAS * TAM_BLOCO;
    float cameraY = jogo->jogador.corpo.y - ALTURA_TELA * 0.65f;

    if (!jogo->jogoIniciado) {
        DesenharTelaInicial();
        return;
    }

    /* A camera segue o jogador, mas nao deixa mostrar fora do mapa. */
    cameraY = LimitarFloat(cameraY, 0, alturaMapa - ALTURA_TELA);
    camera.target = (Vector2){0, cameraY};
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0;
    camera.zoom = 1;

    BeginMode2D(camera);
    DesenharMapa();
    DesenharMoedasJogo(jogo);
    DesenharListaObstaculos(jogo->obstaculos);
    DesenharJogador(jogo->jogador, jogo->gameOver && !jogo->venceu);
    EndMode2D();

    DesenharInterface(jogo);

    if (jogo->gameOver) {
        DesenharGameOver(jogo);
    }
}

void DesenharTelaInicial(void)
{
    ClearBackground(DARKGREEN);
    DrawText("CAOS EM RECIFE", X_TITULO_INICIAL, Y_TITULO_INICIAL, 40, RAYWHITE);
    DrawText("Pressione ENTER para jogar", X_TEXTO_INICIAL, Y_TEXTO_INICIAL, 24, YELLOW);
}

void DesenharInterface(Jogo *jogo)
{
    DrawText(TextFormat("Score: %d", jogo->jogador.score), X_INTERFACE, Y_INTERFACE_TITULOS, 20, BLACK);
    DrawText(TextFormat("Recorde: %d", jogo->recorde), 190, Y_INTERFACE_TITULOS, 20, BLACK);
    DrawText(TextFormat("Fase: %d", jogo->faseAtual), 360, Y_INTERFACE_TITULOS, 20, BLACK);
    DrawText(TextFormat("Moedas: %d", jogo->moedasColetadas), 480, Y_INTERFACE_TITULOS, 20, BLACK);
    DrawText("WASD para mover", X_INTERFACE, Y_INTERFACE_INFORMACOES, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("R para reiniciar", 220, Y_INTERFACE_INFORMACOES, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("P para pausar", 400, Y_INTERFACE_INFORMACOES, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("Cuidado com alagamentos", 530, Y_INTERFACE_INFORMACOES, TAM_TEXTO_INTERFACE, BLACK);

    if (jogo->pausado) {
        DrawText("PAUSADO", 335, 300, 34, YELLOW);
    }
}

void DesenharGameOver(Jogo *jogo)
{
    if (jogo->venceu) {
        DrawText("VOCE VENCEU", 250, 250, 40, YELLOW);
    } else {
        DrawText("VOCE PERDEU", 260, 210, 40, RED);
    }
    DrawText(TextFormat("Score da partida: %d", jogo->jogador.score), 250, 275, 24, WHITE);
    DrawText(TextFormat("Moedas coletadas: %d", jogo->moedasColetadas), 250, 310, 24, GOLD);
    DrawText("Deseja jogar novamente?", 245, 365, 24, WHITE);
    DrawText("Pressione R para sim", 275, 400, 22, YELLOW);
}
