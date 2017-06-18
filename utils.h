#ifndef _UTILS_H
#define _UTILS_H

#define PESO_DEFAULT 0

int VerticeID, TamMatriz, AndaSudeste;

int Count, Nblocos;

typedef struct bloco bloco;
typedef struct avaliador avaliador;
typedef struct celula *celula;
typedef struct no *no;
typedef struct lista *lista;
typedef struct grafo *grafo;
typedef struct vertice *vertice;
typedef struct aresta *aresta;

struct avaliador {
    int cor, n_int, n_ext;
};

struct celula {
    int cor, counted, status, counted2, pintado;
    vertice v;
};

struct no {
    void *conteudo;
    no proximo;
};

struct lista {
    unsigned int tamanho;
    int padding;
    no primeiro;
};

struct grafo {
    int len;
    char *nome;
    lista v;
    vertice lider;
};

struct vertice {
    int *i, *j, cor, id, elems, d;
    lista saida, entrada;
};

struct aresta {
    vertice vs, vc;
    long int peso;
};

unsigned int tamanho_lista(lista l);
no primeiro_no(lista l);
no proximo_no(no n);
void *conteudo(no n);
no insere_lista(void *conteudo, lista l);
lista constroi_lista(void);
int destroi_lista(lista l, int destroi(void *));
int remove_no(struct lista *l, struct no *rno, int destroi(void *));

char *nome_grafo(grafo g);
int direcionado(grafo g);
int ponderado(grafo g);
unsigned int n_vertices(grafo g);
unsigned int n_arestas(grafo g);

char *nome_vertice(vertice v);
grafo le_grafo(FILE *input);  
int destroi_grafo(void *g);
grafo escreve_grafo(FILE *output, grafo g);
grafo copia_grafo(grafo g);
lista vizinhanca(vertice v, int direcao, grafo g);
unsigned int grau(vertice v, int direcao, grafo g);

int comp (const void * elem1, const void * elem2);
int comp_avaliador(const void * elem1, const void * elem2);

grafo constroi_grafo(void);
vertice constroi_vertice(void);
aresta constroi_aresta(void);
void remove_duplicada(void* a, vertice v);
vertice insere_vertice(grafo g, int cor, int id);
aresta insere_aresta(vertice saida, vertice chegada, long int peso);
aresta copia_aresta(aresta a, grafo g);
vertice copia_vertice(vertice v);
grafo copia_subgrafo(grafo g, lista excecoes);
int destroi_aresta(void *v);
int destroi_vertice(void *v);
int destroi_grafo(void* param);
vertice procura_vertice(grafo g, char* nome);
int na_lista(lista l, void* conteudo);
lista vertices_grafo(grafo g);
void imprime_vertice(void* param);
void imprime_lista_vertices(lista l);
void imprime_lista_arestas(lista l);

int maior_restante(int *restantes, int tam);

#endif
