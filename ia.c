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

#define STEP_D 2

#define RANDOM 0
#define GULOSO 1
#define GULOSO_INT_EXT 2
#define GULOSO_INT_EXT_MOVE 3
#define PRIORITY_BLOCK 4

int Algoritmo = GULOSO_INT_EXT_MOVE;

int jogada_random(tmapa m) {
    return rand() % m.ncores + 1;
}

int guloso(tmapa m, grafo g) {
    // Guloso olhando o grafo.
    int i;
    no elem;
    aresta a;
    vertice best;

    if(!primeiro_no(g->v)) {
        puts("Erro calculando a proxima jogada.");
        return -1;
    }

    best = NULL;

    // Achei o vertice inicial. Agora tenho que ver qual eh o vizinho mais adequado.
    for(elem = primeiro_no(g->lider->saida); elem; elem = proximo_no(elem)) {
        a = (aresta) conteudo(elem);
        if(!best) {
            best = a->vc;
        }
        if(a->vs->elems > best->elems) {
            best = a->vc;
        }
    }

    return best->cor;
}

int* quantos_faltam(tmapa *m, grafo g, int *ret) {
    int i;

    for(i=0; i<m->tam; ++i) {
        ++(ret[m->mapa[i]->cor - FIRST_COLOR]);
    }

    // Nada na vida eh tao simples. Eu nao posso considerar os elementos do meu vertice (g->lider).
    ret[g->lider->cor - FIRST_COLOR] -= g->lider->elems;

    return ret;
}

int jogada_otima(tmapa *m, grafo g) {
// Eu sei que a jogada eh otima se eu elimino totalmente alguma cor. Se a jogada nao existir, retorna -1.
// A unica forma de eu eliminar uma cor eh, dada uma cor C, se o somatorio de v->elems para todos os v tal que v eh meu vizinho e v->cor == C for igual ao numero de elementos C na matriz.
// Em outras palavras:
// 1- Conto quantos elementos faltam de cada cor, e salvo em 'faltam'.
// 2- Conto quantos elementos de cada cor eu elimino se eu selecionar essa cor.
// 3- Comparo se o numero de elementos que eu elimino eh igual ao numero que faltam. Se sim, eh jogada otima. Se nao, deixa quieto.
    int i, *faltam, *posso_eliminar;
    no elem;
    aresta a;

    faltam = (int *) malloc(m->ncores * sizeof(int));
    posso_eliminar = (int *) malloc(m->ncores * sizeof(int));
    if(!faltam || !posso_eliminar) {
        puts("(jogada_otima) Erro de malloc.");
        exit(1);
    }

    for(i=0; i<m->ncores; ++i) {
        faltam[i] = 0;
        posso_eliminar[i] = 0;
    }

    faltam = quantos_faltam(m, g, faltam);

    // Ve se na vizinhanca
    for(elem = primeiro_no(g->lider->saida); elem; elem = proximo_no(elem)) {
        a = (aresta) conteudo(elem);
        posso_eliminar[a->vc->cor - FIRST_COLOR] += a->vc->elems;
    }

    for(i=0; i<m->ncores; ++i) {
        //printf("Pra cor %d faltam %d, e posso eliminar %d...\n", i+FIRST_COLOR, faltam[i], posso_eliminar[i]);
        if(faltam[i] == posso_eliminar[i] && faltam[i] != 0) {
            //printf("Eliminando...\n");
            free(faltam);
            free(posso_eliminar);
            return i+FIRST_COLOR;
        }
    }

    free(faltam);
    free(posso_eliminar);
    return -1;
}

int proxima_jogada(tmapa m, grafo g) {
    int r = jogada_otima(&m, g);
    /*if(r != -1) {
        return r;
    }*/

    if(Algoritmo == PRIORITY_BLOCK) {
        conta_blocos(&m);
        define_pesos_blocos();
        r = bloco_calcula_cor(&m);
        /*mostra_mapa_cor(&m, 0);
        printf("R = %d\n", r);*/
        return r;
    }

    // Algoritmo 2: Guloso fronteira int/ext deopis de andar até algum ponto.
    if(Algoritmo == GULOSO_INT_EXT_MOVE) {
        static int executado = 0;
        if(!executado) {
            int i, first, last, *jogadas, x1, x2, y1, y2;
            bloco_baixo_direita(&x1, &y1, &x2, &y2);
            vertice v = vertice_menor_distancia(&m, x1, y1, x2, y2);
            first = menor_caminho(&m, g, v, &jogadas);
            last = first + v->d;
            for(i=first; i<last; ++i) {
                //printf("First = %d, last = %d, jogadas[%d] = %d\n", first, last, i, jogadas[i]);
                //mostra_mapa_cor(&m, 1);
                pinta_mapa(&m, jogadas[i]);
                ++Njogadas;
                //getchar();
                //mostra_mapa_cor(&m, 0);
            }
            executado = 1;

        }
        return guloso_fronteira_externa(&m);
    }

    // Algoritmo 1: Guloso fronteira interna/externa
    if(Algoritmo == GULOSO_INT_EXT) {
        return guloso_fronteira_externa(&m);
    }

    // Metodo 2: Guloso
    if(Algoritmo == GULOSO) {
        return guloso(m, g);
    }

    // Metodo 1: Random
    if(Algoritmo == RANDOM) {
        return jogada_random(m);
    }
}

int main(int argc, char **argv) {
    int cor;
    tmapa m;
    int semente;
    grafo g;

    if(argc < 5 || argc > 6) {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> <algoritmo> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }

    m.nlinhas = atoi(argv[1]);
    m.ncolunas = atoi(argv[2]);
    m.ncores = atoi(argv[3]);
    m.tam = m.nlinhas * m.ncolunas;

    Algoritmo = atoi(argv[4]);

    VerticeID = 0;
    Njogadas = 0;
    Linhas = m.nlinhas;
    Colunas = m.ncolunas;
    TamMatriz = Linhas * Colunas;

    if(argc == 6)
        semente = atoi(argv[5]);
    else
        semente = -1;

    gera_mapa(&m, semente);

    g = cria_grafos(&m);

    if(Algoritmo == PRIORITY_BLOCK) {
        Nblocos = 0;
        define_n_blocos(&m, ceil( (double) m.nlinhas / STEP_D), ceil( (double) m.ncolunas / STEP_D)); // Funciona!
        Bloco = (bloco*) malloc(Nblocos * sizeof(bloco));
        if(!Bloco) {
            printf("Erro mallocando Bloco.\n");
            exit(1);
        }
    }

    //mostra_mapa_blocos(&m);

    //mostra_mapa_cor(&m, 0);
    //escreve_grafo(stdout, g);

    while(!acabou(m)) {
        cor = proxima_jogada(m, g);
        pinta_mapa(&m, cor);
        // Precisa destruir o grafo g!
        destroi_grafo(g);
        g = cria_grafos(&m); // Nao posso comentar isso enquanto eu tiver usando o jogada_otima!
        ++Njogadas;

        //mostra_mapa_cor(&m, 0);

        if(Njogadas > 100) {
            printf("Acho que loop infinito. Quitando...\n");
            exit(1);
        }
    }

    //mostra_mapa_cor(&m, 1); // para mostrar sem cores use mostra_mapa(&m);

    printf("%d\n", Njogadas);

    if(Algoritmo == PRIORITY_BLOCK) {
        free(Bloco);
    }
    destroi_grafo(g);
    destroi_tmapa(m, 0);

    return 0;
}
