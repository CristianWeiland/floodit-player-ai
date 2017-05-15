#ifndef _GRAFO_H
#define _GRAFO_H

#include "utils.h"
#include "mapa.h"

vertice mais_proximo(tmapa *m, int x1, int y1, int x2, int y2);
void cria_vertices(tmapa *m, grafo g);
void adiciona_vizinhanca(tmapa *m, int i, int j, vertice v);
void cria_arestas(tmapa *m, grafo g);
void pega_vizinhos(tmapa *m, int i, int j, vertice v);
grafo cria_grafos(tmapa *m);

#endif
