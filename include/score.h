#ifndef SCORE_H
#define SCORE_H

#define TOTAL_RECORDES 5

int CarregarRecorde(void);
void SalvarRecorde(int score);
void CarregarTopScores(int scores[], int total);
void SalvarTopScores(int scores[], int total);

#endif
