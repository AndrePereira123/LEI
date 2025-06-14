#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include "../include/struct_utilizador.h"
#include "../include/struct_reserva.h"
#include "../include/gestor_passageiro.h"
#include "../include/utilidade_tempo.h"


#define ERRO 0
#define CERTO 1
#define MES 1
#define ANO 2
#define HASH_ASCII 300

struct BTREE_REGISTOS_ANO
{
    int Ano_todo;
    int Janeiro [31];
    int Fevereiro [31];
    int Marco [31];
    int Abril [31];
    int Maio [31];
    int Junho [31];
    int Julho [31];
    int Agosto [31];
    int Setembro [31];
    int Outubro [31];
    int Novembro [31];
    int Decembro [31];
};

struct BTREE_UTILIZADORES
{
    void* user;
    btree_utilizadores* esq;
    btree_utilizadores* dir;
};


btree_registos_ano** criar_lista_anos_registos()
{
    btree_registos_ano** registos = malloc (sizeof(btree_registos_ano*) * 14); //2003 a 2023
    int i = 0;
    while (i < 14)
    {
    registos [i] = criar_ano_registo ();
    i++;
    }
    return registos;
}

btree_registos_ano* criar_ano_registo ()
{
    btree_registos_ano* novo_ano = calloc (1,sizeof(btree_registos_ano));
    return novo_ano;
}

btree_utilizadores* criar_arvore_vazia_utilizador() 
{
    btree_utilizadores* arvore = malloc (sizeof (btree_utilizadores));
    arvore->user = NULL;
    arvore->dir = NULL;
    arvore->esq = NULL;
    return arvore;
}


btree_utilizadores* inserir_utilizador (btree_utilizadores* arvore, void* Nodo_novo) 
{
        if(arvore == NULL)
        {
            arvore = criar_arvore_vazia_utilizador();
            arvore->user = Nodo_novo;
            return arvore;
        }
        else if (arvore->user == NULL)
        {
            arvore->user = Nodo_novo;
            return arvore;
        }
        else 
        {
        int valor;
        valor = compara_nomes (retorna_nome_utilizador (arvore->user), retorna_nome_utilizador(Nodo_novo)); //inserimos alfabeticamente

        if (valor == 1)
            {
                arvore->esq = inserir_utilizador (arvore->esq,Nodo_novo);   
            }
            else if (valor == 2)
            {
                arvore->dir = inserir_utilizador (arvore->dir,Nodo_novo); //nome maior alfabeticamente para direita
            }
            else if (valor == 3)           
            {
                valor = compara_nomes (retorna_id_utilizador (arvore->user),retorna_id_utilizador(Nodo_novo)); //desempatamos por id de utilizador
                if (valor == 1)
                {
                    arvore->esq = inserir_utilizador (arvore->esq,Nodo_novo); 
                }
                else 
                {
                    arvore->dir = inserir_utilizador (arvore->dir,Nodo_novo); //id maior alfabeticamente para a direita
                }
            }
        }
    return arvore;
}

void inserir_registo_utilizador (int ano_registo,btree_registos_ano** registos,int* data)
{
        registos[ano_registo]->Ano_todo++;
        if      (data[0] == 1) registos[ano_registo]->Janeiro[data[1]]++;
        else if (data[0] == 2) registos[ano_registo]->Fevereiro[data[1]]++;
        else if (data[0] == 3) registos[ano_registo]->Marco[data[1]]++;
        else if (data[0] == 4) registos[ano_registo]->Abril[data[1]]++;
        else if (data[0] == 5) registos[ano_registo]->Maio[data[1]]++;
        else if (data[0] == 6) registos[ano_registo]->Junho[data[1]]++;
        else if (data[0] == 7) registos[ano_registo]->Julho[data[1]]++;
        else if (data[0] == 8) registos[ano_registo]->Agosto[data[1]]++;
        else if (data[0] == 9) registos[ano_registo]->Setembro[data[1]]++;
        else if (data[0] ==10) registos[ano_registo]->Outubro[data[1]]++;
        else if (data[0] ==11) registos[ano_registo]->Novembro[data[1]]++;
        else if (data[0] ==12) registos[ano_registo]->Decembro[data[1]]++;
}


char* retorna_nome (btree_utilizadores* arvore)
{
    return (retorna_id_utilizador (arvore->user));
}


int querie_9_interativa (btree_utilizadores* arvore,char* prefixo,char* resposta,int* contador,int F)
{
    if (arvore == NULL) {return -1;};
    int resultado = verificar_prefixo (retorna_nome_utilizador (arvore->user),prefixo);
    
    if (F == 0)
    {
    if (resultado == 0) 
    {
         querie_9_interativa (arvore->esq,prefixo,resposta,contador,F);
        sprintf (resposta + strlen (resposta),"%s;",retorna_id_utilizador (arvore->user));
        sprintf (resposta + strlen (resposta),"%s\n",retorna_nome_utilizador (arvore->user));
         querie_9_interativa (arvore->dir,prefixo,resposta,contador,F);
    }
    else 
    {
    if (resultado == -1) //prefixo do nome é menor
    {
    querie_9_interativa (arvore->dir,prefixo,resposta,contador,F);
    
    }
    else if (resultado == 1) //prefixo do nome é maior 
    {
    querie_9_interativa (arvore->esq,prefixo,resposta,contador,F);
    querie_9_interativa (arvore->dir,prefixo,resposta,contador,F);
    }
    else 
    {
    querie_9_interativa (arvore->esq,prefixo,resposta,contador,F);
    querie_9_interativa (arvore->dir,prefixo,resposta,contador,F);
    }
    }
    }
    else 
    {
        if (resultado == 0) 
    {
         querie_9_interativa (arvore->esq,prefixo,resposta,contador,F);
        sprintf (resposta + strlen (resposta),"--- %d ---\n",*contador);
        (*contador)++;
        sprintf (resposta + strlen (resposta),"id: %s\n",retorna_id_utilizador (arvore->user));
        sprintf (resposta + strlen (resposta),"name: %s\n",retorna_nome_utilizador (arvore->user));
        sprintf (resposta + strlen (resposta)," \n");

         querie_9_interativa (arvore->dir,prefixo,resposta,contador,F);
    }
    else 
    {
    if (resultado == -1) //prefixo do nome é menor
    {
    querie_9_interativa (arvore->dir,prefixo,resposta,contador,F);
    }
    else if (resultado == 1) //prefixo do nome é maior 
    {
    querie_9_interativa (arvore->esq,prefixo,resposta,contador,F);
    }
    else 
    {
        querie_9_interativa (arvore->esq,prefixo,resposta,contador,F);
        querie_9_interativa (arvore->dir,prefixo,resposta,contador,F);
    }
    }
    }
    return 0;
}


void print_querie_9 (btree_utilizadores* arvore,char* prefixo,FILE* ficheiro_output,int* contador,int F) 
{
     if (arvore == NULL ) return;              
    int resultado = verificar_prefixo (retorna_nome_utilizador (arvore->user),prefixo);
    
    if (F == 0)
    {
    if (resultado == 0) 
    {
         print_querie_9 (arvore->esq,prefixo,ficheiro_output,contador,F);
        fprintf (ficheiro_output,"%s;",retorna_id_utilizador (arvore->user));
        fprintf (ficheiro_output,"%s\n",retorna_nome_utilizador (arvore->user));
         print_querie_9 (arvore->dir,prefixo,ficheiro_output,contador,F);
    }
    else 
    {
    if (resultado == -1) //prefixo do nome é menor
    {
    print_querie_9 (arvore->dir,prefixo,ficheiro_output,contador,F);
    
    }
    else if (resultado == 1) //prefixo do nome é maior 
    {
    print_querie_9 (arvore->esq,prefixo,ficheiro_output,contador,F);
    print_querie_9 (arvore->dir,prefixo,ficheiro_output,contador,F);
    }
    else 
    {
        print_querie_9 (arvore->esq,prefixo,ficheiro_output,contador,F);
        print_querie_9 (arvore->dir,prefixo,ficheiro_output,contador,F);
    }
    }
    }
    else 
    {
        if (resultado == 0) 
    {
         print_querie_9 (arvore->esq,prefixo,ficheiro_output,contador,F);
        if (*contador > 1) fprintf (ficheiro_output,"\n");
        fprintf (ficheiro_output,"--- %d ---\n",*contador);
        (*contador)++;
        fprintf (ficheiro_output,"id: %s\n",retorna_id_utilizador (arvore->user));
        fprintf (ficheiro_output,"name: %s\n",retorna_nome_utilizador (arvore->user));
         print_querie_9 (arvore->dir,prefixo,ficheiro_output,contador,F);
    }
    else 
    {
    if (resultado == -1) //prefixo do nome é menor
    {
    print_querie_9 (arvore->dir,prefixo,ficheiro_output,contador,F);
    }
    else if (resultado == 1) //prefixo do nome é maior 
    {
    print_querie_9 (arvore->esq,prefixo,ficheiro_output,contador,F);
    }
    else 
    {
        print_querie_9 (arvore->esq,prefixo,ficheiro_output,contador,F);
        print_querie_9 (arvore->dir,prefixo,ficheiro_output,contador,F);
    }
    }
    }
}


void calcular_utilizadores_data (int data[2],btree_registos_ano** anos_registos_utilizadores,int* numeros_utilizadores,int tipo)
{
    int ano = data[0] - 2010;
    int i = 0;
    if (tipo == MES)
    {
        switch (data[1])
        {
            case 1:
                    for (i = 0; i < 31;i++)  numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Janeiro[i];
                    break;
            case 2:
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Fevereiro[i];
                    break;
            case 3:
                   for (i = 0; i < 31;i++)  numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Marco[i];
                    break;
            case 4:
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Abril[i];
                    break;
            case 5:
                   for (i = 0; i < 31;i++)  numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Maio[i];
                    break;
            case 6:
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Junho[i];
                    break;
            case 7:     
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Julho[i];
                    break;
            case 8:
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Agosto[i];
                    break;
            case 9:
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Setembro[i];
                    break;
            case 10:
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Outubro[i];
                    break;
            case 11:
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Novembro[i];
                    break;
            case 12:
                    for (i = 0; i < 31;i++) numeros_utilizadores[i] = anos_registos_utilizadores[ano]->Decembro[i];
                    break;
        }
    } else if (tipo == ANO)
    {
        numeros_utilizadores[0] = sum (anos_registos_utilizadores[ano]->Janeiro,30);
        numeros_utilizadores[1] = sum (anos_registos_utilizadores[ano]->Fevereiro,30);
        numeros_utilizadores[2] = sum (anos_registos_utilizadores[ano]->Marco,30);
        numeros_utilizadores[3] = sum (anos_registos_utilizadores[ano]->Abril,30);
        numeros_utilizadores[4] = sum (anos_registos_utilizadores[ano]->Maio,30);
        numeros_utilizadores[5] = sum (anos_registos_utilizadores[ano]->Junho,30);
        numeros_utilizadores[6] = sum (anos_registos_utilizadores[ano]->Julho,30);
        numeros_utilizadores[7] = sum (anos_registos_utilizadores[ano]->Agosto,30);
        numeros_utilizadores[8] = sum (anos_registos_utilizadores[ano]->Setembro,30);
        numeros_utilizadores[9] = sum (anos_registos_utilizadores[ano]->Outubro,30);
        numeros_utilizadores[10] = sum (anos_registos_utilizadores[ano]->Novembro,30);
        numeros_utilizadores[11] = sum (anos_registos_utilizadores[ano]->Decembro,30);
    }
    else 
    {
        numeros_utilizadores[0]  = somar_ano_utilizadores (anos_registos_utilizadores[0]);
        numeros_utilizadores[1]  = somar_ano_utilizadores (anos_registos_utilizadores[1]);
        numeros_utilizadores[2]  = somar_ano_utilizadores (anos_registos_utilizadores[2]);
        numeros_utilizadores[3]  = somar_ano_utilizadores (anos_registos_utilizadores[3]);
        numeros_utilizadores[4]  = somar_ano_utilizadores (anos_registos_utilizadores[4]);
        numeros_utilizadores[5]  = somar_ano_utilizadores (anos_registos_utilizadores[5]);
        numeros_utilizadores[6]  = somar_ano_utilizadores (anos_registos_utilizadores[6]);
        numeros_utilizadores[7]  = somar_ano_utilizadores (anos_registos_utilizadores[7]);
        numeros_utilizadores[8]  = somar_ano_utilizadores (anos_registos_utilizadores[8]);
        numeros_utilizadores[9]  = somar_ano_utilizadores (anos_registos_utilizadores[9]);
        numeros_utilizadores[10] = somar_ano_utilizadores (anos_registos_utilizadores[10]);
        numeros_utilizadores[11] = somar_ano_utilizadores (anos_registos_utilizadores[11]);
        numeros_utilizadores[12] = somar_ano_utilizadores (anos_registos_utilizadores[12]);
        numeros_utilizadores[13] = somar_ano_utilizadores (anos_registos_utilizadores[13]);
    }
}


int somar_ano_utilizadores (btree_registos_ano* anos_registos_utilizadores)
{
    int valor = 0;
       valor+= sum (anos_registos_utilizadores->Janeiro,30);
       valor+= sum (anos_registos_utilizadores->Fevereiro,30);
       valor+= sum (anos_registos_utilizadores->Marco,30);
       valor+= sum (anos_registos_utilizadores->Abril,30);
       valor+= sum (anos_registos_utilizadores->Maio,30);
       valor+= sum (anos_registos_utilizadores->Junho,30);
       valor+= sum (anos_registos_utilizadores->Julho,30);
       valor+= sum (anos_registos_utilizadores->Agosto,30);
       valor+= sum (anos_registos_utilizadores->Setembro,30);
       valor+= sum (anos_registos_utilizadores->Outubro,30);
       valor+= sum (anos_registos_utilizadores->Novembro,30);
       valor+= sum (anos_registos_utilizadores->Decembro,30);
    return valor;
}

////////////////////////////////////////////////////////////////////////FREES/////////////////////////////////////////////////////////

void free_registos_utilizadores(btree_registos_ano** anos_registos_utilizadores)
{
    int p ;
    for (p = 0; p < 14;p++)
    {
        free (anos_registos_utilizadores[p]);
    }
    free (anos_registos_utilizadores);
}


void free_single_tree (btree_utilizadores* arvore)
{
        if (arvore != NULL)
        {
            free_single_tree (arvore->esq);
            free_single_tree (arvore->dir);
        }
        free (arvore);
}

void free_hash_utilizadores_inicial (btree_utilizadores** hash_por_inicial)
{
    int i = 0;
    while (i < HASH_ASCII)
    {
        if (hash_por_inicial[i] != NULL) free_single_tree (hash_por_inicial[i]);
        i++;
    }
    free(hash_por_inicial);
}