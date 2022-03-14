//
// Created by hdo on 31/10/2017.
//

#ifndef ARVOREBESTRELA_H
#define ARVOREBESTRELA_H
#include "Reg.h"
#define M 2
#define MM M*2
typedef enum TipoIntExt TipoIntExt;
typedef struct TipoPagina TipoPagina;
typedef struct TipoPagina *Apontador;
int Pesquisa(Apontador*, Reg*);
Apontador inicializa(Apontador);
void Ins(Reg, Apontador*, short *, Reg *, Apontador *);
void Insere(Reg, Apontador *);
void InsereNaPagina(Apontador, int, Apontador);
#endif //TP_01_ED2_ARVOREBESTRELA_H
