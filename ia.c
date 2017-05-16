#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ia.h"
#include "utils.h"
#include "mapa.h"
#include "grafo.h"

int jogada_random(tmapa m) {
    return rand() % m.ncores + 1;
}

int guloso_fronteira_externa(tmapa *m) {
    // Guloso olhando soh matriz.
    int i, cores[m->ncores], best, second, id;

    best = 0;
    second = best;
    for(i=0; i<m->ncores; ++i) {
        cores[i] = 0;
    }
    zera_status(m);

    zera_counted(m);
    flood_set_status(m, 0, 0, m->mapa[0]->cor, STATUS_MAIN);

    zera_counted2(m);
    define_fronteira_vizinhos(m, 0, 0);

    zera_counted(m);
    // TA ERRADO! Ta pegando mais do que deveria... Olhar execucao com parametros 5 5 5 10.
    /*
   0 1 2 3 4
   0: 4 4 4 1 4
   1: 3 1 2 3 3
   2: 4 4 4 4 5
   3: 2 1 4 2 4
   4: 4 5 1 2 3

    Vira...:

   0 1 2 3 4
   0: 1 1 1 4 0
   1: 4 3 3 0 0
   2: 0 0 0 0 0
   3: 0 0 0 0 0
   4: 0 0 0 0 0
    */
    define_front_int_ext(m);

    // Agora que eu tenho as fronteiras devidamente definidas, tenho que contar quantos de cada cor posso eliminar.
    for(i=0; i<m->tam; ++i) {
        if(m->mapa[i]->status == STATUS_F_EXT) {
            id = m->mapa[i]->cor - FIRST_COLOR;
            cores[id]++;
            if(cores[best] < cores[id]) {
                second = best;
                best = id;
            }
        }
    }
    /*
    if(cores[second] == cores[best]) {
        // Se eu cheguei aqui eh porque empatou. Preciso de um criterio de desempate. Posso usar fronteira interna.
    }
    */
    return best + FIRST_COLOR;
}

void define_fronteira_vizinhos(tmapa *m, int i, int j) {
    // Vou setar com STATUS_F todo mundo da fronteira, seja interna ou externa. A ideia eh simples:
    // pra todos os vizinhos das celulas com status = STATUS_MAIN, faco um flood_set_status(STATUS_F)
    if(borda(i,j))
        return;
    if(m->mapa[ID(i,j)]->counted2)
        return;
    m->mapa[ID(i,j)]->counted2 = 1;
    if(m->mapa[ID(i,j)]->status != STATUS_MAIN) {
        // Eh outra cor. Eh fronteira. Nao sei se interna ou externa, por isso seto como STATUS_F.
        zera_counted(m);
        flood_set_status(m, i, j, m->mapa[ID(i,j)]->cor, STATUS_F);
        return;
    }
    // Se cheguei ate aqui, essa celula nao eh borda, eh da cor Main e nao foi contado. Chama pros vizinhos.
    define_fronteira_vizinhos(m, i+1, j);
    define_fronteira_vizinhos(m, i-1, j);
    define_fronteira_vizinhos(m, i, j+1);
    define_fronteira_vizinhos(m, i, j-1);
}

void define_front_int_ext(tmapa *m) {
    // Percorre todas as celulas. Se for fronteira e nao for definido se eh interna ou externa, verifica.
    int i, j;

    for(i=0; i<m->nlinhas; ++i) {
        for(j=0; j<m->ncolunas; ++j) {
            if(m->mapa[ID(i,j)]->status == STATUS_F) {
                if(tipo_fronteira(m, i, j, m->mapa[ID(i,j)]->cor)) {
                    zera_counted(m);
                    flood_set_status(m, i, j, m->mapa[ID(i,j)]->cor, STATUS_F_INT);
                } else {
                    zera_counted(m);
                    flood_set_status(m, i, j, m->mapa[ID(i,j)]->cor, STATUS_F_EXT);
                }
            }
        }
    }
}

int tipo_fronteira(tmapa *m, int i, int j, int cor) {
    // Retorna 1 se achar que eh fronteira interna, 0 caso contrario.
    // Atualmente, para um quadrado qualquer, eu posso dizer que ele eh fronteira interna se TODAS as celulas ao seu redor forem:
    // 1- Borda;
    // 2- Cor == main;
    // 3- Cor == mesma que eu;
    // Se qualquer uma dessas condicoes nao for atendida PRA QUALQUER CELULA, retorna 0. Em outras palavras, soh eh fronteira
    // interna se as 3 condicoes forem atendidas pra TODAS as celulas.
    int ret;

    if(m->mapa[ID(i,j)]->counted) // Jah foi contado, retorna o valor 'neutro' (não atrapalha ninguém)
        return 1;

    m->mapa[ID(i,j)]->counted = 1;

    ret = checa_condicoes(m, i+1, j, cor);
    if(ret == 2)
        ret = tipo_fronteira(m, i+1, j, cor);
    if(ret == 0)
        return 0;

    ret = checa_condicoes(m, i-1, j, cor);
    if(ret == 2)
        ret = tipo_fronteira(m, i-1, j, cor);
    if(ret == 0)
        return 0;

    ret = checa_condicoes(m, i, j+1, cor);
    if(ret == 2)
        ret = tipo_fronteira(m, i, j+1, cor);
    if(ret == 0)
        return 0;

    ret = checa_condicoes(m, i, j-1, cor);
    if(ret == 2)
        ret = tipo_fronteira(m, i, j-1, cor);
    if(ret == 0)
        return 0;

    return 1;
}

int checa_condicoes(tmapa *m, int i, int j, int cor) {
    if(borda(i,j)) // Condicao 1
        return 1;
    if(m->mapa[ID(i,j)]->cor == m->mapa[0]->cor) // Condicao 2
        return 1;
    if(m->mapa[ID(i,j)]->cor == cor) // Condicao 3
        return 2;
    return 0;
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

    // Algoritmo 1: Guloso fronteira interna/externa
    r = guloso_fronteira_externa(&m);
    printf("Retornando %d\n", r);
    return r;

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
    //escreve_grafo(stdout, g);

    while(!acabou(m)) {
        cor = proxima_jogada(m, g);
        //printf("A cor selecionada eh %d\n", cor);
        pinta_mapa(&m, cor);
        // Precisa destruir o grafo g!
        destroi_grafo(g);
        g = cria_grafos(&m); // Nao posso comentar isso enquanto eu tiver usando o jogada_otima!
        ++Njogadas;

        mostra_mapa_cor(&m, 0);
        mostra_mapa_status(&m);

        if(Njogadas > 20) {
            printf("Acho que loop infinito. Quitando...\n");
            exit(1);
        }
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
