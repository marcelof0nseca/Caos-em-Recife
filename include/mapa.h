#ifndef MAPA_H
#define MAPA_H

#include <stdbool.h>

void InicializarMapa(void);
void FinalizarMapa(void);
void DesenharMapa(void);
bool LinhaEhRua(int linha);
bool LinhaEhAlagamento(int linha);

#endif
