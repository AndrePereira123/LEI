#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include "../include/struct_reserva.h"
#include "../include/struct_utilizador.h"
#include "../include/utilidade_tempo.h"
#include "../include/erros.h"
#define HASHES_HOTEL 10000
#define TAMANHO_HASH_ID_RESERVAS 5800

#define MES 1
#define ANO 2



struct ARRAY_RESERVAS
{
    char id_reserva[15];
    int data[3];
    array_reservas* prox;
};

struct Reservas_Arvore{
    void* Nodo;
    reservas_arvore* esquerda;
    reservas_arvore* direita;
    int altura;
};

struct BTREE_RESERVAS_UTILIZADOR {
    void* reserva_utilizador;
    btree_reservas_utilizador* esq;
    btree_reservas_utilizador* dir;
};

struct BTREE_REGISTOS_RESERVAS
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


btree_registos_reservas** criar_lista_registos_reservas() 
{
    btree_registos_reservas** registos = malloc (sizeof(btree_registos_reservas*) * 14); //2003 a 2023
    int i = 0;
    while (i < 14)
    {
    registos [i] = criar_ano_reservas ();
    i++;
    }
    return registos;
}

btree_registos_reservas* criar_ano_reservas ()
{
    btree_registos_reservas* novo_ano = calloc (1,sizeof(btree_registos_reservas));
    return novo_ano;
}

reservas_arvore* criar_arvore_vazia() 
{
    reservas_arvore* arvore = malloc (sizeof (reservas_arvore));
    arvore->Nodo = NULL;
    arvore->direita = NULL;
    arvore->esquerda = NULL;
    arvore->altura = 1;
    return arvore;
}

btree_reservas_utilizador* criar_arvore_reservas_utilizador ()
{
    btree_reservas_utilizador* arvore = malloc (sizeof (btree_reservas_utilizador));
    arvore->dir = NULL;
    arvore->esq = NULL;
    arvore->reserva_utilizador = NULL;
    return arvore;
}

reservas_arvore* Rodar_reserva_direita (reservas_arvore *y) 
{
    if (y->esquerda == NULL) return y;
    reservas_arvore *nova_cabeca = y->esquerda;
    reservas_arvore *nova_esquerda_cabeca_antiga = nova_cabeca->direita;

    nova_cabeca->direita = y;
    y->esquerda = nova_esquerda_cabeca_antiga;

    
    y->altura = Maior_altura(y) + 1;
    nova_cabeca->altura = Maior_altura (nova_cabeca) + 1;

    return nova_cabeca;
}

reservas_arvore* Rodar_reserva_esquerda (reservas_arvore* x) 
{
    if (x->direita == NULL) return x;
    reservas_arvore* nova_cabeca = x->direita;
    reservas_arvore* nova_direita_cabeca_antiga = nova_cabeca->esquerda;

    nova_cabeca->esquerda = x;
    x->direita = nova_direita_cabeca_antiga;

    x->altura = Maior_altura(x) + 1;
    nova_cabeca->altura = Maior_altura(nova_cabeca) + 1;

    return nova_cabeca;
}

int fator_balanceamento (reservas_arvore* arvore)
{
    if (arvore == NULL) return 0;
    if (arvore->direita  == NULL && arvore->esquerda == NULL) return 0;
    if (arvore->direita  == NULL) return (arvore->esquerda->altura);
    if (arvore->esquerda == NULL) return (0-arvore->direita->altura);
    return (arvore->esquerda->altura - arvore->direita->altura);
}

int Maior_altura (reservas_arvore* arvore)
{   
    if (arvore == NULL) return 0;
    int d = 0;
    int e = 0;
    if (arvore->direita != NULL) d = arvore->direita->altura; 
    if (arvore->esquerda != NULL) e = arvore->esquerda->altura;
    return (MAX (d,e)); 
}



reservas_arvore* inserir_reserva (reservas_arvore* arvore, void* Nodo_novo) //insere uma reserva numa arvore de reservas
    {
        if (arvore == NULL)
        {
            arvore = criar_arvore_vazia();
            arvore->Nodo = Nodo_novo;
            return arvore;
        }
        else if (arvore->Nodo == NULL)
        {
            arvore->Nodo = Nodo_novo;
            return arvore;
        }
        else 
        {
        int valor;
        valor = comparar_datas (retorna_data_inicio (arvore->Nodo), retorna_data_inicio (Nodo_novo));  //insere dependendo das datas de inicio da reserva

        if (valor == 1)
            {
                arvore->esquerda = inserir_reserva (arvore->esquerda,Nodo_novo);   //datas maiores para direita
            }
            else if (valor == 0)
            {
                 arvore->direita = inserir_reserva (arvore->direita,Nodo_novo);
            }
             else if (valor == 3)           
            {
              if (compara_ids (retorna_id_reserva (arvore->Nodo), retorna_id_reserva (Nodo_novo)) == 1) //desempata por id caso 2 datas sejam iguais
                {
                    arvore->direita = inserir_reserva (arvore->direita,Nodo_novo);  //ids menores para a direita 
                }
                else 
                {
                    arvore->esquerda = inserir_reserva (arvore->esquerda,Nodo_novo);
                }
            }
            
            if (arvore->esquerda == NULL && arvore->direita == NULL)
            {
                return arvore;
            }

            arvore->altura = 1 + Maior_altura(arvore);
            int BF = fator_balanceamento (arvore);

            if (arvore->esquerda != NULL && arvore->direita != NULL)
            {

           
            int comparar_esq = comparar_datas (retorna_data_inicio (arvore->esquerda->Nodo) , retorna_data_inicio (Nodo_novo));
            
            if (BF > 1 && (comparar_esq == 1 || (comparar_esq == 3 && compara_ids (retorna_id_reserva (Nodo_novo) , retorna_id_reserva (arvore->esquerda->Nodo)) == 1)))
             return Rodar_reserva_direita (arvore);


            int comparar_dir = comparar_datas (retorna_data_inicio (arvore->direita->Nodo) , retorna_data_inicio (Nodo_novo));

            if (BF < -1 && (comparar_dir == 0 || (comparar_dir == 3 &&  compara_ids (retorna_id_reserva (Nodo_novo) , retorna_id_reserva (arvore->direita->Nodo)) != 1)))
            return Rodar_reserva_esquerda(arvore);


            if (BF > 1 && (comparar_esq == 0 || (comparar_esq == 3 && compara_ids (retorna_id_reserva (Nodo_novo) , retorna_id_reserva (arvore->esquerda->Nodo)) != 1)))
            {
                arvore->esquerda = Rodar_reserva_esquerda(arvore->esquerda);
                return Rodar_reserva_direita(arvore);
            }

            if (BF < -1 && (comparar_dir == 1 || (comparar_dir == 3 &&  compara_ids (retorna_id_reserva (Nodo_novo) , retorna_id_reserva (arvore->direita->Nodo)) == 1)))
            {
                arvore->direita = Rodar_reserva_direita(arvore->direita);
                return Rodar_reserva_esquerda(arvore);
            }

            
            }
    }
    return arvore;
}

btree_reservas_utilizador* inserir_reserva_utilizador (void* reserva_nova,btree_reservas_utilizador** arvore_mae) 
 {
    btree_reservas_utilizador* arvore = *arvore_mae;
    if (arvore == NULL)
    {
        arvore = criar_arvore_reservas_utilizador();
        arvore->reserva_utilizador = reserva_nova;
        *arvore_mae = arvore;
    }
    else 
    {
        int valor;
        valor = comparar_datas (retorna_data_inicio (arvore->reserva_utilizador),retorna_data_inicio (reserva_nova));
        
        if (valor == 0)
        {
            arvore->dir = inserir_reserva_utilizador (reserva_nova,&arvore->dir);
        }
        else if (valor == 1)
        {
            arvore->esq = inserir_reserva_utilizador (reserva_nova,&arvore->esq);
        }
        else if (valor == 3)
        {
            if ( (atoi (retorna_id_reserva (reserva_nova) + 4)) < (atoi (retorna_id_reserva (arvore->reserva_utilizador) + 4)) )
            {
                arvore->dir = inserir_reserva_utilizador (reserva_nova,&arvore->dir);
            }
            else 
            {
               arvore->esq = inserir_reserva_utilizador (reserva_nova,&arvore->esq); 
            }
        }

    }
    return arvore;
 }     

/////////////////////////////////////////////////////////QUERIE 2 RESERVAS/////////////////////////////////////////////////////

void print_reservas_q2_interativo(btree_reservas_utilizador* arvore,char* resposta,int* contador,int F)
{
    if(arvore == NULL) return;
    if (F == 0)
    {
    print_reservas_q2_interativo (arvore->dir,resposta,contador,F);
    sprintf(resposta + strlen(resposta),"%s;%d/%02d/%02d\n", retorna_id_reserva (arvore->reserva_utilizador),retorna_data_inicio (arvore->reserva_utilizador)[0],retorna_data_inicio (arvore->reserva_utilizador)[1],retorna_data_inicio (arvore->reserva_utilizador)[2]);
    print_reservas_q2_interativo (arvore->esq,resposta,contador,F);
    }
    else 
    {   
         print_reservas_q2_interativo (arvore->dir,resposta,contador,F);

        
        sprintf(resposta+ strlen(resposta),"--- %d ---\n",*contador);
        (*contador)++;
        sprintf(resposta+ strlen(resposta),"id: %s\n",retorna_id_reserva (arvore->reserva_utilizador));
        sprintf(resposta+ strlen(resposta),"date: %d/%02d/%02d\n",retorna_data_inicio (arvore->reserva_utilizador)[0],retorna_data_inicio (arvore->reserva_utilizador)[1],retorna_data_inicio (arvore->reserva_utilizador)[2]);
        sprintf(resposta+ strlen(resposta)," \n");

        print_reservas_q2_interativo (arvore->esq,resposta,contador,F);
    }
}

void print_reservas_q2 (btree_reservas_utilizador* arvore, FILE *ficheiro_output)
{
    if(arvore == NULL) return;
    print_reservas_q2(arvore->dir,ficheiro_output);
    fprintf(ficheiro_output,"%s;%d/%02d/%02d\n",retorna_id_reserva (arvore->reserva_utilizador),retorna_data_inicio (arvore->reserva_utilizador)[0],retorna_data_inicio (arvore->reserva_utilizador)[1],retorna_data_inicio (arvore->reserva_utilizador)[2]);
    print_reservas_q2(arvore->esq,ficheiro_output);
}

void print_reservas_q2F (btree_reservas_utilizador* arvore, FILE *ficheiro_output,int* contador)
{
        if(arvore == NULL) return;
        print_reservas_q2F(arvore->dir,ficheiro_output,contador);
        if(*contador > 1) fprintf(ficheiro_output,"\n");
        fprintf(ficheiro_output,"--- %d ---\n",*contador);
        (*contador)++;

        fprintf(ficheiro_output,"id: %s\n",retorna_id_reserva (arvore->reserva_utilizador));
        fprintf(ficheiro_output,"date: %d/%02d/%02d\n",retorna_data_inicio (arvore->reserva_utilizador)[0],retorna_data_inicio (arvore->reserva_utilizador)[1],retorna_data_inicio (arvore->reserva_utilizador)[2]);

        print_reservas_q2F(arvore->esq,ficheiro_output,contador);
}

void guardar_btree_id_reservas (array_reservas** reservas,btree_reservas_utilizador* dados_utilizadores_id)
{
    if (dados_utilizadores_id != NULL)
    {
        guardar_btree_id_reservas (reservas,dados_utilizadores_id->esq);
        if (*reservas == NULL)
        {
            array_reservas* novo = malloc (sizeof (array_reservas));
            novo->prox = NULL;
            strcpy (novo->id_reserva,retorna_id_reserva (dados_utilizadores_id->reserva_utilizador));
            novo->data[0] = retorna_data_inicio (dados_utilizadores_id->reserva_utilizador)[0];
            novo->data[1] = retorna_data_inicio (dados_utilizadores_id->reserva_utilizador)[1];
            novo->data[2] = retorna_data_inicio (dados_utilizadores_id->reserva_utilizador)[2];
            (*reservas) = novo;
        }
        else 
        {
            array_reservas* atual = (*reservas);
            array_reservas* novo = malloc (sizeof(array_reservas));
            novo->prox = atual;
            strcpy (novo->id_reserva,retorna_id_reserva (dados_utilizadores_id->reserva_utilizador));
            novo->data[0] = retorna_data_inicio (dados_utilizadores_id->reserva_utilizador)[0];
            novo->data[1] = retorna_data_inicio (dados_utilizadores_id->reserva_utilizador)[1];
            novo->data[2] = retorna_data_inicio (dados_utilizadores_id->reserva_utilizador)[2];
            (*reservas) = novo;
        }
        guardar_btree_id_reservas (reservas,dados_utilizadores_id->dir);
     }
}

void retorna_data_reserva(int data[3],btree_reservas_utilizador* res)
{
    data[0] = retorna_data_inicio (res->reserva_utilizador)[0];
    data[1] = retorna_data_inicio (res->reserva_utilizador)[1];
    data[2] = retorna_data_inicio (res->reserva_utilizador)[2];
}

btree_reservas_utilizador* esquerda_arvore_reservas(btree_reservas_utilizador* arvore)
{
    return arvore->esq;
}

btree_reservas_utilizador* direita_arvore_reservas(btree_reservas_utilizador* arvore)
{
    return arvore->dir;
}


int Somar_ratings (reservas_arvore* total_dados,double* N_nodos)
{  
    if (total_dados == NULL) return 0;
    int soma_final = 0;
    if (strcmp (retorna_classificacao_reserva (total_dados->Nodo),"vazio") == 0)         //soma todos os ratings validos recursivamente
    {
        soma_final = Somar_ratings (total_dados->esquerda,N_nodos) + Somar_ratings (total_dados->direita,N_nodos);
    }
    else 
    {
    soma_final = Somar_ratings (total_dados->esquerda,N_nodos) + atoi(retorna_classificacao_reserva (total_dados->Nodo)) + Somar_ratings (total_dados->direita,N_nodos);
    }
    (*N_nodos)+= 1;             //aumenta o numero de nodos a cada chamada
    return (soma_final);
}


//-----------------------------------------Querie 4---------------------------------------------------------------------//
double total_price (int* data_inicio,int* data_fim,char* preco,char* imposto)
 {
    float dias,preco_noite, imposto_i, resultado;
    dias = (data_fim[2]- data_inicio[2]);               //funcao aussiliar para calcular o preco total de cada reserva
    preco_noite = atof (preco);
    imposto_i = atof (imposto);
    
    resultado = (preco_noite * dias ) + ( (preco_noite * dias) * 0.01 * imposto_i );
    return resultado;
 }

void querie4_interativa   (reservas_arvore* arvore,char* resposta,int* contador,int F)
{
     if (arvore == NULL) return;
        querie4_interativa (arvore->direita,resposta,contador,F);
    double custo = 0.000;
    reserva* nodo = arvore->Nodo;
    custo = total_price (retorna_data_inicio (nodo),retorna_data_fim (nodo),retorna_preco_noite(nodo),retorna_imposto_cidade(nodo));
        
        if (F == 0)
        {
            sprintf(resposta + strlen (resposta),"%s;%d/%02d/%02d;%d/%02d/%02d;%s;%s;%.3f\n",retorna_id_reserva (nodo),retorna_data_inicio (nodo)[0],retorna_data_inicio (nodo)[1],retorna_data_inicio (nodo)[2]
                                                                                            ,retorna_data_fim (nodo)[0],retorna_data_fim (nodo)[1],retorna_data_fim (nodo)[2]
                                                                                            ,retorna_id_utilizador_res(nodo),retorna_classificacao_reserva (nodo),custo);
        }
        else
        {
             sprintf(resposta + strlen (resposta),"--- %d ---\n",*contador);
             sprintf(resposta + strlen (resposta),"id: %s\n",retorna_id_reserva (nodo));
             sprintf(resposta + strlen (resposta),"begin_date: %02d/%02d/%02d\n",retorna_data_inicio (nodo)[0],retorna_data_inicio (nodo)[1],retorna_data_inicio (nodo)[2]);
             sprintf(resposta + strlen (resposta),"end_date: %02d/%02d/%02d\n",retorna_data_fim (nodo)[0],retorna_data_fim (nodo)[1],retorna_data_fim (nodo)[2]);
             sprintf(resposta + strlen (resposta),"user_id: %s\n",retorna_id_utilizador_res(nodo));
             sprintf(resposta + strlen (resposta),"rating: %s\n",retorna_classificacao_reserva (nodo));
             sprintf(resposta + strlen (resposta),"total_price: %.3f\n",custo);
             sprintf(resposta + strlen (resposta)," \n");
             (*contador)++;
        }
        querie4_interativa (arvore->esquerda,resposta,contador,F);
}
void print_querie_4 (reservas_arvore* arvore,FILE* ficheiro_output,int* contador,int F)
{
    if (arvore != NULL)
    {
        print_querie_4(arvore->direita,ficheiro_output,contador,F);
        if (F == 1)
        {
        if (*contador != 1) fprintf(ficheiro_output,"\n");
        fprintf(ficheiro_output,"--- %d ---\n",*contador);      
        print_nodo_q4F(arvore->Nodo,ficheiro_output);
        (*contador)++;
        }
        else print_nodo_q4(arvore->Nodo,ficheiro_output);

        
        print_querie_4(arvore->esquerda,ficheiro_output,contador,F);
    }
}

void print_nodo_q4F(void* nodo,FILE* ficheiro_output)
{
    double custo = 0.000;
    custo = total_price (retorna_data_inicio (nodo),retorna_data_fim (nodo),retorna_preco_noite(nodo),retorna_imposto_cidade(nodo));

    fprintf(ficheiro_output,"id: %s\n",retorna_id_reserva (nodo));
    fprintf(ficheiro_output,"begin_date: %02d/%02d/%02d\n",retorna_data_inicio (nodo)[0],retorna_data_inicio (nodo)[1],retorna_data_inicio (nodo)[2]);
    fprintf(ficheiro_output,"end_date: %02d/%02d/%02d\n",retorna_data_fim (nodo)[0],retorna_data_fim (nodo)[1],retorna_data_fim (nodo)[2]);
    fprintf(ficheiro_output,"user_id: %s\n",retorna_id_utilizador_res(nodo));
    fprintf(ficheiro_output,"rating: %s\n",retorna_classificacao_reserva (nodo));
    fprintf(ficheiro_output,"total_price: %.3f\n",custo);
}


void print_nodo_q4(void* nodo,FILE* ficheiro_output)
 {
    double custo = 0.000;
    custo = total_price (retorna_data_inicio (nodo),retorna_data_fim (nodo),retorna_preco_noite(nodo),retorna_imposto_cidade(nodo));

    fprintf(ficheiro_output,"%s;%d/%02d/%02d;%d/%02d/%02d;%s;%s;%.3f\n",retorna_id_reserva (nodo),retorna_data_inicio (nodo)[0],retorna_data_inicio (nodo)[1],retorna_data_inicio (nodo)[2]
                                                                       ,retorna_data_fim (nodo)[0],retorna_data_fim (nodo)[1],retorna_data_fim (nodo)[2]
                                                                       ,retorna_id_utilizador_res(nodo),retorna_classificacao_reserva (nodo),custo);
 }


 void querie8_interativa(reservas_arvore* arvore,char* resposta,int data_inicio[3],int data_fim[3])
{
    if (arvore == NULL) {strcpy (resposta, "Nao existe um Hotel com esse id !\n");}
    int lucro_total = 0;
    lucro_total = somar_lucro (arvore,data_inicio,data_fim);   
    sprintf(resposta+strlen (resposta),"%d\n",lucro_total);
}



void print_querie_8(reservas_arvore* arvore,FILE* ficheiro_output,int data_inicio[3],int data_fim[3])
{
    if (arvore == NULL)  {return;}
    int lucro_total = 0;
    lucro_total = somar_lucro (arvore,data_inicio,data_fim);   
    fprintf(ficheiro_output,"%d\n",lucro_total);
}


int somar_lucro (reservas_arvore* arvore,int data_inicio[3],int data_fim[3])
{                                                       //range representa o espaco de tempo entre as datas requisitadas

    if (arvore == NULL || arvore->Nodo == NULL) 
    {
        return 0;
    }

    if (comparar_datas (retorna_data_inicio (arvore->Nodo),data_fim) == 1) // datas demasiado recentes para o range
    { 
        return (somar_lucro(arvore->esquerda,data_inicio,data_fim)); //procurar para a esquerda
    }
    if (comparar_datas (retorna_data_fim (arvore->Nodo),data_inicio) == 0 ) // datas demasiado antigas para o range
    {
        return (somar_lucro (arvore->direita,data_inicio,data_fim) + somar_lucro (arvore->esquerda,data_inicio,data_fim)); // procurar para os dois lados
    }

    int lucro = 0;

   
    int comparacao_inicios = comparar_datas (retorna_data_inicio (arvore->Nodo),data_inicio);
    int comparacao_fins = comparar_datas (retorna_data_fim (arvore->Nodo),data_fim);

    ////////////////////////////////////////////////////////INICIO DA RESERVA MENOR QUE RANGE///////////////////////////////////
    if (comparacao_inicios == 0)  
    {
       if (comparacao_fins == 0)   //FIM DENTRO 
       {
        lucro += (diferenca_dias (data_inicio,retorna_data_fim (arvore->Nodo))) * atoi (retorna_preco_noite (arvore->Nodo));
            if (comparar_datas (retorna_data_inicio (arvore->Nodo),retorna_data_fim (arvore->Nodo)) == 3) lucro += atoi (retorna_preco_noite (arvore->Nodo));
       } 
   
       else if (comparacao_fins == 1 ) //FIM FORA
       {
        lucro += (diferenca_dias (data_inicio,data_fim)+1) * atoi (retorna_preco_noite (arvore->Nodo));
       } 
   
       else                         //FIM IGUAL
       {
        lucro += (diferenca_dias (data_inicio,data_fim) ) * atoi (retorna_preco_noite (arvore->Nodo));
       }
    }

    ////////////////////////////////////////////////////////INICIO DA RESERVA IGUAL AO RANGE///////////////////////////////////
    else if (comparacao_inicios == 3) 
    {
       if  (comparacao_fins == 0 ) //FIM DENTRO 
       {
        lucro += diferenca_dias (data_inicio,retorna_data_fim (arvore->Nodo))  * atoi (retorna_preco_noite (arvore->Nodo));
            if (comparar_datas (retorna_data_inicio (arvore->Nodo),retorna_data_fim (arvore->Nodo)) == 3) lucro += atoi (retorna_preco_noite (arvore->Nodo));
       } 
   
       else if (comparacao_fins == 1 ) //FIM FORA 
       {
        lucro += (diferenca_dias (data_inicio,data_fim)+1)  * atoi (retorna_preco_noite (arvore->Nodo));
       } 
   
       else //FIM IGUAL 
       {
        lucro += (diferenca_dias (data_inicio,data_fim))  * atoi (retorna_preco_noite (arvore->Nodo));
       } 
    }
    ////////////////////////////////////////////////////////INICIO DA RESERVA MAIOR QUE RANGE///////////////////////////////////
    else 
    {
    if (comparacao_fins == 0) //FIM DENTRO 
    {
     lucro += diferenca_dias (retorna_data_inicio (arvore->Nodo),retorna_data_fim (arvore->Nodo)) * atoi (retorna_preco_noite (arvore->Nodo));
        if (comparar_datas (retorna_data_inicio (arvore->Nodo),retorna_data_fim (arvore->Nodo)) == 3) lucro += atoi (retorna_preco_noite (arvore->Nodo));
    } 
    
    else if (comparacao_fins == 1 ) //FIM FORA 
    {
     lucro += (diferenca_dias (retorna_data_inicio (arvore->Nodo),data_fim)+1) * atoi (retorna_preco_noite (arvore->Nodo));
         if (comparar_datas (retorna_data_inicio (arvore->Nodo),retorna_data_fim (arvore->Nodo)) == 3) lucro += atoi (retorna_preco_noite (arvore->Nodo));
    } 

    else  //FIM IGUAL 
    { 
     lucro += (diferenca_dias (retorna_data_inicio (arvore->Nodo),data_fim)) * atoi (retorna_preco_noite (arvore->Nodo));
        if (comparar_datas (retorna_data_inicio (arvore->Nodo),retorna_data_fim (arvore->Nodo)) == 3) lucro += atoi (retorna_preco_noite (arvore->Nodo));
    }
    }
    
    lucro += (somar_lucro (arvore->esquerda,data_inicio,data_fim) + somar_lucro (arvore->direita,data_inicio,data_fim));
    //soma todos os lucros recursivamente
    return (lucro);   
} 



void calcular_reservas_data (int data[2],btree_registos_reservas** anos_registos_reservas,int numeros_reservas[],int tipo)
{
    int ano = data[0] - 2010;
    int i = 0;
    if (tipo == MES)
    {
        switch (data[1])
        {
            case 1:
                    for (i = 0; i < 31;i++)  numeros_reservas[i] = anos_registos_reservas[ano]->Janeiro[i];
                    break;
            case 2:
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Fevereiro[i];
                    break;
            case 3:
                   for (i = 0; i < 31;i++)  numeros_reservas[i] = anos_registos_reservas[ano]->Marco[i];
                    break;
            case 4:
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Abril[i];
                    break;
            case 5:
                   for (i = 0; i < 31;i++)  numeros_reservas[i] = anos_registos_reservas[ano]->Maio[i];
                    break;
            case 6:
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Junho[i];
                    break;
            case 7:     
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Julho[i];
                    break;
            case 8:
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Agosto[i];
                    break;
            case 9:
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Setembro[i];
                    break;
            case 10:
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Outubro[i];
                    break;
            case 11:
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Novembro[i];
                    break;
            case 12:
                    for (i = 0; i < 31;i++) numeros_reservas[i] = anos_registos_reservas[ano]->Decembro[i];
                    break;
        }
    }
    else if (tipo == ANO)
    {
        numeros_reservas[0] = sum (anos_registos_reservas[ano]->Janeiro,30);
        numeros_reservas[1] = sum (anos_registos_reservas[ano]->Fevereiro,30);
        numeros_reservas[2] = sum (anos_registos_reservas[ano]->Marco,30);
        numeros_reservas[3] = sum (anos_registos_reservas[ano]->Abril,30);
        numeros_reservas[4] = sum (anos_registos_reservas[ano]->Maio,30);
        numeros_reservas[5] = sum (anos_registos_reservas[ano]->Junho,30);
        numeros_reservas[6] = sum (anos_registos_reservas[ano]->Julho,30);
        numeros_reservas[7] = sum (anos_registos_reservas[ano]->Agosto,30);
        numeros_reservas[8] = sum (anos_registos_reservas[ano]->Setembro,30);
        numeros_reservas[9] = sum (anos_registos_reservas[ano]->Outubro,30);
        numeros_reservas[10] = sum (anos_registos_reservas[ano]->Novembro,30);
        numeros_reservas[11] = sum (anos_registos_reservas[ano]->Decembro,30);
    }
    else 
    {
        numeros_reservas[0]  = somar_ano_reservas (anos_registos_reservas[0]);
        numeros_reservas[1]  = somar_ano_reservas (anos_registos_reservas[1]);
        numeros_reservas[2]  = somar_ano_reservas (anos_registos_reservas[2]);
        numeros_reservas[3]  = somar_ano_reservas (anos_registos_reservas[3]);
        numeros_reservas[4]  = somar_ano_reservas (anos_registos_reservas[4]);
        numeros_reservas[5]  = somar_ano_reservas (anos_registos_reservas[5]);
        numeros_reservas[6]  = somar_ano_reservas (anos_registos_reservas[6]);
        numeros_reservas[7]  = somar_ano_reservas (anos_registos_reservas[7]);
        numeros_reservas[8]  = somar_ano_reservas (anos_registos_reservas[8]);
        numeros_reservas[9]  = somar_ano_reservas (anos_registos_reservas[9]);
        numeros_reservas[10] = somar_ano_reservas (anos_registos_reservas[10]);
        numeros_reservas[11] = somar_ano_reservas (anos_registos_reservas[11]);
        numeros_reservas[12] = somar_ano_reservas (anos_registos_reservas[12]);
        numeros_reservas[13] = somar_ano_reservas (anos_registos_reservas[13]);
    }
}


int somar_ano_reservas (btree_registos_reservas* anos_registos_reservas)
{
    int valor = 0;
       valor+= sum (anos_registos_reservas->Janeiro,30);
       valor+= sum (anos_registos_reservas->Fevereiro,30);
       valor+= sum (anos_registos_reservas->Marco,30);
       valor+= sum (anos_registos_reservas->Abril,30);
       valor+= sum (anos_registos_reservas->Maio,30);
       valor+= sum (anos_registos_reservas->Junho,30);
       valor+= sum (anos_registos_reservas->Julho,30);
       valor+= sum (anos_registos_reservas->Agosto,30);
       valor+= sum (anos_registos_reservas->Setembro,30);
       valor+= sum (anos_registos_reservas->Outubro,30);
       valor+= sum (anos_registos_reservas->Novembro,30);
       valor+= sum (anos_registos_reservas->Decembro,30);
    return valor;
}


//---------------------------------------FUNCOES FREE---------------------------------------------------------------------
void free_registos_reservas(btree_registos_reservas** anos_registo_reservas)
{
    int p ;
    for (p = 0; p < 14;p++)
    {
        free (anos_registo_reservas[p]);
    }
    free (anos_registo_reservas);
}

void  free_reserva (reservas_arvore* arvore_r)
    {
        if (arvore_r != NULL)
        {
            free_reserva (arvore_r->esquerda);
            free (arvore_r->Nodo);
            free_reserva (arvore_r->direita);
            free (arvore_r);
        }
    }


void free_arvore_reservas (btree_reservas_utilizador* arvore) 
{
    if (arvore != NULL)
    {
        free_arvore_reservas (arvore->esq);
        free_arvore_reservas (arvore->dir);
    }
    free (arvore);
}


void free_tree_reservas (reservas_arvore** dados_reservas,void* hash_reservas)
    {
        int i = 0;
        free (hash_reservas);
        
        while (i < HASHES_HOTEL)
        {
            free_reserva (dados_reservas[i]);
            i++;
        }
        free (dados_reservas);
    }



void fazer_registo_reserva (int data_inicio_reserva[3],btree_registos_reservas** registos_reservas)
{

        int indice_registo = (data_inicio_reserva[0] - 2010); 
        
        registos_reservas [indice_registo]->Ano_todo++;
        int endereco_dia = data_inicio_reserva[2] - 1;
        if      (data_inicio_reserva[1] == 1) registos_reservas [indice_registo]->Janeiro[endereco_dia]++;
        else if (data_inicio_reserva[1] == 2) registos_reservas [indice_registo]->Fevereiro[endereco_dia]++;
        else if (data_inicio_reserva[1] == 3) registos_reservas [indice_registo]->Marco[endereco_dia]++;
        else if (data_inicio_reserva[1] == 4) registos_reservas [indice_registo]->Abril[endereco_dia]++;
        else if (data_inicio_reserva[1] == 5) registos_reservas [indice_registo]->Maio[endereco_dia]++;
        else if (data_inicio_reserva[1] == 6) registos_reservas [indice_registo]->Junho[endereco_dia]++;
        else if (data_inicio_reserva[1] == 7) registos_reservas [indice_registo]->Julho[endereco_dia]++;
        else if (data_inicio_reserva[1] == 8) registos_reservas [indice_registo]->Agosto[endereco_dia]++;
        else if (data_inicio_reserva[1] == 9) registos_reservas [indice_registo]->Setembro[endereco_dia]++;
        else if (data_inicio_reserva[1] ==10) registos_reservas [indice_registo]->Outubro[endereco_dia]++;
        else if (data_inicio_reserva[1] ==11) registos_reservas [indice_registo]->Novembro[endereco_dia]++;
        else if (data_inicio_reserva[1] ==12) registos_reservas [indice_registo]->Decembro[endereco_dia]++;  
}