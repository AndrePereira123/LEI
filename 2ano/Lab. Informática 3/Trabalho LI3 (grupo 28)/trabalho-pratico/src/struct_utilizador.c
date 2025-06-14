#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include "../include/gestor_utilizador.h"
#include "../include/struct_utilizador.h"
#include "../include/struct_reserva.h"
#include "../include/gestor_passageiro.h"
#include "../include/utilidade_tempo.h"


#define ERRO 0
#define CERTO 1
#define MES 1
#define ANO 2
#define HASH_ASCII 300

struct  Utilizador {
    char id[35];
    char nome[40];
    char nascimento[15];
    char sexo[2];
    char passaporte[15];
    char codigo_pais[3];
    int criacao_conta[3];
    char estado_conta[15];
    int numero_reservas;
    double total_gasto;
    int voos;
    btree_reservas_utilizador* arvore_reservas;
    btree_id_voos* id_voos;
};

//////////////////////////////////////////////////////////////FUNCAO DE PRE PROCESSAMENTO////////////////////////////////////////////////


void* check_utilizadores (char* ficheiro_inputs,GHashTable* verifcar_utilizador)
{
    verifcar_utilizador = g_hash_table_new_full(g_str_hash, g_str_equal, free_key, free_value);

    FILE* input = fopen(ficheiro_inputs,"r"); //abrimos o ficheiro de inputs
    char linha[512];
    while (fgets (linha,512,input))
    {
        char querie[10];        
    
        char* token;            
        char* save_tok;
        
        token = strtok_r(linha," ",&save_tok);
        strcpy (querie,token);

        int q = atoi (querie);
        
        if (q == 2) 
           {    
             token = strtok_r(NULL," ",&save_tok);
             char* detalhes = g_strdup (token);
             token = strtok_r(NULL," ",&save_tok);
             if (token == NULL) detalhes [strlen (detalhes) -1] = '\0';

             g_hash_table_add (verifcar_utilizador,detalhes);
           }
        else if (q == 1)
        {
            token = strtok_r(NULL,"\n",&save_tok);
            g_hash_table_add (verifcar_utilizador,g_strdup (token));
        } 

    }
    fclose (input);
    return verifcar_utilizador;
}




void* parser_utilizadores(char** total_dados_utilizador,GHashTable* lista_todos_utilizadores,btree_utilizadores** hash_table_inicial
                            ,int n_utilizadores,btree_registos_ano** registos,int* verificar_inicial_utilizador,GHashTable* apontador_verificar_utilizador
                            ,int interativo)
{
   
    int contador = 0;
    int i;
    lista_todos_utilizadores = g_hash_table_new(g_str_hash,g_str_equal);
    while (contador < n_utilizadores)
    {
        i = 0;
        char* token;
        char* save_token;
        char linha_temp [512];
        strcpy(linha_temp,total_dados_utilizador[contador]);
        token = strtok_r(linha_temp,";",&save_token);

        int ASCII = (int)token[0];
        if (ASCII < 0) ASCII += 128;
 

        char dados_utilizador[12][150];
        while (token != NULL)
        {
            strcpy(dados_utilizador[i],token);
            token = strtok_r(NULL,";",&save_token);
            i++;
        }

        for(int m = 0; dados_utilizador[11][m] != '\0'; m++)
        {
          dados_utilizador[11][m] = tolower(dados_utilizador[11][m]);  //homogenizamos o estado da conta para minuscula sempre
        }

        char* hash = malloc (strlen (dados_utilizador[0]) + 1);
        strcpy (hash,dados_utilizador[0]);
        
    

        if(strcmp(dados_utilizador[11],"active\n") == 0 && (interativo == 1 || ( (verificar_inicial_utilizador[ASCII] == 1) || g_hash_table_contains (apontador_verificar_utilizador,hash) )) )
                                                                   //verifica se é necessario para a querie 9   OU  necessario para a querie 1 ou 2     )
        {

        utilizador* novo_utilizador = cria_utilizador(); //criamos o utilizador e guardamos todos os dados nele
        guarda_utilizador_T(novo_utilizador,dados_utilizador[0],dados_utilizador[1],dados_utilizador[2],dados_utilizador[3],dados_utilizador[4],dados_utilizador[5],dados_utilizador[6], 
                         dados_utilizador[7],dados_utilizador[8],dados_utilizador[9],dados_utilizador[10],dados_utilizador[11]);
        

        int indice_registo = (novo_utilizador->criacao_conta[0] - 2010);
        int data[2];
        data [0] =  novo_utilizador->criacao_conta[1]; data [1] =  novo_utilizador->criacao_conta[2] - 1;
        inserir_registo_utilizador (indice_registo,registos,data);


        if (verificar_inicial_utilizador[ASCII] == 1)
        {
        if (hash_table_inicial[ASCII] == NULL)
            {
                btree_utilizadores* arvore = NULL;
                arvore = inserir_utilizador(arvore,novo_utilizador);
                hash_table_inicial[ASCII] = arvore;
            }
            else 
                {
                    btree_utilizadores* arvore = hash_table_inicial[ASCII];
                    arvore = inserir_utilizador(arvore,novo_utilizador);
                    hash_table_inicial[ASCII] = arvore;
                }
        }
     

        g_hash_table_insert (lista_todos_utilizadores,hash,novo_utilizador); //ativos
        }

        else 
        {

        int criacao_conta[3];
        converter_criacao_conta (dados_utilizador[9],criacao_conta);
        

        int indice_registo = (criacao_conta[0] - 2010);
        int data[2];
        data [0] =  criacao_conta[1]; data [1] =  criacao_conta[2] - 1;
        inserir_registo_utilizador (indice_registo,registos,data);
        free (hash);

        }

        contador++;
    }

    return lista_todos_utilizadores;
}

utilizador* cria_utilizador()
{
    utilizador* novo_utilizador = malloc(sizeof(struct Utilizador));
    novo_utilizador->numero_reservas = 0;
    novo_utilizador->total_gasto = 0.000;
    novo_utilizador->voos = 0;
    novo_utilizador->arvore_reservas = NULL;
    novo_utilizador->id_voos = NULL;
    return novo_utilizador;
}


utilizador* guarda_utilizador_T (utilizador* Novo_utilizador,char* id,char* nome,char* email,char* telemovel,char* nascimento,
                                         char* sexo,char* passaporte,char* codigo_pais,char* morada,char* criacao_conta,char* metodo_pagamento,char* estado_conta) 
{

        strcpy (Novo_utilizador->id,id);

        strcpy (Novo_utilizador->nome,nome);

        strcpy (Novo_utilizador->nascimento,nascimento);

        strcpy (Novo_utilizador->sexo,sexo);

        strcpy(Novo_utilizador->passaporte,passaporte);
        
        strcpy (Novo_utilizador->codigo_pais,codigo_pais);

        converter_criacao_conta (criacao_conta,Novo_utilizador->criacao_conta);

        strcpy (Novo_utilizador->estado_conta,estado_conta);

        return Novo_utilizador;
}

void converter_criacao_conta (char letras[],int data[])
{
    char* token;
    char* save_tok;
    token = strtok_r(letras,"/",&save_tok);  //pegar ano
    data[0] = atoi (token);
    token = strtok_r(NULL,"/",&save_tok); //pegar mes
    data[1] = atoi (token);
    token = strtok_r(NULL," ",&save_tok); //pegar o dia
    data[2] = atoi (token);
}

//Compara Nomes
int compara_nomes (char* nome1,char* nome2)
{
   int res = strcoll(nome1,nome2);
    if(res == 0)
    {
        return 3; // São iguais
    }
    else if(res > 0)
    {
        return 1; //nome1 > nome2
    }
    else
    {
        return 2; //nome2 > nome1
    }
}



void aumentar_reservas (char hash_para_reserva[],GHashTable* hash_todos_utilizadores,double dinheiro,void* reserva_nova)
 {

    utilizador* aumentar = g_hash_table_lookup (hash_todos_utilizadores,hash_para_reserva);
    if (aumentar == NULL) return;
    inserir_reserva_utilizador (reserva_nova,&(aumentar->arvore_reservas));
    aumentar->numero_reservas++;
    aumentar->total_gasto += dinheiro;
 }

void aumentar_numero_voos (char hash_para_voo[],GHashTable* hash_todos_utilizadores)
{  
    utilizador* aumentar = g_hash_table_lookup (hash_todos_utilizadores,hash_para_voo);
    if (aumentar!= NULL) aumentar->voos++;
}

/////////////////////////////////////////////////////////////QUERIE 1//////////////////////////////////////////////////

void  querie1_U_interativa (GHashTable* hash_todos_utilizadores,char* hash,char* resposta,int F)
{
     if (g_hash_table_lookup (hash_todos_utilizadores,hash) == NULL ) 
     {
        sprintf (resposta,"O utilizador nao existe\n");
        return;
     }
    utilizador* user = g_hash_table_lookup (hash_todos_utilizadores,hash);
    
    int idade = c_idade (user->nascimento); 
    if (F == 0)
    {
    sprintf (resposta,"%s;%s;%d;%s;%s;%d;%d;%.3f\n",user->nome,user->sexo,idade,user->codigo_pais,user->passaporte,
                                                        user->voos,user->numero_reservas,user-> total_gasto);
    }
    else 
    {
    sprintf (resposta,"--- 1 ---\nname: %s\nsex: %s\nage: %d\ncountry_code: %s\npassport: %s\nnumber_of_flights: %d\nnumber_of_reservations: %d\ntotal_spent: %.3f\n",
                                                        user->nome,user->sexo,idade,user->codigo_pais,user->passaporte,
                                                        user->voos,user->numero_reservas,user->total_gasto);
    }
}


void print_querie_1_U (GHashTable* hash_todos_utilizadores,char* hash,FILE* ficheiro_output,int F)
{   
    if (g_hash_table_lookup (hash_todos_utilizadores,hash) == NULL) return;
    utilizador* user = g_hash_table_lookup (hash_todos_utilizadores,hash);
    
    int idade = c_idade (user->nascimento); 
    if (F == 0)
    {
    fprintf (ficheiro_output,"%s;%s;%d;%s;%s;%d;%d;%.3f\n",user->nome,user->sexo,idade,user->codigo_pais,user->passaporte,
                                                        user->voos,user->numero_reservas,user-> total_gasto);
    }
    else 
    {
    fprintf (ficheiro_output,"--- 1 ---\nname: %s\nsex: %s\nage: %d\ncountry_code: %s\npassport: %s\nnumber_of_flights: %d\nnumber_of_reservations: %d\ntotal_spent: %.3f\n",
                                                        user->nome,user->sexo,idade,user->codigo_pais,user->passaporte,
                                                        user->voos,user->numero_reservas,user->total_gasto);
    }
}


//-----------------------------------------------------------Querie 2----------------------------------------------



void querie_2_Reservas (char* username,GHashTable* dados_utilizadores_id,FILE *ficheiro_output,int F)
{
    utilizador* user = g_hash_table_lookup (dados_utilizadores_id,username);
        int contador = 1;
        if(F == 1) {print_reservas_q2F(user->arvore_reservas,ficheiro_output,&contador);}
        else print_reservas_q2 (user->arvore_reservas,ficheiro_output);
}

void querie_2_Reservas_interativo (char* username,GHashTable* dados_utilizadores_id,char* resposta,int F)
 {
        if (! g_hash_table_contains (dados_utilizadores_id,username)) 
        {
            strcpy (resposta,"Esse utilizador nao existe\n");
            return;
        }
        utilizador* user = g_hash_table_lookup (dados_utilizadores_id,username);
        int contador = 1;
        print_reservas_q2_interativo(user->arvore_reservas,resposta,&contador,F);
 }


void* retorna_arvore_voos(utilizador* user)
{
    return user->id_voos;
}

void* retorna_arvore_voos_mae (utilizador* user)
{
    return &(user->id_voos);
}

char* retorna_nome_utilizador (utilizador* user)
{
    return user->nome;
}

char* retorna_id_utilizador (utilizador* user)
{
    return user->id;
}


void* retorna_arvore_reservas(utilizador* user)
{
    if (user->arvore_reservas == NULL) return NULL;
    return (user->arvore_reservas);
}


//////////////////////////////////////////////////////////////////////AUXILIAR QUERIE 9//////////////////////////////////////////////


int verificar_prefixo(char* nome, char* prefixo) 
{          //verifica se o o nome começa pelo prefixo
    int tamanho_prefixo = strlen(prefixo);

    if (prefixo[0] == '"') {
        tamanho_prefixo -= 2;
        for (int i = 0; i < tamanho_prefixo; i++) {
            prefixo[i] = prefixo[i + 1];                //tiramos aspas do prefixo 
        }
        prefixo[tamanho_prefixo] = '\0';
    }

    char prefixo_nome[50];
    strcpy(prefixo_nome,nome);
    prefixo_nome[tamanho_prefixo] = '\0';       //copiamos o prefixo do nome

    int res = strcoll(prefixo_nome, prefixo); //guardamos a comparacao

    if (res == 0) 
    {                       //se o nome comeca com o prefixo resulta 0;
        return 0;
    }
    if (strcoll (nome,prefixo) > 0)             //se o nome for maior que o prefixo 
    {
    if (res > 0) return 1;      // se o nome é maior por ter um prefixo inicial maior
    return 4; // se o nome é maior so por ser mais longo ou seja mesmo prefixo inicial 
    }
        return -1; //se o nome é menor que o prefixo
}





///////////////////////////////////////////frees//////////////////////////////////////////////////////

void free_erros(GHashTable* lista_erros)
{

    GHashTableIter iter;
    g_hash_table_iter_init(&iter, lista_erros);

    gpointer key, value;
    while (g_hash_table_iter_next(&iter, &key, &value)) 
    {
        g_free(key);
    }
   
    g_hash_table_destroy (lista_erros);
}

void free_utilizador (utilizador* utilizador)
{
    free_arvore_id_voos(utilizador->id_voos);
    free_arvore_reservas (utilizador->arvore_reservas);
    free (utilizador);
}    

void free_g_hash_utilizadores (GHashTable* hash_todos_utilizadores)
{
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, hash_todos_utilizadores);
   

    gpointer key, value;
    while (g_hash_table_iter_next(&iter, &key, &value)) 
    {
        free_utilizador (value);
        g_free (key);
    }

    g_hash_table_destroy (hash_todos_utilizadores);
}



