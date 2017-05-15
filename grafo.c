#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "mapa.h"
#include "grafo.h"

vertice mais_proximo(tmapa *m, int x1, int y1, int x2, int y2) {
    // Procura o vértice mais próximo dentro do quadrado formado pelos pontos p1 e p2 (coordenadas x1, y1 e x2, y2, respectivamente)
    // Obs: x1 tem que ser menor que x2 e y1 menor que y2!
    int i, j;
    vertice menor = m->mapa[ID(i,j)]->v;
    for(i=x1; i<x2; ++i) {
        for(j=y1; j<y2; ++j) {
            if(m->mapa[ID(i,j)]->v->d < menor->d) {
                menor = m->mapa[ID(i,j)]->v;
            }
        }
    }
    return menor;
}

void cria_vertices(tmapa *m, grafo g) {
    int i, j;
    celula c;
    vertice v;

    zera_counted(m);

    for(i=0; i<m->nlinhas; ++i) {
        for(j=0; j<m->ncolunas; ++j) {
            c = m->mapa[ID(i,j)];
            if(c->counted) {
                continue;
            }
            v = insere_vertice(g, c->cor, VerticeID);
            ++VerticeID;
            m->mapa[ID(i,j)]->v = v;
            adiciona_vizinhanca(m, i, j, v);
        }
    }
    return;
}

void adiciona_vizinhanca(tmapa *m, int i, int j, vertice v) {
    if(borda(i,j))
        return;
    celula c = m->mapa[ID(i,j)];
    if(c->counted || v->cor != c->cor)
        return;
    //printf("Vertice %d ganhou %d e %d na posicao %d.\n", v->id, i, j, v->elems);
    v->i[v->elems] = i;
    v->j[v->elems] = j;
    ++(v->elems);
    m->mapa[ID(i,j)]->v = v;
    m->mapa[ID(i,j)]->counted = COUNTED;
    adiciona_vizinhanca(m, i+1, j, v);
    adiciona_vizinhanca(m, i-1, j, v);
    adiciona_vizinhanca(m, i, j+1, v);
    adiciona_vizinhanca(m, i, j-1, v);
    return;
}

void cria_arestas(tmapa *m, grafo g) {
    no elem;
    int i, j, repetido, head, tail = 1; // Head vai indicar qual elemento eu devo olhar em l, pois nao vou remove-los, apenas ignora-los.
                              // Tail serve pra saber onde eu devo adicionar o proximo elemento e quando parar o laco.
    //vertice l[g->len*g->len*g->len], v;
    vertice *l, v;
    aresta a;

    //printf("Alocando %d bytes... Len = %d\n", g->len * g->len * g->len * sizeof(vertice), g->len);
    l = (vertice *) malloc(g->len * g->len * g->len * sizeof(vertice));


    /*
        Porque nao allocar soh g->len pra l? A resposta eh porque as vezes eu adiciono o mesmo vertice mais do que uma vez.
        Isso muitas vezes eh ruim, mas nao sempre. Por exemplo: Se eu achei um caminho menor pro vertice v, entao v->d vai mudar pra um numero menor.
        Isso significa que eu devo conferir se suas arestas estao certas, pois eh bem provavel que isso crie uma reacao em cadeia que vai achar
        caminhos menores pra varios outros vertices.
    */

    zera_counted(m);

    l[0] = m->mapa[0]->v;

    // Inicializa d pra todos os vertices
    for(elem = primeiro_no(g->v); elem; elem = proximo_no(elem)) {
        v = (vertice) conteudo(elem);
        v->d = INITIAL_DISTANCE;
    }

    m->mapa[0]->v->d = 0;

    for(head = 0; head < tail; ++head) {
        v = l[head];
        // ++head "remove" um elemento da lista.
        pega_vizinhos(m, v->i[0], v->j[0], v);
        for(elem = primeiro_no(v->saida); elem; elem = proximo_no(elem)) {
            // Acrescenta mais elementos na lista pra continuar esse laco.
            a = (aresta) conteudo(elem);
            repetido = 0;
            for(j=head; j<tail; ++j) {
                if(a->vc == l[j]) {
                    repetido = 1;
                }
            }
            if(!repetido) {
                l[tail] = a->vc;
                ++tail;
            }
        }
    }
    //printf("Usei %d elementos.\n", tail);

    free(l);
}

void pega_vizinhos(tmapa *m, int i, int j, vertice v) {
    if(borda(i,j) == 1) {
        return;
    }
    if(m->mapa[ID(i,j)]->cor == v->cor) { // Achei outro cara do meu vertice.
        // Isso elimina o Quadrado Magico.
        // Se eu contei ele, nao faco nada. Se nao contei ele ainda, digo que contei e chamo recursivamente.
        if(!(m->mapa[ID(i,j)]->counted)) {
            m->mapa[ID(i,j)]->counted = 1;
            pega_vizinhos(m, i+1, j, v);
            pega_vizinhos(m, i-1, j, v);
            pega_vizinhos(m, i, j+1, v);
            pega_vizinhos(m, i, j-1, v);
        }
        return;
    }

    // Se cheguei aqui, eh porque a cor dele eh diferente.
    // Entao eu devo verificar alguns casos:
    // 1- Ja existe algum caminho menor pra esse vertice.
    // 2- Ja existe algum caminho de mesmo tamanho que o meu pra esse vertice.
    // 3- Nao existe nenhum caminho pra esse vertice.
    // 4- Ja existe algum caminho maior que o meu pra esse vertice.

    // O que fazer em cada caso:
    // 1- Nao faco nada. Nao quero adicionar um caminho maior, isso pode dar treta nos algoritmos depois.
    // 2- Cria mais uma aresta. Quero considerar os dois caminhos, pois eles tem tamanho igual.
    // 3- Cria uma aresta e inicializa a distancia dele como (v->d) + 1.
    // 4- Deleta todas as arestas que existiam antes, que fazem parte da lista de vertices de entrada do vertice destino (m->mapa[ID(i,j)]->v->entrada).
    //    Feito isso, eu entro no caso 3: nao existe nenhum caminho pra esse vertice.

    int destDist = m->mapa[ID(i,j)]->v->d;
    int novaDistancia = (v->d) + 1; // Essa variavel contem a distancia que o vertice teria se passando por v fosse o menor caminho.

    // Caso 1: Ja existe algum caminho menor pra esse vertice.
    if( (destDist != INITIAL_DISTANCE) && (destDist < novaDistancia) ) {
        return;
    }

    // Caso 4:
    // Obs: Perceba que aqui nao tem return;, porque eu quero executar os passos do caso 3!
    if( destDist > novaDistancia ) {
        printf("Acho acho ololoooo\n\n");
        no elem;
        aresta a;
        // Preciso remover as arestas ja existentes. Pra isso tenho dois passos:
        for(elem = primeiro_no(m->mapa[ID(i,j)]->v->entrada); elem; elem = proximo_no(elem)) {
            a = (aresta) conteudo(elem);
            // Primeiro: Removo a aresta da lista de arestas de cada vertice.
            if(!remove_no(a->vs->saida, (void *) a, NULL)) {
                printf("Erro tirando aresta da lista de arestas do vertice a->vs->saida. Talvez eh a->vs->entrada?\n");
            }
            if(!remove_no(a->vc->entrada, (void *) a, NULL)) {
                printf("Erro tirando aresta da lista de arestas do vertice a->vc->entrada. Talvez eh a->vc->saida?\n");
            }
            // Segundo: Destruo a aresta.
            destroi_aresta(a);
        }
        // Nao deve ter return; aqui!
    }

    // Casos 2 e 3:
    //    Distancia eh a mesma      ou  nao existe nenhum caminho ainda.
    if( (destDist == novaDistancia) || (destDist == INITIAL_DISTANCE) ) {
        // Cria mais uma aresta, de v->w. Nao cria se ja existir alguma aresta v->w.
        // Pra testar isso, vou percorrer todas as arestas de saida de v e ver se alguma chega em w.
        no elem;
        int devoInserir = 1;
        for(elem = primeiro_no(v->saida); elem; elem = proximo_no(elem)) {
            if( ((aresta)conteudo(elem))->vc->id == m->mapa[ID(i,j)]->v->id)
                devoInserir = 0;
        }
        if(devoInserir)
            insere_aresta(v, m->mapa[ID(i,j)]->v, 0);
        // Nao faz mal eu fazer essa atribuicao, pois: se eh o caso 2, nao vai mudar nada; se eh o caso 1, vou setar a distancia corretamente.
        m->mapa[ID(i,j)]->v->d = novaDistancia;
        return;
    }
    printf("Oopsie! Nao era pra chegar aqui...\n");
    return;
}

grafo cria_grafos(tmapa *m) {
    grafo g = constroi_grafo();
    cria_vertices(m, g);
    cria_arestas(m, g);

    no elem;
    vertice v;
    // Acha o primeiro vertice da lista.
    for(elem = primeiro_no(g->v); elem; elem = proximo_no(elem)) {
        v = (vertice) conteudo(elem);
        if(v->d == 0) {
            g->lider = v;
            break;
        }
    }
    return g;
}
