#include "score.h"
#include <stdio.h>

int CarregarRecorde(void)
{
    int recorde = 0;
    FILE *arquivo = fopen("scores.txt", "r");

    if (arquivo != NULL) {
        if (fscanf(arquivo, "%d", &recorde) != 1) {
            recorde = 0;
        }
        fclose(arquivo);
    } else {
        SalvarRecorde(recorde);
    }

    return recorde;
}

void SalvarRecorde(int score)
{
    FILE *arquivo = fopen("scores.txt", "w");

    if (arquivo != NULL) {
        fprintf(arquivo, "%d\n", score);
        fclose(arquivo);
    }
}
