#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ia.h"
#include "utils.h"

int callCount = 0;

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
    m->mapa = (celula *) malloc(m->nlinhas * m->ncolunas * sizeof(celula));

    printf("Tamanhos: celula = %d, struct celula = %d\n", sizeof(celula), sizeof(struct celula));

    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++) {
            m->mapa[ID(i,j)] = (celula) malloc(sizeof(struct celula));
            m->mapa[ID(i,j)]->cor = 1 + rand() % m->ncores;
            m->mapa[ID(i,j)]->counted = 0;
            m->mapa[ID(i,j)]->v = NULL;
            printf("Aloquei pra %d e %d, cor = %d, counted = %d, v = %p\n", i, j, m->mapa[ID(i,j)]->cor, m->mapa[ID(i,j)]->counted, m->mapa[ID(i,j)]->v);
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
            if(!scanf("%d", &(m->mapa[ID(i,j)]->cor)))
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
        printf("%d: ", i);
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
	printf("%s%02d%s ", cor_ansi[m->mapa[ID(i,j)]->cor], m->mapa[ID(i,j)]->cor, cor_ansi[0]);
            else
	printf("%s%d%s ", cor_ansi[m->mapa[ID(i,j)]->cor], m->mapa[ID(i,j)]->cor, cor_ansi[0]);
        printf("\n");
    }
    puts("");
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

inline void zera_counted(tmapa *m) {
    int i;
    for(i=0; i<m->tam; ++i) {
        m->mapa[i]->counted = 0;
    }
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
    int i, j, head, tail = 1; // Head vai indicar qual elemento eu devo olhar em l, pois nao vou remove-los, apenas ignora-los.
                              // Tail serve pra saber onde eu devo adicionar o proximo elemento e quando parar o laco.
    int auxi, auxj, len = g->len;
    printf("Len = %d\n", len);
    //vertice l[len], v;
    vertice l[5*len], v; // Treta: Porque nao usa soh g->len? Obviamente estou repetindo vertices, mas nao sei porque. Possivelmente quando acho um caminho de mesmo tamanho a partir de dois vertices. Ex:
    // 1 2
    // 3 4
    // Como caminho pra 4 pode ser 1->2->4 e 1->3->4, provavelmente eu adiciono o 4 duas vezes.
    // Alem disso, eh possivel que ao achar um caminho menor pra um vertice eu tambem coloque ele mais de uma vez. Isso NAO eh ruim!
    aresta a;

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
        //printf("Rodando vertice %d com indice %d, v = %p, i = %p e j = %p. Elems = %d.\n", v->id, head, v, v->i, v->j, v->elems);
        //printf("Rodando vertice %d com indice %d, elems = %d, i[0] = %d, j[0] = %d.\n", v->id, head, v->elems, v->i[0], v->j[0]);
        //printf("Elementos sao i = %d, j = %d.\n", (v->i)[0], (v->j)[0]);
        auxi = (v->i)[0];
        auxj = (v->j)[0];
        // Possivelmente esse laco vai dar loop infinito, uma aresta adicionando outra.
        pega_vizinhos(m, v->i[0], v->j[0], v, m->mapa);
        //printf("Inserindo M = %p, m->mapa = %p, v = %p.\n", m, m->mapa, v);
        for(elem = primeiro_no(v->saida); elem; elem = proximo_no(elem)) {
            // Acrescenta mais elementos na lista pra continuar esse laco.
            //printf("(0) Ate agora, M = %p, m->mapa = %p, conteudo = %p, l = %p, tail = %d.\n", m, m->mapa, conteudo(elem), l, tail);
            a = (aresta) conteudo(elem);
            l[tail] = a->vc;
            ++tail;
        }
    }
    puts("Terminei!");
}

void pega_vizinhos(tmapa *m, int i, int j, vertice v, celula* x) {
    if(borda(i,j) == 1) {
        //printf("Bati na borda, i = %d e j = %d.\n", i, j);
        return;
    }
    //printf("Dados: i = %d, j = %d, Cor = %d e %d, novaDist = %d, destDist = %d\n", i, j, m->mapa[ID(i,j)]->cor, v->cor, (v->d)+1, m->mapa[ID(i,j)]->v->d);
    //printf("Dados: i = %d, j = %d, Cor = %d, novaDist = %d", i, j, v->cor, (v->d)+1);
    if(m->mapa[ID(i,j)]->cor == v->cor) { // Achei outro cara do meu vertice.
        // Isso elimina o Quadrado Magico.
        // Se eu contei ele, nao faco nada. Se nao contei ele ainda, digo que contei e chamo recursivamente.
        //printf("Mesma cor");
        if(!(m->mapa[ID(i,j)]->counted)) {
            //printf(", Mesma cor, contado.\n");
            m->mapa[ID(i,j)]->counted = 1;
            pega_vizinhos(m, i+1, j, v, m->mapa);
            pega_vizinhos(m, i-1, j, v, m->mapa);
            pega_vizinhos(m, i, j+1, v, m->mapa);
            pega_vizinhos(m, i, j-1, v, m->mapa);
        }
        //printf(", ou talvez nao?\n");
        //printf("\n");
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
        // Cria mais uma aresta.
        // Nao insere aresta de v->w se ja existir alguma aresta de v->w. Pra ver isso, vou percorrer todas as arestas de saida de v e ver se alguma chega em w.
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














/*
void pega_vizinhos(tmapa *m, int i, int j, vertice v) {
    //printf("i = %d, j = %d\n", i, j);
    callCount++;
    if(callCount > 100)
        return;
    if(borda(i,j) == 1) {
        printf("Bordinha.\n");
        return;
    }
    celula c = m->mapa[ID(i,j)];
    if((c->v->d < (v->d+1) && c->v->d != -1) || (c->counted && c->v->d != v->d)) {
        printf("Bordinha 1.\n");
        return;
    }
    if(c->v != v) {
        if(c->v != v && c->v->d == -1) {
            printf("Achei um caminho novo!.\n");
            m->mapa[ID(i,j)]->v->d = (v->d) + 1;
            insere_aresta(v, m->mapa[ID(i,j)]->v, 0);
            return;
        }
        //int aux = c->v->d;
        //printf("d eh %d e %d\n", c->v->d, aux);
        //if((c->v->d < (v->d + 1)) && aux != -1)
            // Existe um caminho menor pra essa aresta, que nao sou eu.
            //printf("Caminho menor.\n");
            //return;
        if(c->v->d == (v->d + 1)) {
            // Achei um outro caminho de mesmo tamanho pra essa aresta. Cria isso como outra aresta.
            insere_aresta(v, m->mapa[ID(i,j)]->v, 0);
            puts("Outro caminho, mesmo tamanho.");
            printf("Bordinha 4.\n");
            return;
        }
        if(c->v->d > (v->d + 1)) {
            // Achei um caminho menor pra aresta. Isso provavelmente nao deve acontecer.
            puts("WARNING!! WARNING!! WARNING!! VAI DAR RUIM!! LEIA COMENTARIO NA FUNCAO PEGA_VIZINHOS!!");
            puts("WARNING!! WARNING!! WARNING!! VAI DAR RUIM!! LEIA COMENTARIO NA FUNCAO PEGA_VIZINHOS!!");
            puts("WARNING!! WARNING!! WARNING!! VAI DAR RUIM!! LEIA COMENTARIO NA FUNCAO PEGA_VIZINHOS!!");
            puts("WARNING!! WARNING!! WARNING!! VAI DAR RUIM!! LEIA COMENTARIO NA FUNCAO PEGA_VIZINHOS!!");
            puts("WARNING!! WARNING!! WARNING!! VAI DAR RUIM!! LEIA COMENTARIO NA FUNCAO PEGA_VIZINHOS!!");
            no elem;
            for(elem = primeiro_no(c->v->saida); elem; elem = proximo_no(elem)) {
                destroi_aresta(conteudo(elem)); // Isso nao vai funcionar. Eu nao to removendo das listas de arestas dos vertices. Nao vou arrumar isso por enquanto porque acho que esse caso nunca vai acontecer.
            }
            m->mapa[ID(i,j)]->v->d = (v->d) + 1;
            insere_aresta(v, m->mapa[ID(i,j)]->v, 0);
            return;
        }
    }
    puts("Caso 6.");
    m->mapa[ID(i,j)]->counted = 1;
    pega_vizinhos(m, i+1, j, v);
    pega_vizinhos(m, i-1, j, v);
    pega_vizinhos(m, i, j+1, v);
    pega_vizinhos(m, i, j-1, v);
    return;
}
*/
grafo cria_grafos(tmapa *m) {
    grafo g = constroi_grafo();
    cria_vertices(m, g);
    escreve_grafo(stdout, g);
    mostra_mapa_cor(m, 0);
    cria_arestas(m, g);
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
    TamMatriz = Linhas * Colunas;

    if(argc == 5)
        semente = atoi(argv[4]);
    else
        semente = -1;

    gera_mapa(&m, semente);

    g = cria_grafos(&m);

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
