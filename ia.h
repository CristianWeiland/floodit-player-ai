#ifndef _IA_H
#define _IA_H

#include "utils.h"
#include "mapa.h"
#include "grafo.h"

int callCount = 0;

int jogada_random(tmapa m);

int guloso_fronteira_externa(tmapa *m);
void define_fronteira_vizinhos(tmapa *m, int i, int j);
void define_front_int_ext(tmapa *m);
int tipo_fronteira(tmapa *m, int i, int j, int cor);
int checa_condicoes(tmapa *m, int i, int j, int cor);

int guloso(tmapa m, grafo g);
int* quantos_faltam(tmapa *m, grafo g, int *ret);
int jogada_otima(tmapa *m, grafo g);
int proxima_jogada(tmapa m, grafo g);

#endif
