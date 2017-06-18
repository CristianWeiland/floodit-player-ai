#include <malloc.h>
#include "utils.h"

//---------------------------------------------------------------------------
// devolve o número de nós da lista l

unsigned int tamanho_lista(lista l) { return l->tamanho; }

// Imprime uma lista
void imprime_lista(lista l, void imprime_elemento(void *));

void imprime_lista(lista l, void imprime_elemento(void *)) {
    no elem;
    for(elem = l->primeiro; elem; elem = elem->proximo) {
        imprime_elemento(elem->conteudo);
    }
    return ;
}

//---------------------------------------------------------------------------
// devolve o primeiro nó da lista l,
//      ou NULL, se l é vazia

no primeiro_no(lista l) { return l->primeiro; }

//---------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo(no n) { return n->conteudo; }

//---------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da lista

no proximo_no(no n) { return n->proximo; }

//---------------------------------------------------------------------------
// cria uma lista vazia e a devolve
//
// devolve NULL em caso de falha

lista constroi_lista(void) {

  lista l = malloc(sizeof(struct lista));

  if ( ! l ) 
    return NULL;

  l->primeiro = NULL;
  l->tamanho = 0;

  return l;
}
//---------------------------------------------------------------------------
// desaloca a lista l e todos os seus nós
// 
// se destroi != NULL invoca
//
//     destroi(conteudo(n)) 
//
// para cada nó n da lista. 
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha

int destroi_lista(lista l, int destroi(void *)) {

  no p;
  int ok=1;

  while ( (p = primeiro_no(l)) ) {

    l->primeiro = proximo_no(p);

    if ( destroi )
      ok &= destroi(conteudo(p));

    free(p);
  }

  free(l);

  return ok;
}
//---------------------------------------------------------------------------
// insere um novo nó na lista l cujo conteúdo é p
//
// devolve o no recém-criado 
//      ou NULL em caso de falha

no insere_lista(void *conteudo, lista l) { 

  no novo = malloc(sizeof(struct no));

  if ( ! novo ) 
    return NULL;

  novo->conteudo = conteudo;
  novo->proximo = primeiro_no(l);
  ++l->tamanho;
  
  return l->primeiro = novo;
}

//------------------------------------------------------------------------------
//

int remove_no(struct lista *l, struct no *rno, int destroi(void *)) {
    int r = 1;
    if (l->primeiro == rno) {
        l->primeiro = rno->proximo;
        if (destroi != NULL) {
            r = destroi(conteudo(rno));
        }
        free(rno);
        l->tamanho--;
        return r;
    }
    for (no n = primeiro_no(l); n->proximo; n = proximo_no(n)) {
        if (n->proximo == rno) {
            n->proximo = rno->proximo;
            if (destroi != NULL) {
                r = destroi(conteudo(rno));
            }
            free(rno);
            l->tamanho--;
            return r;
        }
    }
    return 0;
}

grafo constroi_grafo(void) {
    grafo g;
    g = (grafo) malloc(sizeof(struct grafo));
    if(g == NULL) {
        perror("(constroi_grafo) Erro ao allocar memoria para o grafo.");
        return NULL;
    }
    g->len = 0;
    g->v = constroi_lista();
    return g;
}

vertice constroi_vertice(void) {
    vertice v = malloc(sizeof(struct vertice));
    if(v == NULL) {
        perror("(constroi_vertice) Erro ao allocar memoria para o vertice.");
        return NULL;
    }
    if(!(v->i = (int *) malloc(TamMatriz * sizeof(int))))
        puts("Erro ao allocar memoria para i do vertice.");
    if(!(v->j = (int *) malloc(TamMatriz * sizeof(int))))
        puts("Erro ao allocar memoria para i do vertice.");
    if(!(v->saida = constroi_lista()))
        puts("Erro ao construir lista de saida.");
    if(!(v->entrada = constroi_lista()))
        puts("Erro ao construir lista de entrada.");
    v->elems = 0;
    return v;
}

aresta constroi_aresta(void) {
    aresta a = malloc(sizeof(struct aresta));
    if(a == NULL) {
        perror("(constroi_aresta) Erro ao allocar memoria para aresta.");
        return NULL;
    }
    a->peso = PESO_DEFAULT;
    return a;
}

int destroi_aresta(void* param) {
    // Nao damos free em a->vs ou a->vc, porque sao os vertices. Eles estao sendo apontados por outro lugar.
    // Destroi_aresta soh destroi a propria aresta.
    aresta a = (aresta) param;
    if(a == NULL) return 1; // Jah foi destruido
    free(a);
    return 1;
}

int destroi_vertice(void* param) { // Soh funciona pra ser chamado dentro de destroi_grafo.
    vertice v = (vertice) param;
    if(v == NULL) {
        return 1; // Jah foi destruido
    }
    if(!destroi_lista(v->saida, destroi_aresta)) {
        perror("(destroi_vertice) Erro ao destruir lista.");
        return 0;
    }
    if(!destroi_lista(v->entrada, NULL)) {
        perror("(destroi_vertice) Erro ao destruir lista.");
        return 0;
    }
    free(v->i);
    free(v->j);
    free(v);
    return 1;
}

int destroi_grafo(void* param) {
    grafo g = (grafo) param;
    if(g == NULL)
        return 1;
    if(!destroi_lista(g->v, destroi_vertice)) {
        perror("(destroi_grafo) Erro ao destruir grafo.");
        return 0;
    }
    free(g);
    return 1;
}

vertice insere_vertice(grafo g, int cor, int id) {
    void* content = constroi_vertice();
    no novo = insere_lista(content, g->v);
    if(novo == NULL) {
        perror("(insere_vertice) Erro ao inserir vertice no grafo.");
        return NULL;
    }
    vertice v = conteudo(novo);
    v->cor = cor;
    v->id = id;
    ++(g->len);
    return v;
}

aresta insere_aresta(vertice saida, vertice chegada, long int peso) {
    aresta a = constroi_aresta();
    a->vs = saida;
    a->vc = chegada;
    a->peso = peso;
    if(!saida->saida) {
        puts("(insere_aresta) Problema na lista de saida.");
        return NULL;
    }
    if(!chegada->entrada) {
        puts("(insere_aresta) Problema na lista de saida.");
        return NULL;
    }
    // Insere a aresta na lista de arestas do vertice de saida
    if(insere_lista((void*)a, saida->saida) == NULL) {
        perror("(insere_aresta) Erro ao inserir aresta no vertice.");
        return NULL;
    }
    // Insere a aresta na lista de arestas do vertice de chegada
    if(insere_lista((void*)a, chegada->entrada) == NULL) {
        perror("(insere_aresta) Erro ao inserir aresta no vertice.");
        return NULL;
    }
    return a;
}

int menor_aresta(aresta *ar, int a, int b) {
    int i, menor = a;
    for(i=a+1; i<b; ++i) {
        if(ar[i]->vs->id < ar[menor]->vs->id) {
            menor = i;
        }
    }
    return menor;
}

grafo escreve_grafo(FILE *output, grafo g) {
    if(g == NULL) {
        puts("Grafo nulo. Abortando...");
        return NULL;
    }
    puts("Escrevendo grafo: {");

    no elem, childElem;
    vertice v;
    aresta a;

    // Imprime todos os vertices
    for(elem = primeiro_no(g->v); elem; elem = proximo_no(elem)) {
        v = (vertice) conteudo(elem);
        fprintf(output,"   \"%d\", cor = %d, elems = %d\n", v->id, v->cor, v->elems);
    }

    fprintf(output,"\n");

    // Imprime todas as arestas ordenadamente
    aresta *ar;
    ar = (aresta *) malloc(100 * sizeof(aresta));
    int i=0, len, aux;
    for(elem = primeiro_no(g->v); elem; elem = proximo_no(elem)) {
        v = (vertice) conteudo(elem);
        for(childElem = primeiro_no(v->entrada); childElem; childElem = proximo_no(childElem)) {
            a = (aresta) conteudo(childElem);
            ar[i] = a;
            ++i;
        }
    }
    len = i;
    for(i=0; i<len; ++i) {
        aux = menor_aresta(ar, i, len);
        a = ar[i];
        ar[i] = ar[aux];
        ar[aux] = a;
        fprintf(output,"   \"%d\" -- \"%d\"\n", ar[i]->vs->id, ar[i]->vc->id);
    }
    free(ar);

    fprintf(output,"}\n");

    return g;
}

int comp_avaliador(const void * elem1, const void * elem2) {
    avaliador f = *((avaliador*)elem1);
    avaliador s = *((avaliador*)elem2);
    if(f.n_ext > s.n_ext) return -1;
    if(f.n_ext < s.n_ext) return  1;
    // Se cheguei aqui, n_ext eh igual.
    if(f.n_int > s.n_int) return -1;
    if(f.n_int < s.n_int) return  1;
    return 0;
}

int comp(const void * elem1, const void * elem2) {
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return -1;
    if (f < s) return  1;
    return 0;
}

int maior_restante(int *restantes, int tam) {
    int i, maior = 0;
    for(i=0; i<tam; ++i) {
        if(restantes[i] > restantes[maior])
            maior = i;
    }
    return maior;
}
