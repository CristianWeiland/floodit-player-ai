#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ia.h"
#include "utils.h"
#include "mapa.h"
#include "grafo.h"
#include "fronteira.h"

vertice vertice_menor_distancia(tmapa *m, int x1, int y1, int x2, int y2) {
    int i, j, x, y;
    vertice menor = m->mapa[ID(x1,y1)]->v;
    x = x1;
    y = y1;
    for(i=x1; i<x2; ++i) {
        for(j=y1; j<y2; ++j) {
            if(m->mapa[ID(i,j)]->v->d < menor->d) {
                menor = m->mapa[ID(i,j)]->v;
                x = i;
                y = j;
            }
        }
    }
    return menor;
}

void bloco_baixo_direita(int *x1, int *y1, int *x2, int *y2) {
    *x1 = (int) (Linhas * 3) / 4;
    *y1 = (int) (Colunas * 3) / 4;
    *x2 = Linhas;
    *y2 = Colunas;
}

int menor_caminho(tmapa *m, grafo g, vertice v, int **jogadas) {
    // Cria um vetor de v->d jogadas, que fazem vc sair de g->lider e chegar em v.
    // Devolve a posição da primeira jogada (deveria ser 0). O vetor acab em v->d;

    int i, debug = 0;
    vertice w;
    aresta a;
    no elem;

    *jogadas = (int *) malloc(v->d * sizeof(int));

    if(debug) {
        mostra_mapa_cor(m, 0);
        escreve_grafo(stdout, g);
        printf("Comecando no vertice %d, cor %d, elems %d\n", v->id, v->cor, v->elems);
    }

    for(i = v->d-1, w = v; w != g->lider && i >= 0; --i) {
        // Adiciona a cor do vertice que eu quero pintar.
        (*jogadas)[i] = w->cor;
        if(debug)
            printf("Jogada = %d, elems = %d\n", (*jogadas)[i], w->elems);
        // Faz w apontar pro vertice anterior.
        elem = primeiro_no(w->entrada);
        if(!elem) {
            puts("(menor_caminho) Nao consegui achar caminho.");
            exit(1);
        }
        a = (aresta) conteudo(elem);
        w = a->vs;
    }

    if(i != -1) {
        puts("Funcao menor_caminho pode ter problema no i. Conferir please.");
    }
    return 0;
}

int resolve(tmapa *m) {
    int cor, i, first, last, *jogadas, x1, x2, y1, y2;
    grafo g;

    Njogadas = 0;

    // Anda Sudeste
    g = cria_grafos(m);
    bloco_baixo_direita(&x1, &y1, &x2, &y2);
    vertice v = vertice_menor_distancia(m, x1, y1, x2, y2);
    first = menor_caminho(m, g, v, &jogadas);
    last = first + v->d;
    for(i=first; i<last; ++i) {
        pinta_mapa(m, jogadas[i]);
        ++Njogadas;
        printf("%d ", jogadas[i]);
    }
    free(jogadas);

    while(!acabou(*m)) {
        cor = guloso_fronteira_externa_com_otima(m);
        pinta_mapa(m, cor);
        printf("%d\n", cor);
        ++Njogadas;

        if(Njogadas > 10000) {
            printf("Acho que loop infinito. Quitando...\n");
            exit(1);
        }
    }

    destroi_tmapa(m, 0);

    return Njogadas;
}

int main(int argc, char **argv) {
    int cor, semente;
    tmapa m;

    if(argc < 4 || argc > 5) {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }

    m.nlinhas = atoi(argv[1]);
    m.ncolunas = atoi(argv[2]);
    m.ncores = atoi(argv[3]);
    m.tam = m.nlinhas * m.ncolunas;

    Njogadas = 0;
    Linhas = m.nlinhas;
    Colunas = m.ncolunas;
    TamMatriz = Linhas * Colunas;

    if(argc == 5)
        semente = atoi(argv[4]);
    else
        semente = -1;

    gera_mapa(&m, semente);

    resolve(&m);

    printf("0\n");

    printf("%d jogadas.\n", Njogadas);

    return 0;
}
