#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ia.h"
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

//    m->mapa = (int*) malloc(m->nlinhas * m->ncolunas * sizeof(int));
    m->mapa = (celula *) malloc(m->nlinhas * m->ncolunas * sizeof(struct celula));

    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++) {
            m->mapa[ID(i,j)].cor = 1 + rand() % m->ncores;
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
    //m->mapa = (int*) malloc(m->nlinhas * m->ncolunas * sizeof(int));
    m->mapa = (celula *) malloc(m->nlinhas * m->ncolunas * sizeof(struct celula));
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++) {
            //if(!scanf("%d", &(m->mapa[ID(i,j)])))
            if(!scanf("%d", &(m->mapa[ID(i,j)].cor)))
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
	printf("%02d ", m->mapa[ID(i,j)].cor);
            else
	printf("%d ", m->mapa[ID(i,j)].cor);
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
    printf("Linhas: %d Colunas: %d Cores: %d\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
	printf("%s%02d%s ", cor_ansi[m->mapa[ID(i,j)].cor], m->mapa[ID(i,j)].cor, cor_ansi[0]);
            else
	printf("%s%d%s ", cor_ansi[m->mapa[ID(i,j)].cor], m->mapa[ID(i,j)].cor, cor_ansi[0]);
        printf("\n");
    }
    puts("");
}

void pinta(tmapa *m, int l, int c, int fundo, int cor) {
    m->mapa[ID(l,c)].cor = cor;
    if(l < m->nlinhas - 1 && m->mapa[ID(l+1,c)].cor == fundo)
        pinta(m, l+1, c, fundo, cor);
    if(c < m->ncolunas - 1 && m->mapa[ID(l,c+1)].cor == fundo)
        pinta(m, l, c+1, fundo, cor);
    if(l > 0 && m->mapa[ID(l-1,c)].cor == fundo)
        pinta(m, l-1, c, fundo, cor);
    if(c > 0 && m->mapa[ID(l,c-1)].cor == fundo)
        pinta(m, l, c-1, fundo, cor);
}

void pinta_mapa(tmapa *m, int cor) {
    if(cor == m->mapa[ID(0,0)].cor)
        return;
    pinta(m, 0, 0, m->mapa[ID(0,0)].cor, cor);
}

int acabou(tmapa m) {
    int i, cor = m.mapa[0].cor;
    for(i=1; i<m.tam; ++i) {
        if(m.mapa[i].cor != cor) {
            return 0;
        }
    }
    return 1;
}

inline int borda(int i, int j) {
    if(i < 0 || j < 0 || i >= Linhas || j >= Colunas)
        return 1;
    return 0;
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

void cria_vertices(tmapa m, grafo g) {
    int i, j;
    celula c;
    vertice v;

    mostra_mapa_cor(&m, 0);

    for(i=0; i<m.tam; ++i) {
        m.mapa[i].counted = 0;
    }
    for(i=0; i<m.nlinhas; ++i) {
        for(j=0; j<m.ncolunas; ++j) {
            c = m.mapa[ID(i,j)];
            if(c.counted) {
                continue;
            }
            v = insere_vertice(g, c.cor, i, j, VerticeID);
            ++VerticeID;
            c.v = v;
            c.counted = 1;
            adiciona_vizinhanca(m, i, j, v);
        }
    }
    return;
}

void adiciona_vizinhanca(tmapa m, int i, int j, vertice v) {
    if(borda(i,j))
        return;
    celula c = m.mapa[ID(i,j)];
    if(c.counted || v->cor != c.cor)
        return;
    ++(v->elems);
    m.mapa[ID(i,j)].v = v;
    m.mapa[ID(i,j)].counted = COUNTED;
    adiciona_vizinhanca(m, i+1, j, v);
    adiciona_vizinhanca(m, i-1, j, v);
    adiciona_vizinhanca(m, i, j+1, v);
    adiciona_vizinhanca(m, i, j-1, v);
    return;
}

void cria_arestas() {}

grafo cria_grafos(tmapa m) {
    grafo g = constroi_grafo();
    cria_vertices(m, g);
    cria_arestas();
    return g;
}

int proxima_jogada(tmapa m) {
    return rand() % m.ncores + 1;
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

    if(argc == 5)
        semente = atoi(argv[4]);
    else
        semente = -1;

    gera_mapa(&m, semente);

    g = cria_grafos(m);

    escreve_grafo(stdout, g);

    while(!acabou(m)) {
        //mostra_mapa_cor(&m, 1); // para mostrar sem cores use mostra_mapa(&m);
        cor = proxima_jogada(m);
        pinta_mapa(&m, cor);
        ++Njogadas;
    }

    //mostra_mapa_cor(&m, 1); // para mostrar sem cores use mostra_mapa(&m);

    printf("Parabens! Voce venceu em %d jogadas!\n", Njogadas);

    return 0;
}
/*
int pega_informacoes_fronteira(info *dados, tmapa m) {
    int i, j, minha_cor, *map;

    map = m.mapa;
    minha_cor = map[0];

    for(i=0; i<dados->ncores; ++i) {
        fronteira[i] = 0;
    }

    for(i=0; i<m.nlinhas; ++i) {
        for(j=0; j<m.ncolunas; ++j) {
            // Se eh da minha cor, ignora.
            if(fronteira_externa(map, i, j)) {
                dados->fronteira[map[ID(i,j)]]++;
            }
        }
    }
}
*/
