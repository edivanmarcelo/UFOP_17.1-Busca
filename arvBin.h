/*
 * arvBin.h
 *
 * Created on: 21 de out de 2017
 *     Author: Marcelo
 */
#ifndef arvBin_h
#define arvBin_h

#include<stdio.h>
#include"Reg.h"

typedef struct RegBin RegBin;


void CriarArvBin(void);
int  InsereBin(RegBin item, int pos);
int  InsereArqBin(FILE *arq, RegBin item, int pos, int *n);

int PesquisaBin(Reg *item);
int PesquisaArqBin(FILE *arq, Reg *item, int *n);

void ImprimeArqBin(void);


#endif
