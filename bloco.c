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
#include "bloco.h"

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

/* Blocos */
void define_n_blocos(tmapa *m, int stepx, int stepy) {
    int i0, j0, i, j, px, py, mini, minj;

    px = py = 0;

    for(i0 = 0; i0 < m->nlinhas; i0 += stepx) {
        for(j0 = 0; j0 < m->ncolunas; j0 += stepy) {
            mini = ((i0 + stepx) > m->nlinhas) ? m->nlinhas : i0+stepx;
            for(i=i0; i<mini; ++i) {
                minj = ((j0 + stepy) > m->ncolunas) ? m->ncolunas: j0+stepy;
                for(j=j0; j<minj; ++j) {
                    m->mapa[ID(i,j)]->bloco = px + py;
                }
            }
            ++py;
            ++Nblocos;
        }
        px += py;
        py = 0;
    }
}

void conta_blocos(tmapa *m) {
    int i;
    for(i=0; i<Nblocos; ++i) {
        Bloco[i].restante = 0;
    }
    for(i=0; i<m->tam; ++i) {
        if(m->mapa[i]->status != STATUS_MAIN) {
            Bloco[m->mapa[i]->bloco].restante++;
        }
    }
}

void define_pesos_blocos() {
    int i, m, peso = MaiorPeso;
    int restantes[Nblocos];

    for(i=0; i<Nblocos; ++i) {
        restantes[i] = Bloco[i].restante;
    }

    for(i=0; i<Nblocos; ++i) {
        m = maior_restante(restantes, Nblocos);
        Bloco[m].peso = peso;
        restantes[m] = -1;
        peso -= DecrPeso;
    }
}

int bloco_calcula_cor(tmapa *m) {
    int i, best = 0;
    avaliador cores[m->ncores];
    for(i=0; i<m->ncores; ++i) {
        cores[i].cor = i;
        cores[i].n_ext = 0;
        cores[i].n_int = 0;
    }

    //prepara_fronteiras(m);

    for(i=0; i<m->tam; ++i) {
        //printf("Status = %d, Bloco = %d, Peso = %d\n", m->mapa[i]->status, m->mapa[i]->bloco, Bloco[m->mapa[i]->bloco].peso);
        if(m->mapa[i]->status == STATUS_F_EXT) {
            cores[m->mapa[i]->cor - FIRST_COLOR].n_ext += Bloco[m->mapa[i]->bloco].peso;
        } else if(m->mapa[i]->status == STATUS_F_INT) {
            cores[m->mapa[i]->cor - FIRST_COLOR].n_int += Bloco[m->mapa[i]->bloco].peso;
        }
    }

    for(i=0; i<m->ncores; ++i) {
        if(cores[best].n_ext < cores[i].n_ext || (cores[best].n_ext == cores[i].n_ext && cores[best].n_int < cores[i].n_int)) {
            best = i;
        }
    }

    return best + FIRST_COLOR;
}
