//
// Created by hdo on 31/10/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include"ArvoreBEstrela.h"
#define TRUE 1
#define FALSE 0


typedef enum TipoIntExt
{
    Interna,
    Externa
}TipoIntExt;

typedef struct TipoPagina
{
    TipoIntExt Pt;  // Página Interna ou Externa
    union {
        struct {    //Página Interna
            int ni;                 // Quantidade de chaves armazenadas
            int ri[MM];             // Vetor de chaves
            Apontador pi[MM + 1];   // Vetor de apontadores
        } U0;
        struct {    //Página Externa
            int ne; //Quantidade de registros armazenados
            Reg re[MM]; //Vetor de registros
        } U1;
    } UU;
}TipoPagina;

int Pesquisa(Apontador *Ap, Reg* reg)
{
    int i;

    if ((*Ap)->Pt == Interna)   //SE O APONTADOR FOR UMA PÁGINA INTERNA REALIZADA CHAMADAS RECURSIVA ATÉ QUE ENCONTRE A PÁGINA EXTERNA
    {
        i = 1;
        while (i < (*Ap)->UU.U0.ni && reg->chave > (*Ap)->UU.U0.ri[i - 1]) i++;
        if (reg->chave < (*Ap)->UU.U0.ri[i - 1]) return Pesquisa(&(*Ap)->UU.U0.pi[i - 1], reg);
        else return Pesquisa(&(*Ap)->UU.U0.pi[i], reg);
    }

    i = 1;

    while (i < (*Ap)->UU.U1.ne && reg->chave > (*Ap)->UU.U1.re[i - 1].chave) i++;   //PESQUISA SEQUENCIAL NA PAGINA EXTERNA

    if (reg->chave == (*Ap)->UU.U1.re[i - 1].chave) // VERIFICA SE A CHAVE FOI ENCONTRADA
    {
        *reg = ((*Ap)->UU.U1.re[i - 1]);
        return TRUE;
    }
    return FALSE;
}
Apontador inicializa(Apontador Ap)  //INICIALIZA A ARVORE
{
    Ap = NULL;

    FILE *ler;

    if((ler = fopen("registro.bin", "rb")) == NULL)
    {
        return NULL;
    }

    Reg aux;

    while(!feof(ler))   //CONSTROÍ A ARVORE ADICIONANDO OS ELEMENTOS DO ARQUIVO
    {
        fread(&aux, sizeof(Reg),1, ler);
        Insere(aux, &Ap);
        //imprimi(Ap);
    }

    return Ap;
}

void Insere(Reg reg, Apontador *Ap)
{
    short Cresceu;
    Reg RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    Ins(reg, Ap, &Cresceu, &RegRetorno, &ApRetorno);

    if(ApRetorno == NULL)   //CRIA UMA PÁGINA EXTERNA
    {
    	ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
    	ApTemp->Pt = Externa;
    	ApTemp->UU.U1.ne = 1;
    	ApTemp->UU.U1.re[0] = RegRetorno;
    	*Ap = ApTemp;
    }

    else if (Cresceu)   //CRIA PÁGINA INTERNA
    {
        ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = RegRetorno.chave;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap;
        *Ap = ApTemp;
    }
}

void Ins(Reg reg, Apontador *Ap, short *Cresceu, Reg *RegRetorno, Apontador *ApRetorno)
{
    long i = 1, j;
    Apontador ApTemp;

    if((*Ap) == NULL)   //SE O APONTADOR FOR NULO A ARVORE PRECISA CRESCER
    {
        *Cresceu = TRUE;
        (*RegRetorno) = reg;
        (*ApRetorno) = NULL;
        return;
    }

    if((*Ap)->Pt == Interna)    //SE A PAGINA FOR INTERNA
    {
		while (i < (*Ap)->UU.U0.ni && reg.chave > (*Ap)->UU.U0.ri[i-1]) i++;

		if (reg.chave < (*Ap)->UU.U0.ri[i-1]) i--;
		Ins(reg, &(*Ap)->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);  //CHAMADA RECURSIVA ATÉ ENCONTRAR A PÁGINA EXTERNA
		if (*Cresceu)   // VERIFICA SE A ARVORE PRESCISA CRESCER
        {
            if ((*Ap)->UU.U0.ni < MM) { //SE TEM ESPAÇO NA PÁGINA
                InsereNaPagina(*Ap, (*RegRetorno).chave, *ApRetorno);
                *Cresceu = FALSE;
            } else {
                ApTemp = (Apontador) malloc(sizeof(TipoPagina));
                ApTemp->Pt = Interna;
                ApTemp->UU.U0.ni = 0;
                ApTemp->UU.U0.pi[0] = NULL;

                if (i < M + 1) {
                    InsereNaPagina(ApTemp, (*Ap)->UU.U0.ri[MM - 1], (*Ap)->UU.U0.pi[MM]);
                    (*Ap)->UU.U0.ni--;
                    InsereNaPagina(Ap, (*RegRetorno).chave, *ApRetorno);
                } else InsereNaPagina(ApTemp, (*RegRetorno).chave, *ApRetorno);

                for (j = M + 2; j <= MM; j++)
                    InsereNaPagina(ApTemp, (*Ap)->UU.U0.ri[j - 1], (*Ap)->UU.U0.pi[j]);

                (*Ap)->UU.U0.ni = M;
                ApTemp->UU.U0.pi[0] = (*Ap)->UU.U0.pi[M + 1];
                (*RegRetorno).chave = (*Ap)->UU.U0.ri[M];
                *ApRetorno = ApTemp;
            }
        }
    }
    else //SE A PAGINA FOR EXTERNA
    {
        if((*Ap)->UU.U1.ne == (MM)) //SE A PAGINA ESTIVER CHEIA PRECISA CRIAR UMA NOVA
        {
            ApTemp = (Apontador) malloc(sizeof(TipoPagina));
            ApTemp->Pt = Externa;
            ApTemp->UU.U1.ne = 0;


            //DIVISÃO DA PÁGINA ATUAL POR 2

            int tam = (*Ap)->UU.U1.ne;
            int meio = tam / 2;
            int i = 0;
            *RegRetorno = (*Ap)->UU.U1.re[meio];    //A CHAVE DO MEIO DA PAGINA EXTERNA SOBE E VIRA INDICE NA PÁGINA PAI

            for(; meio < tam; meio++)
            {
                ApTemp->UU.U1.re[i] = (*Ap)->UU.U1.re[meio];
                ApTemp->UU.U1.ne++;
                (*Ap)->UU.U1.ne--;
                i++;
            }

            if(reg.chave >= RegRetorno->chave)
            {
                ApTemp->UU.U1.re[i] = reg;
                ApTemp->UU.U1.ne++;
            } else{
                (*Ap)->UU.U1.re[(*Ap)->UU.U1.ne] = reg;
                (*Ap)->UU.U1.ne++;
            }

            //ORDENA OS REGISTROS EM APTEMP DE ACORDO COM A CHAVE

            short NaoAchouPosicao;
            int k;
            k = (*Ap)->UU.U1.ne;
            NaoAchouPosicao = (k > 0);

            while (NaoAchouPosicao) //LOOP ATÉ ENCONTRA A POSIÇÃO PARA ADICIONAR O NOVO REGISTRO
            {
                if(reg.chave >= ApTemp->UU.U1.re[k-1].chave)
                    NaoAchouPosicao = FALSE;
                else
                {
                    ApTemp->UU.U1.re[k] = ApTemp->UU.U1.re[k-1];    //AVANÇA O REGISTRO UMA POSIÇÃO Á FRENTE
                    k--;
                    if(k < 1) NaoAchouPosicao = FALSE;
                }
            }

            *Cresceu = TRUE;
            *ApRetorno = ApTemp;
        }
        else    //SE TIVER ESPAÇO ADICIONA O REGISTRO
        {

            //ORDENA OS REGISTROS EM AP DE ACORDO COM A CHAVE

            short NaoAchouPosicao;
            int k;
            k = (*Ap)->UU.U1.ne;
            NaoAchouPosicao = (k > 0);

            while (NaoAchouPosicao) //LOOP ATÉ ENCONTRA A POSIÇÃO PARA ADICIONAR O NOVO REGISTRO
            {
                if(reg.chave >= (*Ap)->UU.U1.re[k-1].chave)
                    NaoAchouPosicao = FALSE;
                else
                {
                    (*Ap)->UU.U1.re[k] = (*Ap)->UU.U1.re[k-1];    //AVANÇA O REGISTRO UMA POSIÇÃO Á FRENTE
                    k--;
                    if(k < 1) NaoAchouPosicao = FALSE;
                }
            }

            (*Ap)->UU.U1.re[k] = reg;    //INSERE NOVO REGISTRO
            (*Ap)->UU.U1.ne++; //INCREMENTA O NUMERO DE REGISTROS
            *ApRetorno = ApTemp;
            *Cresceu = FALSE;
            *RegRetorno = reg;
        }
    }
}

void imprimi(Apontador arvore)
{
    int i = 0;
    if (arvore == NULL) return;

    if(arvore->Pt == Interna) {
        printf("Interna: ");
        while (i <= arvore->UU.U0.ni) {
            imprimi(arvore->UU.U0.pi[i]);

            if (i != arvore->UU.U0.ni)
                printf("[%i] ", arvore->UU.U0.ri[i]);
            i++;
        }
    }
    else
    {
        printf("Externa: ");
        while(i < arvore->UU.U1.ne)
        {
            if (i != arvore->UU.U0.ni)
                printf("[%i] ", arvore->UU.U1.re[i].chave);
            i++;
        }
    }
    printf("\n");
}

void InsereNaPagina(Apontador Ap, int chave, Apontador ApDir)
{
        short NaoAchouPosicao;
        int k;
        k = Ap->UU.U0.ni;
        NaoAchouPosicao = (k > 0);

        while (NaoAchouPosicao) //LOOP ATÉ ENCONTRA A POSIÇÃO PARA ADICIONAR A NOVA CHAVE
        {
            if (chave >= Ap->UU.U0.ri[k - 1])
                NaoAchouPosicao = FALSE;
            else {
                Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];    //AVANÇA A CHAVE UMA POSIÇÃO Á FRENTE
                Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];   //INSERE APONTADOR DA POSIÇÃO ANTERIOR NA PRÓXIMA POSIÇÃO
                k--;
                if (k < 1) NaoAchouPosicao = FALSE;
            }
        }
        Ap->UU.U0.ri[k] = chave;    //INSERE NOVA CHAVE
        Ap->UU.U0.pi[k + 1] = ApDir;  //ADICIONA NOVO APONTADOR
        Ap->UU.U0.ni++; //INCREMENTA O NUMERO DE CHAVES
}
