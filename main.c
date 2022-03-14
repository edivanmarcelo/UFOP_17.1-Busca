/*
 * main.c
 *
 *  Created on: 6 de out de 2017
 *      Author: higor
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Indice.h"
#include "Arq.h"
#include "ArvoreBEstrela.h"

#include "arvB.h"
#include "arvB.h"
#include "Reg.h"

int main(int argc, char *argv[])
{
	if(argc == 5 || argc == 6)
	{
		int metodo = atoi(argv[1]);
		int quantidade = atoi(argv[2]);
		int situacao = atoi(argv[3]);
		int chave = atoi(argv[4]);

		gerar_arq(situacao, quantidade);

		switch(metodo)
		{
			case 1:
			{
				Reg *x;
                x = GeraTabela(chave);

                if(x != NULL) {
                    printf ("O registro %i foi localizado\n\n", chave);
                    if (argc == 6) {
                        if (x != NULL)
                        {
                            if (strcmp(argv[5], "-P") == 0 || strcmp(argv[5], "-p") == 0) {
                                printf("Registro: %d\nDado1: %li\nDado2: %s\n", x->chave, x->dado1, x->dado2);
                            }
                        }
                    }
                } else printf ("O registro %i nao foi localizado", chave);

				break;
			}
			case 2:
            {
                Reg x;
                x.chave = chave;
                CriarArvBin();

                comp = 0; //Comparacoes
                leit = 0; //Leituras
                leitAux = 0; //Leituras Em Arquivo segundario;
                tempo = clock(); //Tempo

                if(PesquisaBin(&x)){
                    tempo = clock() - tempo;
                    tempo = tempo/(CLOCKS_PER_SEC/1000);
                    printf("Registro Encontrado\n\n");
                    if(argc == 6)
                        if(&x != NULL)
                            if(strcmp(argv[5], "-P") == 0 || strcmp(argv[5], "-p") == 0)
                                printf("Registro: %d\nDado1:    %d\nDado2:    %s\n" , x.chave, x.dado1, x.dado2);
                }
                else{
                    tempo = clock() - tempo;
                    tempo = tempo/(CLOCKS_PER_SEC/1000);
                    printf("Registro nao encontrado\n");
                }

                printf("\nTempo da pesquisa em Arvore Binaria Externa: %lu\n", tempo);
                printf("Comparacoes realizadas: %d\n", comp);
                printf("Leituras feitas no arquivo \"registro.bin\": %d\n", leit);
                printf("Leituras feitas no arquivo \"ArvBin.bin\": %d\n\n", leitAux);

                break;
            }
			case 3:
            {
                TipoAp avB;
                avB = InicializaAB(avB);
                Reg x;
                x.chave = chave;

                comp = 0; //Comparacoes
                leit = 0; //Leituras
                tempo = clock(); //Tempo

                if(PesquisaAB(&x, avB)){
                    tempo = clock() - tempo;
                    tempo = tempo/(CLOCKS_PER_SEC/1000);
                    printf("Registro Encontrado\n\n");
                    if(argc == 6)
                        if(&x != NULL)
                            if(strcmp(argv[5], "-P") == 0 || strcmp(argv[5], "-p") == 0)
                                printf("Registro: %d\nDado1:    %d\nDado2:    %s\n" , x.chave, x.dado1, x.dado2);
                }
                else{
                    tempo = clock() - tempo;
                    tempo = tempo/(CLOCKS_PER_SEC/1000);
                    printf("Registro nao encontrado\n");
                }

                printf("\nTempo da pesquisa em Arvore B: %lu\n", tempo);
                printf("Comparacoes realizadas: %d\n", comp);
                printf("Leituras feitas no arquivo: %d\n\n", leit);

                //DeletarAB(&avB);
                break;
            }
			case 4:
            {
                Apontador Ap;
                Ap = inicializa(Ap);
                Reg x;
				x.chave = chave;

                if (Pesquisa(&Ap,&x))
                {
                    printf("Registro Encontrado\n\n");
                    if(argc == 6)
                    {
                        if(&x != NULL)
                            if(strcmp(argv[5], "-P") == 0 || strcmp(argv[5], "-p") == 0)
                            {
                                printf("Registro: %d\nDado1: %li\nDado2: %s\n", x.chave, x.dado1, x.dado2);
                            }
                    }
                } else printf("Registro nao encontrado");

                break;
            }
        }
	}

	else
	{
		printf("pesquisa <metodo> <quantidade> <situacao> <chave>");
	}
}


