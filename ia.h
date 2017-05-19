#ifndef _IA_H
#define _IA_H

#include "utils.h"
#include "mapa.h"
#include "grafo.h"

//int callCount = 0, Count, Nblocos, *Restantes, *Pesos;
int callCount = 0, Count, Nblocos;
bloco *Bloco;

typedef struct {
    int restante, peso;
} bloco;

int jogada_random(tmapa m);

int guloso_fronteira_externa(tmapa *m);
void define_fronteira_vizinhos(tmapa *m, int i, int j);
void define_front_int_ext(tmapa *m);
int tipo_fronteira(tmapa *m, int i, int j, int cor);
int checa_condicoes(tmapa *m, int i, int j, int cor);

vertice vertice_menor_distancia(tmapa *m, int x1, int y1, int x2, int y2);
void bloco_baixo_direita(int *x1, int *y1, int *x2, int *y2);
int menor_caminho(tmapa *m, grafo g, vertice v, int **jogadas);

int guloso(tmapa m, grafo g);
int* quantos_faltam(tmapa *m, grafo g, int *ret);
int jogada_otima(tmapa *m, grafo g);
int proxima_jogada(tmapa m, grafo g);

#endif
