#include "score.h"
#include <stdio.h>

#define ARQUIVO_SCORES "scores.txt"

static void ZerarScores(int scores[], int total)
{
    for (int i = 0; i < total; i++) {
        scores[i] = 0;
    }
}

static void OrdenarDoMaiorParaOMenor(int scores[], int total)
{
    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {
            if (scores[j] > scores[i]) {
                int troca = scores[i];
                scores[i] = scores[j];
                scores[j] = troca;
            }
        }
    }
}

void CarregarTopScores(int scores[], int total)
{
    FILE *arquivo = fopen(ARQUIVO_SCORES, "r"); /* leitura em arquivo */

    ZerarScores(scores, total);

    if (arquivo == NULL) {
        return;
    }

    for (int i = 0; i < total; i++) {
        if (fscanf(arquivo, "%d", &scores[i]) != 1) {
            scores[i] = 0;
        }
    }

    fclose(arquivo);
    OrdenarDoMaiorParaOMenor(scores, total);
}

void SalvarTopScores(int scores[], int total)
{
    FILE *arquivo = fopen(ARQUIVO_SCORES, "w"); /* escrita em arquivo */

    if (arquivo == NULL) {
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%d\n", scores[i]);
    }

    fclose(arquivo);
}

int CarregarRecorde(void)
{
    int scores[TOTAL_RECORDES];

    CarregarTopScores(scores, TOTAL_RECORDES);
    return scores[0];
}

void SalvarRecorde(int score)
{
    int scores[TOTAL_RECORDES];

    CarregarTopScores(scores, TOTAL_RECORDES);

    if (score > scores[TOTAL_RECORDES - 1]) {
        scores[TOTAL_RECORDES - 1] = score;
        OrdenarDoMaiorParaOMenor(scores, TOTAL_RECORDES);
        SalvarTopScores(scores, TOTAL_RECORDES);
    }
}
