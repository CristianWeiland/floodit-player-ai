#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define N 700000000
#define MAX_JOGADAS 200

#define GERANDO 1
#define TESTANDO 0

#define MODO GERANDO

int Head, Tail, Seed;
int8_t *fila;

typedef struct tmapa tmapa;

struct tmapa {
    int nlinhas;
    int ncolunas;
    int ncores;
    int **mapa;
};

void gera_mapa(tmapa *m, int semente) {
    int i, j;

    if(semente < 0)
        srand(time(NULL));
    else
        srand(semente);
    m->mapa = (int**) malloc(m->nlinhas * sizeof(int*));
    if(!m->mapa) {
        printf("(gera_mapa) Erro mallocando mapa.\n");
        exit(1);
    }
    for(i = 0; i < m->nlinhas; i++) {
        m->mapa[i] = (int*) malloc(m->ncolunas * sizeof(int));
        if(!m->mapa[i]) {
            printf("(gera_mapa) Erro mallocando mapa[i].\n");
            exit(1);
        }
        for(j = 0; j < m->ncolunas; j++)
            m->mapa[i][j] = 1 + rand() % m->ncores;
    }
}

void mostra_mapa(tmapa *m) {
    int i, j;

    printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
        printf("%02d ", m->mapa[i][j]);
            else
        printf("%d ", m->mapa[i][j]);
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

    if(m->ncores > 15) {
        mostra_mapa(m);
        return;
    }
    printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
        printf("%s%02d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
            else
        printf("%s%d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
        printf("\n");
    }
}

void pinta(tmapa *m, int l, int c, int fundo, int cor) {
    m->mapa[l][c] = cor;
    if(l < m->nlinhas - 1 && m->mapa[l+1][c] == fundo)
        pinta(m, l+1, c, fundo, cor);
    if(c < m->ncolunas - 1 && m->mapa[l][c+1] == fundo)
        pinta(m, l, c+1, fundo, cor);
    if(l > 0 && m->mapa[l-1][c] == fundo)
        pinta(m, l-1, c, fundo, cor);
    if(c > 0 && m->mapa[l][c-1] == fundo)
        pinta(m, l, c-1, fundo, cor);
}

void pinta_mapa(tmapa *m, int cor) {
    if(cor == m->mapa[0][0])
        return;
    pinta(m, 0, 0, m->mapa[0][0], cor);
}

tmapa* copia_tmapa(tmapa *m) {
    int i, j;
    tmapa *n = (tmapa *) malloc(sizeof(struct tmapa));
    if(!n) {
        puts("(copia_tmapa) Erro allocando n.");
        exit(1);
    }
    n->ncores = m->ncores;
    n->nlinhas = m->nlinhas;
    n->ncolunas = m->ncolunas;
    n->mapa = (int **) malloc(n->nlinhas * sizeof(int *));
    if(!n->mapa) {
        puts("(copia_tmapa) Erro allocando n->mapa.");
        exit(1);
    }
    for(i=0; i<n->nlinhas; ++i) {
        n->mapa[i] = (int *) malloc(n->ncolunas * sizeof(int));
        if(!n->mapa[i]) {
            puts("(copia_tmapa) Erro allocando n->mapa[i].");
            exit(1);
        }
        for(j=0; j<n->ncolunas; ++j) {
            n->mapa[i][j] = m->mapa[i][j];
        }
    }
    return n;
}

void destroi_tmapa(tmapa *n) {
    int i;
    for(i=0; i<n->nlinhas; ++i) {
        free(n->mapa[i]);
    }
    free(n->mapa);
    free(n);
}

int acabou(tmapa m) {
    int i, j, cor = m.mapa[0][0];
    for(i=0; i<m.nlinhas; ++i) {
        for(j=0; j<m.ncolunas; ++j) {
            if(m.mapa[i][j] != cor) {
                return 0;
            }
        }
    }
    return 1;
}

int backtrack(tmapa *m) {
    int i, j, njogadas;
    tmapa *n;
    int jogadas[MAX_JOGADAS];

    for(i=m->ncores*2; i<N; ++i) {
        fila[i] = -2;
    }
    for(i=0; i<m->ncores*2; i+=2) {
        fila[i] = i/2+1;
        fila[i+1] = -1;
    }
    Head = 0;
    Tail = 2*m->ncores;

    while(Head < Tail) {
        if(Tail > N) {
            printf("Tail got too big (%d). We are doomed...\n", Tail);
        }
        n = copia_tmapa(m);
        for(i=0; i<MAX_JOGADAS; ++i) {
            jogadas[i] = -1;
        }

        // Le jogadas:
        for(njogadas = 0; fila[Head] != -1; ++Head, ++njogadas) {
            jogadas[njogadas] = fila[Head];
        }
        // Faz apontar pro proximo que não é -1.
        ++Head;

        for(i=0; i<njogadas; ++i) {
            pinta_mapa(n, jogadas[i]);
        }

        if(acabou(*n)) {
            if(MODO == TESTANDO) {
                printf("%d\n", njogadas);
            } else if(MODO == GERANDO) {
                printf("%d\n", Seed);
                printf("%d\n", njogadas);
                for(i=0; i<njogadas; ++i) {
                    printf("%d\n", jogadas[i]);
                }
            }
            exit(0);
        }

        for(i=0; i<m->ncores; ++i) {
            for(j=0; j<njogadas; ++j, ++Tail) {
                fila[Tail] = jogadas[j];
            }
            fila[Tail] = i+1;
            fila[Tail+1] = -1;
            Tail += 2;
        }
        destroi_tmapa(n);
    }
}

int main(int argc, char **argv) {
    int cor;
    tmapa m, *n;
    int semente;

    if(argc < 4 || argc > 5) {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }

    fila = (int8_t*) malloc(N * sizeof(int8_t));

    m.nlinhas = atoi(argv[1]);
    m.ncolunas = atoi(argv[2]);
    m.ncores = atoi(argv[3]);

    if(argc == 5)
        semente = atoi(argv[4]);
    else
        semente = -1;
    Seed = semente;
    gera_mapa(&m, semente);
    //mostra_mapa_cor(&m);

    backtrack(&m);
    free(fila);
    return 0;
}
