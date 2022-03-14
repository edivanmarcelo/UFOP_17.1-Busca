/*
 * arvB.c
 *
 * Created on: 4 de out de 2017
 *     Author: Marcelo
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"arvB.h"
#include"Reg.h"

#define TRUE  1
#define FALSE 0

typedef struct TipoPg * TipoAp;

typedef struct TipoPg{
    short  n; //Numero de itens da pagina
    Reg  r[MM]; //Vetor de registros
    TipoAp p[MM+1]; //Vetor de apontadores
} TipoPg;

TipoAp InicializaAB(TipoAp Arvore){ //Inicializa a arvore com os itens do arquivo
    tempo = clock(); //Variavel para calcular o tempo
    comp = 0; //Variavel para contar as comparacoes
    leit = 0; //Variavel para contar as leituras no arquivo

    Arvore = NULL;
    FILE *ler;
    Reg aux;

    comp++;
    if((ler = fopen("registro.bin", "rb")) == NULL){
        return NULL;
    }

    comp++;
    while((fread(&aux, sizeof(Reg), 1, ler)) != NULL){
        leit++;

        //printf("Chave:  %d\n", aux.chave);
        //printf("Dado 1: %d\n", aux.dado1);
        //printf("Dado 2: %s\n\n", aux.dado2);

        InsereAB(aux, &Arvore);
        comp++;
    }

    //ImprimeAB(Arvore);
    fclose(ler);

    tempo = clock() - tempo;
    tempo = tempo/(CLOCKS_PER_SEC/1000);
    printf("Tempo para Montar a Arvore B: %lu\n", tempo);
    printf("Comparacoes realizadas: %d\n", comp);
    printf("Leituras feitas no arquivo: %d\n\n", leit);


    return Arvore;
}

int PesquisaAB(Reg *item, TipoAp Ap){ //Busca o item na arvore
    long i=1;

    comp++;
    if(Ap == NULL){
        return 0; //Retorna 0 se o item nao foi encontrado
    }

    comp++;
    while(i < Ap->n && item->chave > Ap->r[i-1].chave){
        i++;
        comp++;
    }

    comp++;
    if(item->chave == Ap->r[i-1].chave){
        *item = Ap->r[i-1];
        return 1; // Retorna 1 se o item foi encontrado
    }

    comp++;
    if(item->chave < Ap->r[i-1].chave)
        return PesquisaAB(item, Ap->p[i-1]);
    else
        return PesquisaAB(item, Ap->p[i]);
}

void InsereNaPaginaAB(TipoAp Ap, Reg item, TipoAp ApDir){ //Insere o item na pagina correta
    short NaoAchouPos;
    int k;
    k = Ap->n;
    NaoAchouPos = (k>0);

    comp++;
    while(NaoAchouPos){
        comp++;
        if(item.chave >= Ap->r[k-1].chave){
            NaoAchouPos = FALSE;
        }
        else{
            Ap->r[k]   = Ap->r[k-1];
            Ap->p[k+1] = Ap->p[k];
            k--;
            if(k < 1)
                NaoAchouPos = FALSE;
        }
        comp++;
    }
    Ap->r[k] = item;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

int InsAB(Reg item, TipoAp Ap, short *cresceu, Reg *itemRet, TipoAp *ApRet){
    long i=1;
    long j;
    TipoAp ApTemp;

    comp++;
    if(Ap == NULL){
        *cresceu = TRUE;
        *itemRet = item;
        *ApRet   = NULL;
        return 1;
    }

    comp++;
    while(i < Ap->n && item.chave > Ap->r[i-1].chave){
        i++;
        comp++;
    }

    comp++;
    if(item.chave == Ap->r[i-1].chave){
        //ERRO: Item ja esta presente
        *cresceu = FALSE;
        return 0;
    }

    comp++;
    if(item.chave < Ap->r[i-1].chave)
        i--;

    InsAB(item, Ap->p[i], cresceu, itemRet, ApRet);

    comp++;
    if(!*cresceu)
        return 0;

    // Pagina tem espaco
    comp++;
    if(Ap->n < MM){
        InsereNaPaginaAB(Ap, *itemRet, *ApRet);
        *cresceu = FALSE;
        return 1;
    }

    //Overflow: Pagina tem que ser dividida
    ApTemp = (TipoAp)malloc(sizeof(TipoPg));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    comp++;
    if(i < M+1){
        InsereNaPaginaAB(ApTemp, Ap->r[MM-1], Ap->p[MM]);
        Ap->n--;
        InsereNaPaginaAB(Ap, *itemRet, *ApRet);
    }
    else
        InsereNaPaginaAB(ApTemp, *itemRet, *ApRet);

    comp++;
    for(j=M+2; j<=MM; j++){
        InsereNaPaginaAB(ApTemp, Ap->r[j-1], Ap->p[j]);
        comp++;
    }

    Ap->n        = M;
    ApTemp->p[0] = Ap->p[M+1];
    *itemRet     = Ap->r[M];
    *ApRet       = ApTemp;

    return 1;
}

int InsereAB(Reg item, TipoAp *Ap){ // Insere o item na arvore
    int aux;
    short cresceu;
    Reg itemRet;
    TipoPg *ApRet, *ApTemp;

    aux = InsAB(item, *Ap, &cresceu, &itemRet, &ApRet);
    comp++;
    if(aux == 0)
        return 0;

    comp++;
    if(cresceu){ // Arvore cresce na altura pela Raiz
        ApTemp = (TipoPg *)malloc(sizeof(TipoPg));
        ApTemp->n = 1;
        ApTemp->r[0] = itemRet;
        ApTemp->p[1] = ApRet;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
    }
    return 1;
}

void ImprimeAB(TipoAp arvore){ //Imprime a arvore criada
    int i = 0;

    if(arvore == NULL)
        return;

    while(i <= arvore->n){
        ImprimeAB(arvore->p[i]);
        if(i != arvore->n){
            printf("Chave:  %d\n", arvore->r[i].chave);
            printf("Dado 1: %d\n", arvore->r[i].dado1);
            printf("Dado 2: %s\n\n", arvore->r[i].dado2);
        }
        i++;
    }
}

void DeletarAB(TipoAp *arvore){
    int i =0;

    if((*arvore) = NULL)
        return;

    while(i <= (*arvore)->n){
        DeletarAB(&((*arvore)->p[i]));
        i++;
    }
    DeletarAB(&((*arvore)->p[i]));
    free((*arvore));
    *arvore = NULL;
}
