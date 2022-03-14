/*
 * arq.c
 *
 *  Created on: 7 de out de 2017
 *      Author: higor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Reg.h"
#include "Arq.h"

void gerar_arq(int situacao, unsigned long int qt){
	switch(situacao)
	{
		case 1:
			ascendentemente(qt);
			break;
		case 2:
			descendentemente(qt);
			break;
		case 3:
			aleatorio(qt);
			break;
	}
}

//FUN��O QUE GERA UM ARQUIVO COM A CHAVE GERADA ASCENDENTEMENTE
void ascendentemente(unsigned long int quant)
{
	FILE *escrita;
	Reg reg;
	char dado2[1000] = "ascendentemente";

	if((escrita = fopen("registro.bin", "wb")) == NULL)
	{
		printf("Arquivo nao pode ser aberto");
		exit(1);
	}

	for(long int i = 0; i < quant; i++)
	{
		reg.chave = i;
		reg.dado1 = i;
		strcpy(reg.dado2, dado2);
		fwrite(&reg, sizeof(Reg), 1, escrita);
	}

	fclose(escrita);
}

//FUN��O QUE GERA UM ARQUIVO COM A CHAVE GERADA DESCENDENTEMENTE
void descendentemente(unsigned long int quant)
{
	FILE *escrita;
	Reg reg;
	char dado2[1000] = "descendentemente";

	if((escrita = fopen("registro.bin", "wb")) == NULL)
	{
		printf("Arquivo nao pode ser aberto");
		exit(1);
	}

	for(unsigned long int i = quant ; i > 0; i--)
	{
		reg.chave = i-1;
		reg.dado1 = quant - i;
		strcpy(reg.dado2, dado2);
		fwrite(&reg, sizeof(Reg), 1, escrita);
	}
    fclose(escrita);
}

//FUNÇÃO QUE GERA ARQUIVO COM CHAVE ALEATÓRIA
void aleatorio(unsigned long int quant)
{
	FILE *escrita;
	Reg reg;
	char dado2[1000] = "aleatorio";

	if((escrita = fopen("registro.bin", "wb")) == NULL)
	{
		printf("Arquivo nao pode ser aberto");
		exit(1);
	}
	fclose(escrita);

	srand(time(NULL));

	long int auxChave;
	for(long int i = 0; i < quant; i++)
	{
	    do{
            auxChave = rand() % quant;
	    }while(existe(auxChave) == 1);

	    //printf("Chave: %d\n", auxChave);

		reg.chave = auxChave;
		reg.dado1 = i;
		strcpy(reg.dado2, dado2);

		escrita = fopen("registro.bin", "r+b");
		fseek(escrita, 0, 2);
		fwrite(&reg, sizeof(Reg), 1, escrita);
		fclose(escrita);
	}
}

int existe(long int chave){
    FILE *arq;
    Reg aux;

    arq = fopen("registro.bin", "r+b");

    while((fread(&aux, sizeof(Reg), 1, arq)) != NULL){
        if(aux.chave == chave){
            fclose(arq);
            return 1;
        }
    }
    fclose(arq);
    return 0;
}

