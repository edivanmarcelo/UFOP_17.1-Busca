/*
 * reg.c
 *
 *  Created on: 6 de out de 2017
 *      Author: higor
 */
#include <stdlib.h>
#include "Indice.h"
#include <string.h>
#define ITENSPAGINA 100
#define MAXTABELA 20000

// defini��o de uma entrada da tabela de �ndice das p�ginas
typedef struct indice {
	int posicao;
	int chave;
} Indice;
// defini��o de um reg do arquivo de dados

int pesquisa_asi (Indice *tab, int tam, Reg* reg, FILE *arq)
{
	Reg *pagina = malloc (sizeof(Reg) * (ITENSPAGINA));
	int i, quantitens;
	long desloc;

	// procura pela p�gina onde o reg pode se encontrar
	i = 0;

	while (i < tam && tab[i].chave <= (*reg).chave) i++;
	// caso a chave desejada seja menor que a 1a chave, o reg
	// n�o existe no arquivo
		if (i == 0) return 0;
		else {
			// a ultima p�gina pode n�o estar completa
			if (i < tam) quantitens = ITENSPAGINA;
			else {
				fseek (arq, 0, SEEK_END);
				quantitens = (ftell(arq)/sizeof(Reg))%ITENSPAGINA;
			}
			// l� a p�gina desejada do arquivo
			desloc = (tab[i-1].posicao-1)*ITENSPAGINA*sizeof(Reg);
			fseek (arq, desloc, SEEK_SET);
			//ler_arq(pagina, quantitens, arq);
			fread(pagina,quantitens * sizeof(Reg),1,arq);

			// pesquisa sequencial na p�gina lida
			for (i=0; i < quantitens; i++)
				if (pagina[i].chave == (*reg).chave) {
                    (*reg).chave = pagina[i].chave;
                    (*reg).dado1 = pagina[i].dado1;
					strcpy((*reg).dado2,pagina[i].dado2);
					return 1;
				}
			return 0;
		}
}

Reg* GeraTabela(int chave)
{
	Indice tabela[MAXTABELA];
	FILE *arq;
	Reg *x = malloc(sizeof(Reg));
	int pos;

	// abre o arquivo de dados
	if ((arq = fopen("registro.bin","rb")) == NULL) {
		printf("Erro na abertura do arquivo\n");
		return NULL;
	}

	// gera a tabela de �ndice das p�ginas
	pos = 0;
	while (fread(x, sizeof(Reg),1,arq) == 1) {
		tabela[pos].chave = x->chave;
		tabela[pos].posicao = pos+1;
		pos++;
		fseek(arq,((ITENSPAGINA - 1) * sizeof(Reg)),SEEK_CUR);
	};

	x->chave = chave;

	// ativa a fun��o de pesquisa
	if (pesquisa_asi(tabela, pos, x, arq))
	{
		return x;
	}

	fclose (arq);
	return NULL;
}


