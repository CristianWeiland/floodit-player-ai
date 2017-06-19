#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "mapa.h"
#include "utils.h"

inline int ID(int i, int j) {
    return i * Colunas + j;
}

void gera_mapa(tmapa *m, int semente) {
    int i, j;

    if(semente < 0)
        srand(time(NULL));
    else
        srand(semente);

    m->mapa = (celula *) malloc(m->nlinhas * m->ncolunas * sizeof(celula));
    if(!m->mapa) {
        puts("(gera_mapa) Erro de malloc em mapa.");
        exit(1);
    }

    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++) {
            m->mapa[ID(i,j)] = (celula) malloc(sizeof(struct celula));
            if(!m->mapa[ID(i,j)]) {
                puts("(gera_mapa) Erro de malloc em mapa[i].");
                exit(1);
            }
            scanf("%d", &(m->mapa[ID(i,j)]->cor));
            m->mapa[ID(i,j)]->counted = 0;
            m->mapa[ID(i,j)]->v = NULL;
        }
    }
}

tmapa* copia_tmapa(tmapa *m) {
    int i;
    tmapa *n = (tmapa *) malloc(sizeof(struct tmapa));
    memcpy(n, m, sizeof(struct tmapa));
    n->mapa = (celula *) malloc(n->tam * sizeof(celula));
    for(i=0; i<n->tam; ++i) {
        n->mapa[i] = (celula) malloc(sizeof(struct celula));
        memcpy(n->mapa[i], m->mapa[i], sizeof(struct celula));
    }
    return n;
}

void destroi_tmapa(tmapa *m, int destruir_vertices) {
    int i;
    for(i=0; i<m->tam; ++i) {
        if(destruir_vertices && m->mapa[i]->v) {
            free(m->mapa[i]->v);
        }
        free(m->mapa[i]);
    }
    free(m->mapa);
}

void mostra_mapa(tmapa *m) {
    int i, j;

    clear();

    printf("Linhas: %d Colunas: %d Cores: %d\n\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
	printf("%02d ", m->mapa[ID(i,j)]->cor);
            else
	printf("%d ", m->mapa[ID(i,j)]->cor);
        printf("\n");
    }
}

void mostra_mapa_cor(tmapa *m, int shouldClear) {
    int i, j;
    char* cor_ansi[] = { "\x1b[0m",
		             "\x1b[31m", "\x1b[32m", "\x1b[33m",
		             "\x1b[34m", "\x1b[35m", "\x1b[36m",
		             "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
		             "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
		             "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m" };

    if(shouldClear)
        clear();

    if(m->ncores > 15) {
        mostra_mapa(m);
        return;
    }
    printf("Linhas: %d Colunas: %d Cores: %d\n   ", m->nlinhas, m->ncolunas, m->ncores);
    for(i=0; i<m->ncolunas; ++i) {
        printf("%d ", i);
    }
    printf("\n");
    for(i = 0; i < m->nlinhas; i++) {
        if(m->nlinhas && i < 10) {
            printf(" %d: ", i);
        } else {
            printf("%d: ", i);
        }
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
	printf("%s%02d%s ", cor_ansi[m->mapa[ID(i,j)]->cor], m->mapa[ID(i,j)]->cor, cor_ansi[0]);
            else
	printf("%s%d%s ", cor_ansi[m->mapa[ID(i,j)]->cor], m->mapa[ID(i,j)]->cor, cor_ansi[0]);
        printf("\n");
    }
    puts("");
}

void mostra_mapa_status(tmapa *m) {
    int i, j;
    char* cor_ansi[] = { "\x1b[0m",
		             "\x1b[31m", "\x1b[32m", "\x1b[33m",
		             "\x1b[34m", "\x1b[35m", "\x1b[36m",
		             "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
		             "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
		             "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m" };
    for(i=0; i<m->ncolunas; ++i) {
        printf("%d ", i);
    }
    printf("\n");
    for(i = 0; i < m->nlinhas; i++) {
        printf("%d: ", i);
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
	printf("%s%02d%s ", cor_ansi[m->mapa[ID(i,j)]->status], m->mapa[ID(i,j)]->status, cor_ansi[0]);
            else
	printf("%s%d%s ", cor_ansi[m->mapa[ID(i,j)]->status], m->mapa[ID(i,j)]->status, cor_ansi[0]);
        printf("\n");
    }
    puts("");
}

void mostra_mapa_param(tmapa *m, int print) {
    int i, j;

    printf("Linhas: %d Colunas: %d Cores: %d\n\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
            if(print == PRINT_COR) {
            	printf("%d ", m->mapa[ID(i,j)]->cor);
            } else if(print == PRINT_STATUS) {
            	printf("%d ", m->mapa[ID(i,j)]->status);
            }
        printf("\n");
    }
}

void pinta(tmapa *m, int l, int c, int fundo, int cor) {
    m->mapa[ID(l,c)]->cor = cor;
    if(l < m->nlinhas - 1 && m->mapa[ID(l+1,c)]->cor == fundo)
        pinta(m, l+1, c, fundo, cor);
    if(c < m->ncolunas - 1 && m->mapa[ID(l,c+1)]->cor == fundo)
        pinta(m, l, c+1, fundo, cor);
    if(l > 0 && m->mapa[ID(l-1,c)]->cor == fundo)
        pinta(m, l-1, c, fundo, cor);
    if(c > 0 && m->mapa[ID(l,c-1)]->cor == fundo)
        pinta(m, l, c-1, fundo, cor);
}

void pinta_mapa(tmapa *m, int cor) {
    if(cor == m->mapa[ID(0,0)]->cor)
        return;
    pinta(m, 0, 0, m->mapa[ID(0,0)]->cor, cor);
}

void pinta_fake_rec(tmapa *m, int l, int c, int fundo, int cor) {
    m->mapa[ID(l,c)]->pintado = 1;
    if(l < m->nlinhas - 1 && m->mapa[ID(l+1,c)]->cor == fundo && !m->mapa[ID(l+1,c)]->pintado)
        pinta_fake_rec(m, l+1, c, fundo, cor);
    if(c < m->ncolunas - 1 && m->mapa[ID(l,c+1)]->cor == fundo && !m->mapa[ID(l,c+1)]->pintado)
        pinta_fake_rec(m, l, c+1, fundo, cor);
    if(l > 0 && m->mapa[ID(l-1,c)]->cor == fundo && !m->mapa[ID(l-1,c)]->pintado)
        pinta_fake_rec(m, l-1, c, fundo, cor);
    if(c > 0 && m->mapa[ID(l,c-1)]->cor == fundo && !m->mapa[ID(l,c-1)]->pintado)
        pinta_fake_rec(m, l, c-1, fundo, cor);
}

void pinta_mapa_fake(tmapa *m, int cor) {
    if(cor == m->mapa[ID(0,0)]->cor)
        return;
    pinta_fake_rec(m, 0, 0, m->mapa[ID(0,0)]->cor, cor);
}

int acabou(tmapa m) {
    int i, cor = m.mapa[0]->cor;
    for(i=1; i<m.tam; ++i) {
        if(m.mapa[i]->cor != cor) {
            return 0;
        }
    }
    return 1;
}

inline int borda(int i, int j) {
    if(i < 0 || j < 0 || i >= Linhas || j >= Colunas) {
        return 1;
    }
    return 0;
}

inline void zera_status(tmapa *m) {
    int i;
    for(i=0; i<m->tam; ++i) {
        m->mapa[i]->status = 0;
    }
}

inline void zera_counted(tmapa *m) {
    int i;
    for(i=0; i<m->tam; ++i) {
        m->mapa[i]->counted = 0;
    }
}

inline void zera_counted2(tmapa *m) {
    int i;
    for(i=0; i<m->tam; ++i) {
        m->mapa[i]->counted2 = 0;
    }
}

inline void zera_pintados(tmapa *m) {
    int i;
    for(i=0; i<m->tam; ++i) {
        m->mapa[i]->pintado = 0;
    }
}

void flood_set_status(tmapa *m, int i, int j, int minha_cor, int status) {
    // Antes de chamar isso, eu quero setar counted como 0 (usar zera_counted)!
    if(borda(i,j)) {
        return;
    }
    if(m->mapa[ID(i,j)]->counted)
        return;
    m->mapa[ID(i,j)]->counted = 1;
    if(m->mapa[ID(i,j)]->cor == minha_cor) { // Achei outro cara com a minha cor. Seta status.
        m->mapa[ID(i,j)]->status = status;
        flood_set_status(m, i+1, j, minha_cor, status);
        flood_set_status(m, i-1, j, minha_cor, status);
        flood_set_status(m, i, j+1, minha_cor, status);
        flood_set_status(m, i, j-1, minha_cor, status);
    }
    return; // Se cheguei aqui, eh porque essa celula nao eh da mesma cor, entao ignora.
}

void flood_set_status_fake(tmapa *m, int i, int j, int minha_cor, int status) {
    // Antes de chamar isso, eu quero setar counted como 0 (usar zera_counted)!
    if(borda(i,j)) {
        return;
    }
    if(m->mapa[ID(i,j)]->counted)
        return;
    m->mapa[ID(i,j)]->counted = 1;
    if(m->mapa[ID(i,j)]->cor == minha_cor || m->mapa[ID(i,j)]->pintado) { // Achei outro cara com a minha cor. Seta status.
        m->mapa[ID(i,j)]->status = status;
        flood_set_status(m, i+1, j, minha_cor, status);
        flood_set_status(m, i-1, j, minha_cor, status);
        flood_set_status(m, i, j+1, minha_cor, status);
        flood_set_status(m, i, j-1, minha_cor, status);
    }
    return; // Se cheguei aqui, eh porque essa celula nao eh da mesma cor, entao ignora.
}

void bloco_baixo_direita(int *x1, int *y1, int *x2, int *y2) {
    *x1 = (int) (Linhas * 3) / 4;
    *y1 = (int) (Colunas * 3) / 4;
    *x2 = Linhas;
    *y2 = Colunas;
}
