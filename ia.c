#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ia.h"
#include "utils.h"
#include "mapa.h"
#include "grafo.h"

#define MAIOR_PESO 10 // Provavelmente eu quero fazer alguma conta a partir de Nblocos
#define DECR_PESO 1

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

int guloso_fronteira_externa(tmapa *m) {
    // Guloso olhando soh matriz.
    int i, best;
    avaliador cores[m->ncores];

    best = 0;
    for(i=0; i<m->ncores; ++i) {
        cores[i].cor = i;
        cores[i].n_int = 0;
        cores[i].n_ext = 0;
    }
    zera_status(m);

    zera_counted(m);
    flood_set_status(m, 0, 0, m->mapa[0]->cor, STATUS_MAIN);

    zera_counted2(m);
    define_fronteira_vizinhos(m, 0, 0);

    zera_counted(m);
    define_front_int_ext(m);

    // Agora que eu tenho as fronteiras devidamente definidas, tenho que contar quantos de cada cor posso eliminar.
    for(i=0; i<m->tam; ++i) {
        if(m->mapa[i]->status == STATUS_F_EXT) {
            cores[m->mapa[i]->cor - FIRST_COLOR].n_ext++;
        } else if(m->mapa[i]->status == STATUS_F_INT) {
            cores[m->mapa[i]->cor - FIRST_COLOR].n_int++;
        }
    }

    for(i=0; i<m->ncores; ++i) {
        if(cores[best].n_ext < cores[i].n_ext || (cores[best].n_ext == cores[i].n_ext && cores[best].n_int < cores[i].n_int)) {
            best = i;
        }
    }

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
    // Eu to chamando zera_counted MUITAS vezes. Pra reduzir, eu posso fazer um esquema assim: começa os counteds com 0.
    // Em vez de fazer m->counted = 1 e m->counted == 1, faz assim: cria um contador de iterações, count.
    // Eu seto m->counted = count, e comparo com m->counted == count.
    int i, j, ret;

    Count = 0;

    for(i=0; i<m->nlinhas; ++i) {
        for(j=0; j<m->ncolunas; ++j, ++Count) {
            if(m->mapa[ID(i,j)]->status == STATUS_F) {
                zera_counted(m);
                ret = tipo_fronteira(m, i, j, m->mapa[ID(i,j)]->cor);
                //printf("Eh %d\n", ret);
                if(ret) {
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

    if(m->mapa[ID(i,j)]->counted == Count) // Jah foi contado, retorna o valor 'neutro' (não atrapalha ninguém)
        return 1;

    //printf("Pos = %d-%d, cor = %d, cor2 = %d\n", i, j, cor, m->mapa[ID(i,j)]->cor);

    m->mapa[ID(i,j)]->counted = Count;

    ret = checa_condicoes(m, i+1, j, cor);
    /*
    if(!borda(i+1,j))
        printf("Direita (cor %d e status %d) deu %d\n", m->mapa[ID(i,j)]->cor, m->mapa[ID(i,j)]->status, ret);
    */
    if(ret == 2)
        ret = tipo_fronteira(m, i+1, j, cor);
    if(ret == 0)
        return 0;

    ret = checa_condicoes(m, i-1, j, cor);
    /*
    if(!borda(i-1,j))
        printf("Esquerda (cor %d e status %d) deu %d\n", m->mapa[ID(i,j)]->cor, m->mapa[ID(i,j)]->status, ret);
    */
    if(ret == 2)
        ret = tipo_fronteira(m, i-1, j, cor);
    if(ret == 0)
        return 0;

    ret = checa_condicoes(m, i, j+1, cor);
    /*
    if(!borda(i,j+1))
        printf("Cima (cor %d e status %d) deu %d\n", m->mapa[ID(i,j)]->cor, m->mapa[ID(i,j)]->status, ret);
    */
    if(ret == 2)
        ret = tipo_fronteira(m, i, j+1, cor);
    if(ret == 0)
        return 0;

    ret = checa_condicoes(m, i, j-1, cor);
    /*
    if(!borda(i,j-1))
        printf("Baixo (cor %d e status %d) deu %d\n", m->mapa[ID(i,j)]->cor, m->mapa[ID(i,j)]->status, ret);
    */
    if(ret == 2)
        ret = tipo_fronteira(m, i, j-1, cor);
    if(ret == 0)
        return 0;

    return 1;
}

int checa_condicoes(tmapa *m, int i, int j, int cor) {
    if(borda(i,j)) // Condicao 1
        return 1;
    if(m->mapa[ID(i,j)]->status == STATUS_MAIN) // Condicao 2
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
    int i, m, peso = MAIOR_PESO;
    int restantes[Nblocos];

    for(i=0; i<Nblocos; ++i) {
        restantes[i] = Bloco[i].restante;
    }

    for(i=0; i<Nblocos; ++i) {
        m = maior_restante(restantes, Nblocos);
        Bloco[m].peso = peso;
        restantes[m] = -1;
        peso -= DECR_PESO;
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
    zera_status(m);

    zera_counted(m);
    flood_set_status(m, 0, 0, m->mapa[0]->cor, STATUS_MAIN);

    zera_counted2(m);
    define_fronteira_vizinhos(m, 0, 0);

    zera_counted(m);
    define_front_int_ext(m);

    for(i=0; i<m->tam; ++i) {
        if(m->mapa[i]->status == STATUS_F_EXT) {
            cores[m->mapa[i]->cor - FIRST_COLOR].n_ext += Bloco[m->mapa[i]->bloco].peso;
        } else if(m->mapa[i]->status == STATUS_F_INT) {
            cores[m->mapa[i]->cor - FIRST_COLOR].n_int += Bloco[m->mapa[i]->peso].peso;
        }
    }

    for(i=0; i<m->ncores; ++i) {
        if(cores[best].n_ext < cores[i].n_ext || (cores[best].n_ext == cores[i].n_ext && cores[best].n_int < cores[i].n_int)) {
            best = i;
        }
    }

    return best + FIRST_COLOR;
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
        //Restantes = (int*) malloc(Nblocos * sizeof(int));
        //if(!Restantes) printf("Erro mallocando Restantes.\n");
        //Pesos = (int*) malloc(Nblocos * sizeof(int));
        //if(!Pesos) printf("Erro mallocando Pesos.\n");
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

        if(Njogadas > 10) {
            printf("Acho que loop infinito. Quitando...\n");
            exit(1);
        }
    }

    //mostra_mapa_cor(&m, 1); // para mostrar sem cores use mostra_mapa(&m);

    //printf("Parabens! Voce venceu em %d jogadas!\n", Njogadas);
    printf("%d\n", Njogadas);

    destroi_grafo(g);
    destroi_tmapa(m, 0);

    return 0;
}
