#include "jogo.h"
#include "mapa.h"
#include "config.h"
#include "score.h"

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

static void AdicionarMoedaFase(Jogo *jogo, int coluna, int linha)
{
    if (jogo->totalMoedas >= MAX_MOEDAS) {
        return;
    }

    IniciarMoeda(&jogo->moedas[jogo->totalMoedas], coluna, linha);
    jogo->totalMoedas++;
}

static void ConfigurarMoedas(Jogo *jogo)
{
    jogo->totalMoedas = 0;

    AdicionarMoedaFase(jogo, 4, 41);
    AdicionarMoedaFase(jogo, 15, 38);
    AdicionarMoedaFase(jogo, 6, 35);
    AdicionarMoedaFase(jogo, 13, 32);
    AdicionarMoedaFase(jogo, 8, 29);
    AdicionarMoedaFase(jogo, 18, 23);
    AdicionarMoedaFase(jogo, 2, 19);
    AdicionarMoedaFase(jogo, 14, 16);
    AdicionarMoedaFase(jogo, 4, 13);
    AdicionarMoedaFase(jogo, 12, 10);
    AdicionarMoedaFase(jogo, 6, 7);
    AdicionarMoedaFase(jogo, 17, 4);
}

static void AdicionarObstaculoNaLinha(Jogo *jogo, TipoObstaculo tipo, float x, int linha, float velocidade, int direcao)
{
    AdicionarObstaculo(
        &jogo->obstaculos,
        CriarObstaculo(tipo, x, linha * TAM_BLOCO + 5, velocidade, direcao)
    );
}

static void AdicionarObstaculoFixo(Jogo *jogo, TipoObstaculo tipo, int coluna, int linha)
{
    AdicionarObstaculo(
        &jogo->obstaculos,
        CriarObstaculo(tipo, coluna * TAM_BLOCO, linha * TAM_BLOCO, 0, 0)
    );
}

static void ConfigurarObstaculos(Jogo *jogo)
{
    LiberarObstaculos(&jogo->obstaculos);
    jogo->obstaculos = NULL;

    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -100, 43, 165, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -460, 43, 165, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 80, 42, 155, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, LARGURA_TELA + 430, 42, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, -220, 40, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -620, 40, 175, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 120, 39, 170, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 520, 39, 170, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -160, 37, 185, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, -560, 37, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 200, 36, 175, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 610, 36, 175, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, -260, 34, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -700, 34, 190, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 100, 33, 180, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, LARGURA_TELA + 500, 33, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -140, 31, 190, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -520, 31, 190, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, LARGURA_TELA + 220, 30, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 650, 30, 185, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -180, 28, 200, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, -640, 28, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 160, 27, 190, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 540, 27, 190, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, -220, 25, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -660, 25, 205, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 100, 24, 195, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_ONIBUS, LARGURA_TELA + 500, 24, 0, -1);

    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -120, 21, 205, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, -480, 21, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 140, 20, 195, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, LARGURA_TELA + 500, 20, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, -180, 18, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -560, 18, 215, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 160, 17, 205, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, LARGURA_TELA + 540, 17, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -150, 15, 220, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, -510, 15, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, LARGURA_TELA + 150, 14, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 520, 14, 215, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -160, 12, 225, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, -530, 12, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 170, 11, 215, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, LARGURA_TELA + 540, 11, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, -190, 9, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -560, 9, 230, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 170, 8, 220, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, LARGURA_TELA + 530, 8, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -160, 6, 230, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, -520, 6, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, LARGURA_TELA + 170, 5, 0, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 540, 5, 225, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, -160, 3, 235, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, -540, 3, 0, 1);
    AdicionarObstaculoNaLinha(jogo, TIPO_CARRO, LARGURA_TELA + 180, 2, 225, -1);
    AdicionarObstaculoNaLinha(jogo, TIPO_MOTO, LARGURA_TELA + 540, 2, 0, -1);

    AdicionarObstaculoFixo(jogo, TIPO_ARVORE, 3, 41);
    AdicionarObstaculoFixo(jogo, TIPO_PEDRA, 12, 41);
    AdicionarObstaculoFixo(jogo, TIPO_ARVORE, 16, 38);
    AdicionarObstaculoFixo(jogo, TIPO_PEDRA, 5, 35);
    AdicionarObstaculoFixo(jogo, TIPO_ARVORE, 14, 32);
    AdicionarObstaculoFixo(jogo, TIPO_PEDRA, 7, 29);
    AdicionarObstaculoFixo(jogo, TIPO_ARVORE, 17, 26);
    AdicionarObstaculoFixo(jogo, TIPO_PEDRA, 4, 23);
    AdicionarObstaculoFixo(jogo, TIPO_ARVORE, 15, 19);
    AdicionarObstaculoFixo(jogo, TIPO_PEDRA, 6, 16);
    AdicionarObstaculoFixo(jogo, TIPO_ARVORE, 13, 13);
    AdicionarObstaculoFixo(jogo, TIPO_PEDRA, 3, 10);
    AdicionarObstaculoFixo(jogo, TIPO_ARVORE, 16, 7);
    AdicionarObstaculoFixo(jogo, TIPO_PEDRA, 8, 4);
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
    jogo->faseAtual = jogo->jogador.linha <= 22 ? 2 : 1;
}

void IniciarJogo(Jogo *jogo)
{
    IniciarJogador(&jogo->jogador);
    jogo->faseAtual = 1;
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

    if (IsKeyPressed(KEY_P)) {
        jogo->pausado = !jogo->pausado;
    }

    if (jogo->pausado) {
        return;
    }

    jogadorAntes = jogo->jogador;
    AtualizarJogador(&jogo->jogador);

    if (VerificarColisaoFixaLista(jogo->obstaculos, jogo->jogador.corpo)) {
        jogo->jogador = jogadorAntes;
    }

    AtualizarRecorde(jogo);
    AtualizarMoedas(jogo);

    AtualizarListaObstaculos(jogo->obstaculos);

    if (VerificarColisaoLista(jogo->obstaculos, jogo->jogador.corpo)) {
        VoltarJogadorCheckpoint(&jogo->jogador);
    }

    AtualizarFase(jogo);
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

    cameraY = LimitarFloat(cameraY, 0, alturaMapa - ALTURA_TELA);
    camera.target = (Vector2){0, cameraY};
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0;
    camera.zoom = 1;

    BeginMode2D(camera);
    DesenharMapa();
    DesenharMoedasJogo(jogo);
    DesenharListaObstaculos(jogo->obstaculos);
    DesenharJogador(jogo->jogador);
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
    DrawRectangle(0, 0, LARGURA_TELA, 82, Fade(RAYWHITE, 0.75f));
    DrawText(TextFormat("Score: %d", jogo->jogador.score), X_INTERFACE, Y_SCORE, 20, BLACK);
    DrawText(TextFormat("Recorde: %d", jogo->recorde), 140, Y_SCORE, 20, BLACK);
    DrawText(TextFormat("Fase: %d", jogo->faseAtual), 330, Y_SCORE, 20, BLACK);
    DrawText("WASD para mover", X_INTERFACE, Y_DICA_MOVIMENTO, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("R para reiniciar", X_INTERFACE, Y_DICA_REINICIO, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("P para pausar", 220, Y_DICA_REINICIO, TAM_TEXTO_INTERFACE, BLACK);
    DrawText("Evite veiculos e colete moedas", 480, 10, TAM_TEXTO_INTERFACE, BLACK);

    if (jogo->pausado) {
        DrawText("PAUSADO", 335, 285, 34, YELLOW);
    }
}

void DesenharGameOver(Jogo *jogo)
{
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.6f));
    if (jogo->venceu) {
        DrawText("VOCE VENCEU", 250, 250, 40, YELLOW);
    } else {
        DrawText("GAME OVER", 270, 250, 40, RED);
    }
    DrawText("Pressione R para reiniciar", 245, 310, 24, WHITE);
}
