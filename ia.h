#ifndef _IA_H
#define _IA_H

#include "utils.h"
#include "mapa.h"
#include "grafo.h"

int guloso(tmapa m, grafo g);
int* quantos_faltam(tmapa *m, grafo g, int *ret);
int jogada_otima(tmapa *m, grafo g);
int proxima_jogada(tmapa m, grafo g);

int resolve(tmapa *m, int algoritmo);
void joga(tmapa *m);

#endif
