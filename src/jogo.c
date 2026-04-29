#include "jogo.h"
#include "mapa.h"
#include "config.h"
#include "score.h"

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

    if (jogo->faseAtual == 1) {
        AdicionarMoedaFase(jogo, 4, 11);
        AdicionarMoedaFase(jogo, 14, 7);
        AdicionarMoedaFase(jogo, 8, 3);
        return;
    }

    AdicionarMoedaFase(jogo, 2, 12);
    AdicionarMoedaFase(jogo, 16, 8);
    AdicionarMoedaFase(jogo, 5, 6);
    AdicionarMoedaFase(jogo, 12, 2);
}

static void ConfigurarObstaculos(Jogo *jogo)
{
    LiberarObstaculos(&jogo->obstaculos);
    jogo->obstaculos = NULL;

    AdicionarObstaculo(&jogo->obstaculos, CriarObstaculo(TIPO_CARRO, X_INICIAL_CARRO, 280, VELOCIDADE_CARRO, 1));
    AdicionarObstaculo(&jogo->obstaculos, CriarObstaculo(TIPO_CARRO, LARGURA_TELA + 80, 360, 130, -1));
    AdicionarObstaculo(&jogo->obstaculos, CriarObstaculo(TIPO_ONIBUS, -220, 200, VELOCIDADE_CARRO, 1));

    if (jogo->faseAtual >= 2) {
        AdicionarObstaculo(&jogo->obstaculos, CriarObstaculo(TIPO_MOTO, LARGURA_TELA + 120, 240, 0, -1));
        AdicionarObstaculo(&jogo->obstaculos, CriarObstaculo(TIPO_MOTO, -140, 320, 0, 1));
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
    if (jogo->jogador.linha != 0) {
        jogo->tocouTopoNestaFase = false;
        return;
    }

    if (jogo->tocouTopoNestaFase) {
        return;
    }

    jogo->tocouTopoNestaFase = true;

    if (jogo->faseAtual < TOTAL_FASES_ATUAIS) {
        jogo->faseAtual++;
        ConfigurarFase(jogo);
    }
}

void IniciarJogo(Jogo *jogo)
{
    IniciarJogador(&jogo->jogador);
    jogo->faseAtual = 1;
    jogo->tocouTopoNestaFase = false;
    ConfigurarFase(jogo);
    jogo->gameOver = false;
    jogo->jogoIniciado = false;
    jogo->venceu = false;
    jogo->pausado = false;
    jogo->recorde = CarregarRecorde();
}

void AtualizarJogo(Jogo *jogo)
{
    if (IsKeyPressed(KEY_P)) {
        jogo->pausado = !jogo->pausado;
    }

    if (jogo->pausado) {
        return;
    }

    AtualizarJogador(&jogo->jogador);
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
    if (!jogo->jogoIniciado) {
        DesenharTelaInicial();
        return;
    }

    DesenharMapa();
    DesenharMoedasJogo(jogo);
    DesenharListaObstaculos(jogo->obstaculos);
    DesenharJogador(jogo->jogador);

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
