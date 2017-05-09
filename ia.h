#ifndef _IA_H
#define _IA_H

#include "utils.h"

#define clear() printf("\033[H\033[J")
#define FIRST_COLOR 1
#define COUNTED 1
#define INITIAL_DISTANCE -1

/*

[
    1, 2,
    3, 4,
    5, 6,
]

[ 1, 2, 3, 4, 5, 6 ]

Linhas = 3
Colunas = 2

m[1,1] = 4;
v[Id(1,1) == 1*2+1 == 3] = 4;

m[2,0] = 5;
v[Id(2,0) == 2*2+0 == 4] = 5;

*/

int Njogadas, Linhas, Colunas;

typedef struct {
    int nlinhas;
    int ncolunas;
    int ncores;
    int tam;
    celula *mapa;
} tmapa;

int ID(int i, int j);
void gera_mapa(tmapa *m, int semente);
void carrega_mapa(tmapa *m);
void mostra_mapa(tmapa *m);
void mostra_mapa_cor(tmapa *m, int shouldClear);
void pinta(tmapa *m, int l, int c, int fundo, int cor);
void pinta_mapa(tmapa *m, int cor);
int acabou(tmapa m);
int borda(int i, int j);
void teste_lista();
void zera_counted(tmapa *m);
void cria_vertices(tmapa *m, grafo g);
void adiciona_vizinhanca(tmapa *m, int i, int j, vertice v);
void cria_arestas(tmapa *m, grafo g);
void pega_vizinhos(tmapa *m, int i, int j, vertice v);
grafo cria_grafos(tmapa *m);
int jogada_random(tmapa m);
int guloso(tmapa m, grafo g);
int proxima_jogada(tmapa m, grafo g);

#endif
