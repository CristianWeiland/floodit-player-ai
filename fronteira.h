#ifndef _FRONTEIRA_H
#define _FRONTEIRA_H

void prepara_fronteiras(tmapa *m);
int guloso_fronteira_externa(tmapa *m);
void define_fronteira_vizinhos(tmapa *m, int i, int j);
void define_front_int_ext(tmapa *m);
int tipo_fronteira(tmapa *m, int i, int j, int cor);
int checa_condicoes(tmapa *m, int i, int j, int cor);

#endif
