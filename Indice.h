/*
 * reg.h
 *
 *  Created on: 6 de out de 2017
 *      Author: higor
 */

#ifndef INDICE_H
#define INDICE_H
#include "Reg.h"
#include <stdio.h>
typedef struct indice Indice;
Reg* GeraTabela(int);
int pesquisa_asi(Indice*, int, Reg*, FILE*);
#endif /* REG_H_ */
