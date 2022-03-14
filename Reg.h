/*
 * item.h
 *
 *  Created on: 7 de out de 2017
 *      Author: higor
 */

#ifndef REG_H
#define REG_H
#include <stdio.h>
#include <time.h>

typedef struct Reg{
    long int chave;
    long int dado1;
    char dado2[1000];
}Reg;

clock_t tempo; //Tempo do programa
long int comp;      //Num. de comparacoes
long int leit;     //Num. de leituras do arquivo
long int leitAux;
#endif /* ITEM_H_ */
