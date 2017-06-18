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

    printf("\n%d jogadas.\n", Njogadas);

    return 0;
}
