#ifndef _UTILS_H
#define _UTILS_H

#define PESO_DEFAULT 0

int VerticeID, TamMatriz;

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
//    int cor, counted, status, counted2, bloco, peso;
    int cor, counted, status, counted2, bloco, peso; // Nao to alocando direito em algum lugar...
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

// Funcoes criadas por mim. Soh as que tem // no final eu coloquei a implementacao.
grafo constroi_grafo(void); //
vertice constroi_vertice(void); //
aresta constroi_aresta(void); //
void remove_duplicada(void* a, vertice v);
vertice insere_vertice(grafo g, int cor, int id);
aresta insere_aresta(vertice saida, vertice chegada, long int peso);
aresta copia_aresta(aresta a, grafo g);
vertice copia_vertice(vertice v);
grafo copia_subgrafo(grafo g, lista excecoes);
int destroi_aresta(void *v); //
int destroi_vertice(void *v); //
int destroi_grafo(void* param); //
vertice procura_vertice(grafo g, char* nome);
int na_lista(lista l, void* conteudo);
lista vertices_grafo(grafo g);
void imprime_vertice(void* param);
void imprime_lista_vertices(lista l);
void imprime_lista_arestas(lista l);

/*
//-----------------------------------------------------------------------------
// (apontador para) nó da lista encadeada cujo conteúdo é um void *

typedef struct no *no;

//-----------------------------------------------------------------------------
// (apontador para) lista encadeada

typedef struct lista *lista;

//---------------------------------------------------------------------------
// nó de lista encadeada cujo conteúdo é um void *

struct no {
  void *conteudo;
  no proximo;
};

//---------------------------------------------------------------------------
// lista encadeada

struct lista {
  unsigned int tamanho;
  int padding; // só pra evitar warning 
  no primeiro;
};

//------------------------------------------------------------------------------
// devolve o número de nós da lista l

unsigned int tamanho_lista(lista l);

//------------------------------------------------------------------------------
// devolve o primeiro nó da lista l,
//      ou NULL, se l é vazia

no primeiro_no(lista l);

//------------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da lista

no proximo_no(no n);

//------------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo(no n);
//------------------------------------------------------------------------------
// insere um novo nó na lista l cujo conteúdo é p
//
// devolve o no recém-criado 
//      ou NULL em caso de falha

no insere_lista(void *conteudo, lista l);
//------------------------------------------------------------------------------
// cria uma lista vazia e a devolve
//
// devolve NULL em caso de falha

lista constroi_lista(void);
//------------------------------------------------------------------------------
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

int destroi_lista(lista l, int destroi(void *));

//------------------------------------------------------------------------------
// remove o no de endereço rno de l
// se destroi != NULL, executa destroi(conteudo(rno)) 
// devolve 1, em caso de sucesso
//         0, se rno não for um no de l

int remove_no(struct lista *l, struct no *rno, int destroi(void *));

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo pode ser
// - direcionado ou não
// - com pesos nas arestas ou não
// 
// além dos vértices e arestas, o grafo tem um nome, que é uma "string"
// 
// num grafo com pesos nas arestas todas as arestas tem peso, que é um long int
// 
// o peso default de uma aresta é 0

typedef struct grafo *grafo;

//------------------------------------------------------------------------------
// devolve o nome do grafo g

char *nome_grafo(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g é direcionado, ou
//         0, caso contrário

int direcionado(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g tem pesos nas arestas/arcos,
//      ou 0, caso contrário

int ponderado(grafo g);

//------------------------------------------------------------------------------
// devolve o número de vértices do grafo g

unsigned int n_vertices(grafo g);

//------------------------------------------------------------------------------
// devolve o número de arestas/arcos do grafo g

unsigned int n_arestas(grafo g);

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados que representa um vértice do grafo
// 
// cada vértice tem um nome que é uma "string"

typedef struct vertice *vertice;

//------------------------------------------------------------------------------
// devolve o nome do vertice v

char *nome_vertice(vertice v);

//------------------------------------------------------------------------------
// lê um grafo no formato dot de input, usando as rotinas de libcgraph
// 
// desconsidera todos os atributos do grafo lido exceto o atributo
// "peso" quando ocorrer; neste caso o valor do atributo é o peso da
// aresta/arco que é um long int
// 
// num grafo com pesos todas as arestas/arcos tem peso
// 
// o peso default de uma aresta num grafo com pesos é 0
// 
// todas as estruturas de dados alocadas pela libcgraph são
// desalocadas ao final da execução
// 
// devolve o grafo lido ou
//         NULL em caso de erro 

grafo le_grafo(FILE *input);  

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso ou
//         0 caso contrário
// 
// g é um (void *) para que destroi_grafo() possa ser usada como argumento de
// destroi_lista()

int destroi_grafo(void *g);

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot, de forma que
// 
// 1. todos os vértices são escritos antes de todas as arestas/arcos 
// 
// 2. se uma aresta tem peso, este deve ser escrito como um atributo 
//    de nome "peso"
//
// devolve o grafo escrito ou
//         NULL em caso de erro 

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// devolve um grafo igual a g

grafo copia_grafo(grafo g);

//------------------------------------------------------------------------------
// devolve a vizinhança do vértice v no grafo g
// 
// se direcao == 0, v é um vértice de um grafo não direcionado
//                  e a função devolve sua vizinhanca 
//
// se direcao == -1, v é um vértice de um grafo direcionado e a função
//                   devolve sua vizinhanca de entrada
//
// se direcao == 1, v é um vértice de um grafo direcionado e a função
//                  devolve sua vizinhanca de saída

lista vizinhanca(vertice v, int direcao, grafo g);

//------------------------------------------------------------------------------
// devolve o grau do vértice v no grafo g
// 
// se direcao == 0, v é um vértice de um grafo não direcionado
//                  e a função devolve seu grau
//
// se direcao == -1, v é um vértice de um grafo direcionado
//                   e a função devolve seu grau de entrada
//
// se direcao == 1, v é um vértice de um grafo direcionado
//                  e a função devolve seu grau de saída

unsigned int grau(vertice v, int direcao, grafo g);

//------------------------------------------------------------------------------
// Estruturas Auxiliares Criadas:

// Lista v = lista de vertices do grafo.
// Nome = nome do grafo.
// Int direcao = 1 se o grafo for direcionado, 0 caso contrario.
// Int ponderado = 1 se o grafo possui peso nas arestas, 0 caso contrario.
struct grafo {
    lista v;
    char* nome;
    int direcao;
    int ponderado;
};

//------------------------------------------------------------------------------
// Estrutura de dados que representa um vértice do grafo.
// Cada vértice tem um nome, que é uma "string"
// Nome = nome do vertice
// Lista saida e entrada sao listas para arestas de saida e de entrada.
// Em grafos nao direcionados, as arestas soh sao inseridas uma vez em cada vertice!
// Por exemplo, uma aresta a--b aparece na lista de saida do vertice a e na lista de
// entrada do vertice b..
struct vertice {
    char* nome;
    lista saida, entrada;
    int *rotulo;
    int estado, atributo;
};

//------------------------------------------------------------------------------
// Cada aresta aparecera tanto no vertice de entrada como no de saida.
// Por exemplo, caso existam dois vertices a e b e um grafo nao direcionado
// que possua a aresta a--b, tanto o vertice a quanto o vertice b apontarao
// para a aresta a--b. O mesmo vale para grafos direcionados.
// Vertice vs = vertice de saida.
// Vertice vc = vertice de chegada.
// Peso = peso da aresta (0 é o default).
typedef struct aresta *aresta;
struct aresta {
    vertice vs, vc;
    long int peso;
};

//------------------------------------------------------------------------------
// a estrutura par servira para formar uma tabela de pares head e tail.
// Cada par tera como funcao indicar que o par jah foi inserido ou utilizado
// em alguma funcao, para tratar arestas duplicadas.
typedef struct par *par;
struct par {
    char *head;
    char *tail;
};
//------------------------------------------------------------------------------
// Alloca memoria para um grafo
grafo constroi_grafo(void);

//------------------------------------------------------------------------------
// Alloca memoria para um vertice
vertice constroi_vertice(void);

//------------------------------------------------------------------------------
// Alloca memoria para uma aresta
aresta constroi_aresta(void);

//------------------------------------------------------------------------------
// Dado um vertice, procura um apontador pra aresta a e remove ele
void remove_duplicada(void* a, vertice v);

//------------------------------------------------------------------------------
// Cria um vertice, atribui a ele um nome e insere na lista de vertices do grafo
vertice insere_vertice(grafo g, char* nome);

//------------------------------------------------------------------------------
// Cria uma aresta com atributos vs, vc e peso sendo, respectivamente,
// saida, chegada e peso, e insere nas 2 listas (do vertice de entrada e do de saida).
aresta insere_aresta(vertice saida, vertice chegada, long int peso);

//------------------------------------------------------------------------------
// Cria uma copia da aresta e retorna um apontador para a nova aresta
aresta copia_aresta(aresta a, grafo g);

//------------------------------------------------------------------------------
// Cria uma copia do vertice e retorna um apontador para o novo vertice
vertice copia_vertice(vertice v);

//------------------------------------------------------------------------------
// Faz uma copia de um grafo, mas nao insere elementos que aparecem na lista
// de vertices excecoes, tambem garantindo que as arestas que tinham uma ponta
// em algum desses vertices tambem nao sao inseridas.
grafo copia_subgrafo(grafo g, lista excecoes);

//------------------------------------------------------------------------------
// Desalloca memoria de um elemento par (argumento eh void* para a funcao poder ser
// usada como parametro da funcao destroi_lista)
int destroi_par(void* param);

//------------------------------------------------------------------------------
// Desalloca memoria de uma aresta (argumento eh void* para a funcao poder ser
// usada como parametro da funcao destroi_lista)
int destroi_aresta(void *v);

//------------------------------------------------------------------------------
// Desalloca memoria de um vertice (argumento eh void* para a funcao poder ser
// usada como parametro da funcao destroi_lista)
int destroi_vertice(void *v);

//------------------------------------------------------------------------------
// Procura na lista de vertices do grafo um vertice com o nome do parametro
// retorna NULL em caso de erro ou caso nao ache o vertice
// retorna o vertice caso ache ele
vertice procura_vertice(grafo g, char* nome);

//------------------------------------------------------------------------------
// Devolve 1, se o vertice v2 é adjacente (ligado por uma aresta) a v, ou
//         0, caso contrário
int adjacente(vertice v, vertice v2);

//------------------------------------------------------------------------------
// Devolve 1, se o vertice é uma clique em g, ou
//         0, caso contrário
//
// Um vertice V de um conjunto de vertices C de um grafo G é uma clique em G
// se todo vértice no conjunto C é vizinho de V em G
int vertice_clique(vertice v, lista l);

//------------------------------------------------------------------------------
// Verifica se um elemento 'Par' ja foi inserido na tabela. Caso ja tenha sido
// inserido, retorna 1. Caso nao tenha sido inserido, insere-o e retorna 0.
int inserido(char* n1, char* n2, lista tabela);

//------------------------------------------------------------------------------
// percorre a lista e retorna 1 se o parametro conteudo estiver dentro dessa
// lista, retorna 0 caso contrario.
int na_lista(lista l, void* conteudo);

//------------------------------------------------------------------------------
// Retorna a lista de vertices de um grafo g
lista vertices_grafo(grafo g);

//------------------------------------------------------------------------------
// Imprime o rotulo de um vertice v.
void imprime_rotulo(vertice v);

//------------------------------------------------------------------------------
// Imprime um vertice
void imprime_vertice(void* param);

//------------------------------------------------------------------------------
// Imprime uma lista de vertices.
void imprime_lista_vertices(lista l);

//------------------------------------------------------------------------------
// Imprime uma lista de arestas.
void imprime_lista_arestas(lista l);

*/
#endif
