#ifndef _MAPA_H
#define _MAPA_H

#include "utils.h"

#define clear() printf("\033[H\033[J")

#define FIRST_COLOR 1
#define COUNTED 1
#define INITIAL_DISTANCE -1

#define PRINT_COR 0
#define PRINT_BLOCO 1
#define PRINT_STATUS 2

#define STATUS_UNDF 0
#define STATUS_MAIN 1 // Quem faz parte do meu conjunto (noroeste)
#define STATUS_F 2
#define STATUS_F_INT 3
#define STATUS_F_EXT 4

int Njogadas, Linhas, Colunas;

typedef struct tmapa tmapa;

struct tmapa {
    int nlinhas;
    int ncolunas;
    int ncores;
    int tam;
    celula *mapa;
};

int ID(int i, int j);
void gera_mapa(tmapa *m, int semente);
void carrega_mapa(tmapa *m);
tmapa* copia_tmapa(tmapa *m);
void destroi_tmapa(tmapa *m, int destruir_vertices);
void mostra_mapa(tmapa *m);
void mostra_mapa_cor(tmapa *m, int shouldClear);
void mostra_mapa_status(tmapa *m);
void mostra_mapa_param(tmapa *m, int print);
void pinta(tmapa *m, int l, int c, int fundo, int cor);
void pinta_mapa(tmapa *m, int cor);
int acabou(tmapa m);
int borda(int i, int j);
void zera_status(tmapa *m);
void zera_counted(tmapa *m);
void zera_counted2(tmapa *m);
void flood_set_status(tmapa *m, int i, int j, int minha_cor, int status);

#endif
