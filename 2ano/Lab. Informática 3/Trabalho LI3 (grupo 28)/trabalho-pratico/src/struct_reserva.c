#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include "../include/gestor_reserva.h"
#include "../include/struct_reserva.h"
#include "../include/struct_utilizador.h"
#include "../include/utilidade_tempo.h"
#include "../include/erros.h"
#define HASHES_HOTEL 10000

#define MES 1
#define ANO 2

struct Reserva 
{
    char id_reserva[15];
    char id_utilizador[35];
    char id_hotel[8];
    char nome_hotel[40];
    char estrelas_hotel[2];
    char imposto_cidade[2];
    char morada_hotel[50];
    int data_inicio[3];
    int data_fim[3];
    char preco_noite [6];
    char inclui_pequeno_almoco[15];
    char classificacao[2];
};

//////////////////////////////////////////////////////////////FUNCAO DE PRE PROCESSAMENTO////////////////////////////////////////////////

void* check_hoteis_e_passageiro (char* ficheiro_inputs,GHashTable* verificar_hotel_passageiro,int* verificar_inicial_utilizador)
{
    verificar_hotel_passageiro = g_hash_table_new_full(g_str_hash, g_str_equal, free_key, free_value);

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

        token = strtok_r(NULL," ",&save_tok);
        
        if (q <= 4 || q == 8) 
           {    
             char* detalhes = strdup (token);
             token = strtok_r(NULL," ",&save_tok);
             if (q != 8 && token == NULL) detalhes [strlen (detalhes) - 1] = '\0';

             g_hash_table_insert (verificar_hotel_passageiro,detalhes,"Cheio");
           }
        else if (q == 9)
            {
                 int ASCII = 0; 
                 if (token[0] != '\"') {ASCII =  (int)token[0];}
                 else ASCII =  (int)token[1];
                 if (ASCII < 0) ASCII += 128;

                 verificar_inicial_utilizador [ASCII] = 1;
            }

    }
    fclose (input);
    
    return verificar_hotel_passageiro;
}


reservas_arvore** parser_reserva (char** total_dados_reserva,reservas_arvore** hash_tree,reserva*** hash_reserva,GHashTable* hash_todos_utilizadores,GHashTable* verificar_reserva,int n_reservas,btree_registos_reservas** registos_reservas,int interativo) //guarad todas as reservas em structs especificas
{
    reserva** hash_reservas = *hash_reserva;
    
    
    int contador = 0;
    int tamanho_preenchido = 0;
    while (contador < n_reservas) 
    {
        char dados_reserva [14][126]; 
        char linha_temp[512]; 
        strcpy (linha_temp,total_dados_reserva[contador]);

        dividirLinha (linha_temp,dados_reserva);        //divide os campos de cada reserva
        
        int data_inicio_reserva[3];
        converter_data (dados_reserva[7],data_inicio_reserva);
        fazer_registo_reserva(data_inicio_reserva,registos_reservas);



        
            reserva* reserva = cria_reserva();                                          //criamos e preeenchemos as structs de reserva com todos os dados
            guarda_reserva_T(reserva,dados_reserva[0],dados_reserva[1],dados_reserva[2],dados_reserva[3],dados_reserva[4],dados_reserva[5],dados_reserva[6], 
                             data_inicio_reserva,dados_reserva[8],dados_reserva[9],dados_reserva[10],dados_reserva[11],dados_reserva[12],dados_reserva[13]);
            

            if (interativo == 1 || g_hash_table_contains(verificar_reserva,dados_reserva[1])) //so guardamos se o id do utilizador estiver guardardo
            //pois a isto so é usado para a querie 2 onde precisamos das reservas todas de um utilizador
            {
             double dinheiro = total_price (reserva->data_inicio,reserva->data_fim,reserva->preco_noite,reserva->imposto_cidade);
             char hash_para_reserva [30]; //tamanho para id de utilizador
             strcpy (hash_para_reserva,dados_reserva[1]);
             aumentar_reservas (hash_para_reserva,hash_todos_utilizadores,dinheiro,reserva);
            }


             tamanho_preenchido = atoi (dados_reserva[0] + 4) - 1; //hash por id de reserva



             (hash_reservas[tamanho_preenchido]) = reserva;
     
     
             int hash = atoi (dados_reserva[2] + 3);  //hash por id de Hotel 
             
             if (hash_tree[hash] == NULL) { hash_tree[hash] = inserir_reserva (hash_tree[hash],reserva);}
             else { hash_tree[hash] = inserir_reserva (hash_tree[hash],reserva);}

        contador++;
     }
     
     *hash_reserva = hash_reservas;
     return hash_tree;
}

int* retorna_data_inicio (reserva* r)
{
    return (r->data_inicio);
}

int* retorna_data_fim (reserva* r)
{
    return (r->data_fim);
}

char* retorna_id_reserva (reserva* r)
{
    return (r->id_reserva);
}

char* retorna_classificacao_reserva (reserva* r)
{
    return (r->classificacao);
}

char* retorna_id_utilizador_res (reserva* r)
{
    return (r->id_utilizador);
}

char* retorna_preco_noite (reserva* r)
{
    return r->preco_noite;
}

char* retorna_imposto_cidade (reserva* r)
{
    return r->imposto_cidade;
}


reserva* cria_reserva() 
{
    reserva* nova_res = malloc(sizeof(struct Reserva));
    nova_res->data_inicio [0] = 0 ;
    nova_res->data_inicio [1] = 0 ;
    nova_res->data_inicio [2] = 0 ;
    nova_res->data_fim [0] = 0 ;
    return nova_res;
}

reserva* guarda_reserva_T (reserva* Nova_reserva ,char* id_reserva,char* id_utilizador,char* id_hotel,char* nome_hotel,char* estrelas_hotel,char* imposto_cidade,char* morada_hotel
                         ,int data_inicio[3],char* data_fim,char* preco_noite,char* inclui_pequeno_almoco,char* detalhes_quarto,char* classificacao,char* comentario) 
{

        strcpy (Nova_reserva->id_reserva,id_reserva);
        strcpy (Nova_reserva->id_utilizador,id_utilizador);

        
        strcpy (Nova_reserva->id_hotel,id_hotel);
       
        strcpy (Nova_reserva->nome_hotel,nome_hotel);
       
        strcpy (Nova_reserva->estrelas_hotel,estrelas_hotel);
        
        strcpy (Nova_reserva->imposto_cidade,imposto_cidade);
      
        strcpy ( Nova_reserva->morada_hotel,morada_hotel);

        Nova_reserva->data_inicio[0] = data_inicio[0];
        Nova_reserva->data_inicio[1] = data_inicio[1];
        Nova_reserva->data_inicio[2] = data_inicio[2];

        converter_data (data_fim,Nova_reserva->data_fim);
        
      
        strcpy (Nova_reserva->preco_noite,preco_noite);
     
        strcpy (Nova_reserva->inclui_pequeno_almoco,inclui_pequeno_almoco);
      
        strcpy (Nova_reserva->classificacao,classificacao);

        return Nova_reserva; 
}


int compara_ids (char* id1,char* id2) //retorna 1 se id1 for o maior id
{
    if (atoi (id1+4) > atoi (id2+4)) 
    {
        return 1;
    }
    else return 0;
}





//-----------------------------------------------------------Querie 1---------------------------------------------------------

void querie1_R_interativa (reserva** hash_reserva,char* id,char resposta[1000],int F)
{
    int select = atoi(id + 4) - 1;
    if (hash_reserva[select] == NULL) 
    {
        strcpy (resposta,"Reserva nao existe");
        return;
    }
    
    int noites = diferenca_dias (hash_reserva[select]->data_inicio,hash_reserva[select]->data_fim) ;
    double custo = 0.000;
    custo = total_price (hash_reserva[select]->data_inicio,hash_reserva[select]->data_fim,hash_reserva[select]->preco_noite,hash_reserva[select]->imposto_cidade);
    
    hash_reserva [select]->inclui_pequeno_almoco[0] = toupper (hash_reserva [select]->inclui_pequeno_almoco[0]);
    
    if (hash_reserva[select]->inclui_pequeno_almoco[0] == 'T' && strlen (hash_reserva[select]->inclui_pequeno_almoco) == 1) 
    {
        strcpy(hash_reserva[select]->inclui_pequeno_almoco,"True");
    }   
    
    if (F == 0)
    {
    sprintf (resposta,"%s;%s;%s;%d/%02d/%02d;%d/%02d/%02d;%s;%d;%.3f\n",
    hash_reserva [select]->id_hotel,
    hash_reserva [select]->nome_hotel,
    hash_reserva [select]->estrelas_hotel,
    hash_reserva [select]->data_inicio[0],
    hash_reserva [select]->data_inicio[1],
    hash_reserva [select]->data_inicio[2],
    hash_reserva [select]->data_fim[0],
    hash_reserva [select]->data_fim[1],
    hash_reserva [select]->data_fim[2],
    hash_reserva [select]->inclui_pequeno_almoco,
    noites,
    custo);
    }
    else
    {
    sprintf(resposta,"--- 1 ---\nhotel_id: %s\nhotel_name: %s\nhotel_stars: %s\nbegin_date: %d/%02d/%02d\nend_date: %d/%02d/%02d\nincludes_breakfast: %s\nnights: %d\ntotal_price: %.3f\n",
    hash_reserva [select]->id_hotel,
    hash_reserva [select]->nome_hotel,
    hash_reserva [select]->estrelas_hotel,
    hash_reserva [select]->data_inicio[0],
    hash_reserva [select]->data_inicio[1],
    hash_reserva [select]->data_inicio[2],
    hash_reserva [select]->data_fim[0],
    hash_reserva [select]->data_fim[1],
    hash_reserva [select]->data_fim[2],
    hash_reserva [select]->inclui_pequeno_almoco,
    noites,
    custo);
    }
}   

void print_querie_1_R (reserva** hash_reserva,char* id,FILE* ficheiro_output,int F)
{
    int select = atoi(id + 4) - 1;
    if (hash_reserva[select] == NULL) return;
    
    int noites = diferenca_dias (hash_reserva[select]->data_inicio,hash_reserva[select]->data_fim) ;
    double custo = 0.000;
    custo = total_price (hash_reserva[select]->data_inicio,hash_reserva[select]->data_fim,hash_reserva[select]->preco_noite,hash_reserva[select]->imposto_cidade);
    
    hash_reserva [select]->inclui_pequeno_almoco[0] = toupper (hash_reserva [select]->inclui_pequeno_almoco[0]);
    
    if (hash_reserva[select]->inclui_pequeno_almoco[0] == 'T' && strlen (hash_reserva[select]->inclui_pequeno_almoco) == 1) 
    {
        strcpy(hash_reserva[select]->inclui_pequeno_almoco,"True");
    }   
    
    if (F == 0)
    {
    fprintf(ficheiro_output,"%s;%s;%s;%d/%02d/%02d;%d/%02d/%02d;%s;%d;%.3f\n",
    hash_reserva [select]->id_hotel,
    hash_reserva [select]->nome_hotel,
    hash_reserva [select]->estrelas_hotel,
    hash_reserva [select]->data_inicio[0],
    hash_reserva [select]->data_inicio[1],
    hash_reserva [select]->data_inicio[2],
    hash_reserva [select]->data_fim[0],
    hash_reserva [select]->data_fim[1],
    hash_reserva [select]->data_fim[2],
    hash_reserva [select]->inclui_pequeno_almoco,
    noites,
    custo);
    }
    else
    {
    fprintf(ficheiro_output,"--- 1 ---\nhotel_id: %s\nhotel_name: %s\nhotel_stars: %s\nbegin_date: %d/%02d/%02d\nend_date: %d/%02d/%02d\nincludes_breakfast: %s\nnights: %d\ntotal_price: %.3f\n",
    hash_reserva [select]->id_hotel,
    hash_reserva [select]->nome_hotel,
    hash_reserva [select]->estrelas_hotel,
    hash_reserva [select]->data_inicio[0],
    hash_reserva [select]->data_inicio[1],
    hash_reserva [select]->data_inicio[2],
    hash_reserva [select]->data_fim[0],
    hash_reserva [select]->data_fim[1],
    hash_reserva [select]->data_fim[2],
    hash_reserva [select]->inclui_pequeno_almoco,
    noites,
    custo);
    }
}   


void  free_hash_reservas_essenciais (GHashTable* hash_verificacao)
{
    g_hash_table_destroy (hash_verificacao);
}

void free_key(gpointer key) {
    g_free(key);
}

void free_value(gpointer value) {
   return;
}






