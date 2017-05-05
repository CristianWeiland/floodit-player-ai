#ifndef _UTILS_H
#define _UTILS_H

typedef struct no *no;
typedef struct lista *lista;
typedef struct grafo *grafo;
typedef struct vertice *vertice;
typedef struct aresta *aresta;

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
	lista v;
	char* nome;
	int direcao;
	int ponderado;
};

struct vertice {
	char* nome;
	lista saida, entrada;
    int *rotulo;
    int estado, atributo;
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

*/
#endif
