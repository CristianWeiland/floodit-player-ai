#ifndef _IA_H
#define _IA_H

#include "utils.h"
#include "mapa.h"
#include "grafo.h"

vertice vertice_menor_distancia(tmapa *m, int x1, int y1, int x2, int y2);
int menor_caminho(tmapa *m, grafo g, vertice v, int **jogadas);
void bloco_baixo_direita(int *x1, int *y1, int *x2, int *y2);
int resolve(tmapa *m);

#endif
