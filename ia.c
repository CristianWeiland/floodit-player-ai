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
		Sequencia[Njogadas] = jogadas[i];
        ++Njogadas;
    }
    free(jogadas);

    while(!acabou(*m)) {
        cor = guloso_fronteira_externa_com_otima(m);
        pinta_mapa(m, cor);
		Sequencia[Njogadas] = cor;
        ++Njogadas;
    }

    destroi_tmapa(m, 0);

    return Njogadas;
}

int main(int argc, char **argv) {
    int cor, semente;
    tmapa m;

	scanf("%d %d %d ", &(m.nlinhas), &(m.ncolunas), &(m.ncores));
    m.tam = m.nlinhas * m.ncolunas;

    Njogadas = 0;
    Linhas = m.nlinhas;
    Colunas = m.ncolunas;
    TamMatriz = Linhas * Colunas;

	Sequencia = malloc(TamMatriz * sizeof(int));

    gera_mapa(&m, semente);

    resolve(&m);

    printf("%d\n", Njogadas);

	int i;
	for(i=0; i<Njogadas; ++i) {
		printf("%d ", Sequencia[i]);
	}
	printf("\n");

    return 0;
}
