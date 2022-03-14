/*
 * arvB.h
 *
 * Created on: 4 de out de 2017
 *     Author: Marcelo
 */

#ifndef arvB_h
#define arvB_h

#include<stdio.h>
#include"Reg.h"

#define M  2
#define MM 2*M

typedef struct TipoPg * TipoAp;
typedef struct TipoPg;

TipoAp InicializaAB(TipoAp Arvore);
int PesquisaAB(Reg *item, TipoAp Ap);
void ImprimeAB(TipoAp arvore);

void InsereNaPaginaAB(TipoAp Ap, Reg item, TipoAp ApDir);
int InsAB(Reg item, TipoAp Ap, short *cresceu, Reg *itemRet, TipoAp *ApRet);
int InsereAB(Reg item, TipoAp *Ap);

void DeletarAB(TipoAp *arvore);

#endif
