#ifndef _FRONTEIRA_H
#define _FRONTEIRA_H

void prepara_fronteiras(tmapa *m);
void define_fronteira_vizinhos(tmapa *m, int i, int j);
void define_front_int_ext(tmapa *m);
int tipo_fronteira(tmapa *m, int i, int j, int cor);
int checa_condicoes(tmapa *m, int i, int j, int cor);
int guloso_fronteira_externa_com_otima(tmapa *m);
avaliador look_ahead(tmapa *m, avaliador *cores, int cor);
void calcula_fronteira_com_fakes(tmapa *m);
void define_fronteira_vizinhos_fake(tmapa *m, int i, int j);
void define_front_int_ext_fake(tmapa *m);
int tipo_fronteira_fake(tmapa *m, int i, int j, int cor);
int checa_condicoes_fake(tmapa *m, int i, int j, int cor);

#endif
