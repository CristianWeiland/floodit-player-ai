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

void prepara_fronteiras(tmapa *m) {
    zera_status(m);

    zera_counted(m);
    flood_set_status(m, 0, 0, m->mapa[0]->cor, STATUS_MAIN);

    zera_counted2(m);
    define_fronteira_vizinhos(m, 0, 0);

    zera_counted(m);
    define_front_int_ext(m);
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

    m->mapa[ID(i,j)]->counted = Count;

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
    if(m->mapa[ID(i,j)]->status == STATUS_MAIN) // Condicao 2
        return 1;
    if(m->mapa[ID(i,j)]->cor == cor) // Condicao 3
        return 2;
    return 0;
}

int guloso_fronteira_externa_com_otima(tmapa *m) {
    // Guloso olhando soh matriz.
    int i, best, *faltam;
    avaliador cores[m->ncores];

    best = 0;
    faltam = (int *) malloc(m->ncores * sizeof(int));

    for(i=0; i<m->ncores; ++i) {
        faltam[i] = 0;
        cores[i].cor = i;
        cores[i].n_int = 0;
        cores[i].n_ext = 0;
    }

    prepara_fronteiras(m);

    // Agora que eu tenho as fronteiras devidamente definidas, tenho que contar quantos de cada cor posso eliminar.
    for(i=0; i<m->tam; ++i) {
        if(m->mapa[i]->status == STATUS_F_EXT) {
            cores[m->mapa[i]->cor - FIRST_COLOR].n_ext++;
        } else if(m->mapa[i]->status == STATUS_F_INT) {
            cores[m->mapa[i]->cor - FIRST_COLOR].n_int++;
        }
    }

    // Calcula quantos faltam:
    for(i=0; i<m->tam; ++i) {
        faltam[m->mapa[i]->cor - FIRST_COLOR]++;
    }

    // Vê se tem alguma cor que é 100% eliminada:
    for(i=0; i<m->ncores; ++i) {
        if(faltam[i] != 0 && faltam[i] == cores[i].n_int + cores[i].n_ext) {
            //mostra_mapa_cor(m, 0);
            return i + FIRST_COLOR;
        }
    }

    for(i=0; i<m->ncores; ++i) {
        if(cores[best].n_ext < cores[i].n_ext || (cores[best].n_ext == cores[i].n_ext && cores[best].n_int < cores[i].n_int)) {
            best = i;
        }
    }

    return best + FIRST_COLOR;
}

