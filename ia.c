#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

#define clear() printf("\033[H\033[J")

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

int Njogadas, Colunas;

typedef struct {
    int nlinhas;
    int ncolunas;
    int ncores;
    int tam;
    int *mapa;
} tmapa;

inline int ID(int i, int j) {
    return i * Colunas + j;
}

void gera_mapa(tmapa *m, int semente) {
    int i, j;

    if(semente < 0)
        srand(time(NULL));
    else
        srand(semente);

    m->mapa = (int*) malloc(m->nlinhas * m->ncolunas * sizeof(int));

    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++) {
            m->mapa[ID(i,j)] = 1 + rand() % m->ncores;
        }
    }
}

void carrega_mapa(tmapa *m) {
    int i, j;

    if(!scanf("%d", &(m->nlinhas)))
        puts("Erro lendo linhas.");
    if(!scanf("%d", &(m->ncolunas)))
        puts("Erro lendo colunas.");
    if(!scanf("%d", &(m->ncores)))
        puts("Erro lendo cores.");
    m->mapa = (int*) malloc(m->nlinhas * m->ncolunas * sizeof(int));
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++) {
            if(!scanf("%d", &(m->mapa[ID(i,j)])))
                puts("Erro lendo cor.");
        }
    }
}

void mostra_mapa(tmapa *m) {
    int i, j;

    clear();

    printf("Linhas: %d Colunas: %d Cores: %d\n\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
	printf("%02d ", m->mapa[ID(i,j)]);
            else
	printf("%d ", m->mapa[ID(i,j)]);
        printf("\n");
    }
}

void mostra_mapa_cor(tmapa *m) {
    int i, j;
    char* cor_ansi[] = { "\x1b[0m",
		             "\x1b[31m", "\x1b[32m", "\x1b[33m",
		             "\x1b[34m", "\x1b[35m", "\x1b[36m",
		             "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
		             "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
		             "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m" };

    clear();

    if(m->ncores > 15) {
        mostra_mapa(m);
        return;
    }
    printf("Linhas: %d Colunas: %d Cores: %d\n\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
	printf("%s%02d%s ", cor_ansi[m->mapa[ID(i,j)]], m->mapa[ID(i,j)], cor_ansi[0]);
            else
	printf("%s%d%s ", cor_ansi[m->mapa[ID(i,j)]], m->mapa[ID(i,j)], cor_ansi[0]);
        printf("\n");
    }
}

void pinta(tmapa *m, int l, int c, int fundo, int cor) {
    m->mapa[ID(l,c)] = cor;
    if(l < m->nlinhas - 1 && m->mapa[ID(l+1,c)] == fundo)
        pinta(m, l+1, c, fundo, cor);
    if(c < m->ncolunas - 1 && m->mapa[ID(l,c+1)] == fundo)
        pinta(m, l, c+1, fundo, cor);
    if(l > 0 && m->mapa[ID(l-1,c)] == fundo)
        pinta(m, l-1, c, fundo, cor);
    if(c > 0 && m->mapa[ID(l,c-1)] == fundo)
        pinta(m, l, c-1, fundo, cor);
}

void pinta_mapa(tmapa *m, int cor) {
    if(cor == m->mapa[ID(0,0)])
        return;
    pinta(m, 0, 0, m->mapa[ID(0,0)], cor);
}

int acabou(tmapa m) {
    int i, cor = m.mapa[0];
    for(i=1; i<m.tam; ++i) {
        if(m.mapa[i] != cor) {
            return 0;
        }
    }
    return 1;
}

void teste_lista() {
    lista l = constroi_lista();
    int *x;
    x = malloc(sizeof(int));
    *x = 12;
    insere_lista((void*) x, l);

    x = malloc(sizeof(int));
    *x = 22;
    insere_lista((void*) x, l);

    no elem;
    for(elem = l->primeiro; elem; elem = elem->proximo) {
        x = (int*) elem->conteudo;
        printf("Valor é %d\n", *x);
    }
}

int main(int argc, char **argv) {
    int cor;
    tmapa m;
    int semente;

    if(argc < 4 || argc > 5) {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }

    m.nlinhas = atoi(argv[1]);
    m.ncolunas = atoi(argv[2]);
    m.ncores = atoi(argv[3]);
    m.tam = m.nlinhas * m.ncolunas;

    Njogadas = 0;
    Colunas = m.ncolunas;

    if(argc == 5)
        semente = atoi(argv[4]);
    else
        semente = -1;

    gera_mapa(&m, semente);

    while(!acabou(m)) {
        mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
        printf("Qual sua proxima jogada? ");
        if(!scanf("%d", &cor))
            puts("Erro lendo jogada.");
        pinta_mapa(&m, cor);
        ++Njogadas;
    }

    mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);

    printf("Parabens! Voce venceu em %d jogadas!\n", Njogadas);

    return 0;
}
