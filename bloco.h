#ifndef _BLOCO_H
#define _BLOCO_H

#define MAIOR_PESO 10 // Provavelmente eu quero fazer alguma conta a partir de Nblocos
#define DECR_PESO 1

bloco *Bloco;

vertice vertice_menor_distancia(tmapa *m, int x1, int y1, int x2, int y2);
void bloco_baixo_direita(int *x1, int *y1, int *x2, int *y2);
int menor_caminho(tmapa *m, grafo g, vertice v, int **jogadas);

void define_n_blocos(tmapa *m, int stepx, int stepy);
void conta_blocos(tmapa *m);
void define_pesos_blocos();
int bloco_calcula_cor(tmapa *m);

#endif
