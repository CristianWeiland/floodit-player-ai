#ifndef _BLOCO_H
#define _BLOCO_H

#define STEP_X_1 2
#define STEP_Y_1 2
#define STEP_X_2 4

#define STEP_Y_2 4
#define STEP_X_3 10
#define STEP_Y_3 10

// Provavelmente eu quero fazer alguma conta a partir de Nblocos
#define DECR_PESO_1 1
#define DECR_PESO_2 2
#define DECR_PESO_3 2

#define MAIOR_PESO_1 8
#define MAIOR_PESO_2 100
#define MAIOR_PESO_3 1000

int MaiorPeso, DecrPeso;
bloco *Bloco;

vertice vertice_menor_distancia(tmapa *m, int x1, int y1, int x2, int y2);
void bloco_baixo_direita(int *x1, int *y1, int *x2, int *y2);
int menor_caminho(tmapa *m, grafo g, vertice v, int **jogadas);

void define_n_blocos(tmapa *m, int stepx, int stepy);
void conta_blocos(tmapa *m);
void define_pesos_blocos();
int bloco_calcula_cor(tmapa *m);

#endif
