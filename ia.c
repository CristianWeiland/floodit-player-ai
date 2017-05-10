#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ia.h"
#include "utils.h"

// TODO: Destruir os grafos nas iteracoes, ou parar de recria-lo e soh atualiza-lo.

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

    m->mapa = (celula *) malloc(m->nlinhas * m->ncolunas * sizeof(celula));
    if(!m->mapa) {
        puts("(jogada_otima) Erro de malloc.");
        exit(1);
    }

    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++) {
            m->mapa[ID(i,j)] = (celula) malloc(sizeof(struct celula));
            if(!m->mapa[ID(i,j)]) {
                puts("(jogada_otima) Erro de malloc.");
                exit(1);
            }
            m->mapa[ID(i,j)]->cor = 1 + rand() % m->ncores;
            m->mapa[ID(i,j)]->counted = 0;
            m->mapa[ID(i,j)]->v = NULL;
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
    m->mapa = (celula *) malloc(m->nlinhas * m->ncolunas * sizeof(struct celula));
    if(!m->mapa) {
        puts("(jogada_otima) Erro de malloc.");
        exit(1);
    }
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++) {
            m->mapa[ID(i,j)] = (celula) malloc(sizeof(struct celula));
            if(!m->mapa[ID(i,j)]) {
                puts("(jogada_otima) Erro de malloc.");
                exit(1);
            }
            m->mapa[ID(i,j)]->counted = 0;
            m->mapa[ID(i,j)]->v = NULL;
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

int jogada_random(tmapa m) {
    return rand() % m.ncores + 1;
}

int guloso(tmapa m, grafo g) {
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
    if(r != -1) {
        return r;
    }

    // Metodo 2: Guloso
    return guloso(m, g);

    // Metodo 1: Random
    return jogada_random(m);
}

tmapa* copia_tmapa(tmapa *m) {
    // Não testado!
    int i;
    tmapa *n = (tmapa *) malloc(sizeof(struct tmapa));
    memcpy(n, m, sizeof(struct tmapa));
    n->mapa = (celula *) malloc(n->tam * sizeof(celula));
    // Não dá pra fazer um memcpy só porque é um struct celula**, ou seja, a memória tá toda perdida por aí...
    for(i=0; i<n->tam; ++i) {
        n->mapa[i] = (celula) malloc(sizeof(struct celula));
        memcpy(n->mapa[i], m->mapa[i], sizeof(struct celula));
    }
    return n;
}

void destroi_tmapa(tmapa m, int destruir_vertices) {
    int i;
    for(i=0; i<m.tam; ++i) {
        if(destruir_vertices && m.mapa[i]->v) {
            free(m.mapa[i]->v);
        }
        free(m.mapa[i]);
    }
    free(m.mapa);
}

grafo atualiza_grafo(grafo g, int cor) {
    no elem, child;
    vertice w;
    aresta a;
    int i;
    for(elem = primeiro_no(g->first->saida); elem; elem = proximo_no(elem)) {
        w = (vertice) conteudo(elem);
        // Só quero vizinhos da mesma cor, foram os que eu pintei.
        if(w->cor != cor)
            continue;
        // Adiciona *i e *j de w em first.
        for(i=0; i<w->elems; ++i) {
            g->first->i[g->first->elems + i] = w->i[i];
            g->first->j[g->first->elems + i] = w->j[i];
        }
        g->first->elems += w->elems;
        /* Acho que aqui eu arrumei os vertices. Provavelmente se eu chamar o cria_arestas() funciona. Pensar nisso!
        for(child = primeiro_no(w->saida); child; child = proximo_no(child)) {
            a = (aresta) conteudo(child);
            a->vs = g->first;
        }
        */
        destroi_lista(w->saida, NULL);
        destroi_lista(w->entrada, destroi_aresta);
        destroi_vertice(w);
    }

    return g;
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
    escreve_grafo(stdout, g);

    while(!acabou(m)) {
        //mostra_mapa_cor(&m, 0); // para mostrar sem cores use mostra_mapa(&m);
        cor = proxima_jogada(m, g);
        //printf("A cor selecionada eh %d\n", cor);
        pinta_mapa(&m, cor);
        // Precisa destruir o grafo g!
        destroi_grafo(g);
        g = cria_grafos(&m);
        ++Njogadas;
    }

    //mostra_mapa_cor(&m, 1); // para mostrar sem cores use mostra_mapa(&m);

    printf("Parabens! Voce venceu em %d jogadas!\n", Njogadas);

    destroi_grafo(g);
    destroi_tmapa(m, 0);

    return 0;
}
