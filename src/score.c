#include "score.h"
#include <stdio.h>

#define ARQUIVO_SCORE "scores.txt"

int CarregarRecorde(void)
{
    int recorde = 0;
    FILE *arquivo = fopen(ARQUIVO_SCORE, "r"); /* leitura em arquivo */

    if (arquivo == NULL) {
        return 0;
    }

    if (fscanf(arquivo, "%d", &recorde) != 1) {
        recorde = 0;
    }

    fclose(arquivo);
    return recorde;
}

void SalvarRecorde(int score)
{
    FILE *arquivo = fopen(ARQUIVO_SCORE, "w"); /* escrita em arquivo */

    if (arquivo == NULL) {
        return;
    }

    fprintf(arquivo, "%d\n", score);
    fclose(arquivo);
}
