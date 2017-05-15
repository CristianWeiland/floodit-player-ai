#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ia.h"
#include "utils.h"
#include "mapa.h"
#include "grafo.h"

// TODO: Destruir os grafos nas iteracoes, ou parar de recria-lo e soh atualiza-lo.

int jogada_random(tmapa m) {
    return rand() % m.ncores + 1;
}

int guloso(tmapa m, grafo g) {
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

    //printf("Vou tirar %d elementos, da cor %d.\n", best->elems, best->cor);
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
    static int i = -1;
    int r = jogada_otima(&m, g);
    if(r != -1) {
        return r;
    }

    // Metodo 3: Começa indo até o bloco mais sudoeste, depois faz um guloso.
/*
    if(i == -1) {
        static int x1, x2, y1, y2, *jogadas;
        bloco_baixo_direita(&x1, &y1, &x2, &y2);
        i = menor_caminho(m, g, mais_proximo(m, x1, y1, x2, y2), jogadas);
    } else if(i == v->d-1) {
        i = -2;
    } else if(i >= 0) {
        ++i;
        return jogadas[i-1];
    }
*/
/*
    if(i == -1) {
        static int x1, x2, y1, y2, *jogadas;
        bloco_baixo_direita(&x1, &y1, &x2, &y2);
        i = menor_caminho(m, g, mais_proximo(m, x1, y1, x2, y2), jogadas);
        while(i < (v->d-2) ) {
            pinta_mapa(&m, jogadas[i++]);
            ++Njogadas;
        }
        i = -2;
        return jogadas[v->d-1];
    }
*/

    // Metodo 2: Guloso
    return guloso(m, g);

    // Metodo 1: Random
    return jogada_random(m);
}

void bloco_baixo_direita(int *x1, int *y1, int *x2, int *y2) {
    *x1 = (int) (Linhas * 3) / 4;
    *y1 = (int) (Colunas * 3) / 4;
    *x2 = Linhas - 1;
    *y2 = Colunas - 1;
}

int menor_caminho(tmapa *m, grafo g, vertice v, int *jogadas) {
    // Cria um vetor de v->d jogadas, que fazem vc sair de g->lider e chegar em v.
    // Devolve a posição da primeira jogada (deveria ser 0). O vetor acab em v->d;

    int i;
    vertice w;
    aresta a;

    jogadas = (int *) malloc(v->d * sizeof(int));

    for(i = v->d-1, w = v; w != g->lider; --i) {
        // Adiciona a cor do vertice que eu quero pintar.
        jogadas[i] = w->cor;

        // Faz w apontar pro vertice anterior.
        a = (aresta) conteudo(primeiro_no(v->entrada));
        w = a->vs;
    }

    if(i != 0) {
        printf("Vetor de jogadas não começa no 0, começa em %d\n", i);
    }
    return i;
}

int main(int argc, char **argv) {
    int cor;
    tmapa m;
    int semente;
    grafo g;

    if(argc < 4 || argc > 5) {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }

    m.nlinhas = atoi(argv[1]);
    m.ncolunas = atoi(argv[2]);
    m.ncores = atoi(argv[3]);
    m.tam = m.nlinhas * m.ncolunas;

    VerticeID = 0;
    Njogadas = 0;
    Linhas = m.nlinhas;
    Colunas = m.ncolunas;
    TamMatriz = Linhas * Colunas;

    if(argc == 5)
        semente = atoi(argv[4]);
    else
        semente = -1;

    gera_mapa(&m, semente);

    g = cria_grafos(&m);

    mostra_mapa_cor(&m, 0);
    escreve_grafo(stdout, g);

    while(!acabou(m)) {
        //mostra_mapa_cor(&m, 0); // para mostrar sem cores use mostra_mapa(&m);
        cor = proxima_jogada(m, g);
        //printf("A cor selecionada eh %d\n", cor);
        pinta_mapa(&m, cor);
        // Precisa destruir o grafo g!
        destroi_grafo(g);
        g = cria_grafos(&m);
        ++Njogadas;
    }

    //mostra_mapa_cor(&m, 1); // para mostrar sem cores use mostra_mapa(&m);

    printf("Parabens! Voce venceu em %d jogadas!\n", Njogadas);

    destroi_grafo(g);
    destroi_tmapa(m, 0);

    return 0;
}

// Funções não prontas
grafo atualiza_grafo(grafo g, int cor) {
    no elem, child;
    vertice w;
    aresta a;
    int i;
    for(elem = primeiro_no(g->lider->saida); elem; elem = proximo_no(elem)) {
        w = (vertice) conteudo(elem);
        // Só quero vizinhos da mesma cor, foram os que eu pintei.
        if(w->cor != cor)
            continue;
        // Adiciona *i e *j de w em lider.
        for(i=0; i<w->elems; ++i) {
            g->lider->i[g->lider->elems + i] = w->i[i];
            g->lider->j[g->lider->elems + i] = w->j[i];
        }
        g->lider->elems += w->elems;
        /* Acho que aqui eu arrumei os vertices. Provavelmente se eu chamar o cria_arestas() funciona. Pensar nisso!
        for(child = primeiro_no(w->saida); child; child = proximo_no(child)) {
            a = (aresta) conteudo(child);
            a->vs = g->lider;
        }
        */
        destroi_lista(w->saida, NULL);
        destroi_lista(w->entrada, destroi_aresta);
        destroi_vertice(w);
    }

    return g;
}
