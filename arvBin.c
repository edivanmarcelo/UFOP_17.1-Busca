/*
 * arvBin.c
 *
 * Created on: 21 de out de 2017
 *     Author: Marcelo
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"Reg.h"
#include"arvBin.h"

#define Iconst 1000
#define Lconst 1000

typedef struct RegBin{
    int chave;
    long int dado1;
    char dado2[1000];

    int  dir;
    int  esq;
}RegBin;

void CriarArvBin(void){
    comp = 0; //Comparacoes
    leit = 0; //Leituras
    tempo = clock(); //Tempo

    FILE *ler;
    FILE *escrever;
    int pos  = 0;
    int erro = 1;

    comp++;
    if((ler = fopen("registro.bin", "rb")) == NULL)
        return;

    comp++;
    if((escrever = fopen("ArvBin.bin", "wb")) == NULL)
        return;
    fclose(escrever);

    Reg    leitura;
    RegBin escrita;

    comp++;
    while((fread(&leitura, sizeof(Reg), 1, ler)) != NULL){
        leit++;

        escrita.chave = leitura.chave;
        escrita.dado1 = leitura.dado1;
        strcpy(escrita.dado2, leitura.dado2);
        escrita.dir = -1;
        escrita.esq = -1;

        escrever = fopen("ArvBin.bin", "r+b");
        fseek(escrever, 0, 2);
        fwrite(&escrita, sizeof(RegBin), 1, escrever);
        fclose(escrever);

        erro = InsereBin(escrita, pos);
        pos++;
        comp++;
    }
    fclose(ler);

    comp++;
    if(erro == -1)
        printf("\nERRO\n\n");

    tempo = clock() - tempo;
    tempo = tempo/(CLOCKS_PER_SEC/1000);
    printf("Tempo de Criacao: %lu\n", tempo);
    printf("Comparacoes realizadas: %d\n", comp);
    printf("Leituras feitas no arquivo \"registro.bin\": %d\n", leit);
    printf("Leituras feitas no arquivo \"ArvBin.bin\": %d\n\n", leitAux);

    //ImprimeArqBin();
}

int InsereBin(RegBin item, int pos){
    FILE *arq;
    int ret = 0;

    comp++;
    if((arq = fopen("ArvBin.bin", "r+b")) == NULL)
        return -1;
    fclose(arq);

    int n = 0;
    int cont = 0;

    while(ret == 0){
        comp++;
        arq = fopen("ArvBin.bin", "r+b");
        fseek(arq, ((cont*(Iconst-1))*sizeof(RegBin)), 0);

        ret = InsereArqBin(arq, item, pos, &n);
        fclose(arq);

        cont ++;
        n = 0;
    }
    return ret;
}
int InsereArqBin(FILE *arq, RegBin item, int pos, int *n){
    (*n)++;
    comp++;
    if((*n)%Iconst == 0)
        return 0;
    else{
        RegBin aux;
        fread(&aux, sizeof(RegBin), 1, arq);
        leitAux++;

        comp++;
        if(aux.chave == item.chave)
            return 1;

        else if(item.chave < aux.chave){
            comp = comp+2;
            if(aux.esq == -1){
                aux.esq = pos;
                fseek(arq, (-sizeof(RegBin)), 1);
                fwrite(&aux, sizeof(RegBin), 1, arq);
                return 1;
            }
            else{
                fseek(arq, aux.esq*sizeof(RegBin), 0);
                return InsereArqBin(arq, item, pos, n);
            }
        }
        else if(item.chave > aux.chave){
            comp = comp+2;
            if(aux.dir == -1){
                aux.dir = pos;
                fseek(arq, (-sizeof(RegBin)), 1);
                fwrite(&aux, sizeof(RegBin), 1, arq);
                return 1;
            }
            else{
                fseek(arq, aux.dir*sizeof(RegBin), 0);
                return InsereArqBin(arq, item, pos, n);
            }
        }
    }
}

int PesquisaBin(Reg *item){
    FILE *arq;
    int ret = -1;

    comp++;
    if((arq = fopen("ArvBin.bin", "rb")) == NULL)
        return -1;
    fclose(arq);

    int n=0;
    int cont=0;

    while(ret == -1){
        comp++;
        arq = fopen("ArvBin.bin", "rb");
        fseek(arq, ((cont*(Lconst-1))*sizeof(RegBin)), 0);

        ret = PesquisaArqBin(arq, item, &n);
        fclose(arq);

        cont ++;
        n = 0;
    }
    return ret;
}

int PesquisaArqBin(FILE *arq, Reg *item, int *n){
    (*n)++;
    comp++;
    if((*n)%Lconst == 0)
        return -1;
    else{
        RegBin aux;
        fread(&aux, sizeof(RegBin), 1, arq);
        leitAux++;

        comp++;
        if(aux.chave == item->chave){
            item->dado1 = aux.dado1;
            strcpy(item->dado2, aux.dado2);
            return 1;
        }
        else if(item->chave < aux.chave){
            comp = comp+2;
            if(aux.esq == -1)
                return 0;
            else{
                fseek(arq, aux.esq*sizeof(RegBin), 0);
                return PesquisaArqBin(arq, item, n);
            }
        }
        else{
            comp++;
            if(aux.dir == -1)
                return 0;
            else{
                fseek(arq, aux.dir*sizeof(RegBin), 0);
                return PesquisaArqBin(arq, item, n);
            }
        }
    }
}

void ImprimeArqBin(void){
    FILE *arq;
    RegBin aux;

    if((arq = fopen("ArvBin.bin", "rb")) == NULL)
        printf("ERRO ao abrir arquivo\n");

    while((fread(&aux, sizeof(RegBin), 1, arq)) != NULL){

        printf("Chave:  %d\n", aux.chave);
        printf("DADO 1: %d\n", aux.dado1);
        printf("DADO 2: %s\n", aux.dado2);
        printf("ESQ:    %d\n", aux.esq);
        printf("DIR:    %d\n\n", aux.dir);
    }

    fclose(arq);
}
