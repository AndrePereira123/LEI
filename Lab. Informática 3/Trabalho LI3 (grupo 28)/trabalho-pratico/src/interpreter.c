#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <sys/resource.h>
#include "../include/struct_reserva.h"
#include "../include/struct_utilizador.h"
#include "../include/gestor_passageiro.h"
#include "../include/struct_voos.h"
#include "../include/queries.h"
#include "../include/interpreter.h"


void interpreter(btree_registos_ano** anos_registos_utilizadores,btree_registos_reservas** anos_registos_reservas,btree_registos_voos** anos_registos_voos,aeroporto_arvore* arvore_todos_voos,char** matriz_passageiros,int linhas_matriz,reservas_arvore** total_dados_reservas,reserva** hash_reservas, btree_utilizadores** arvores_por_inicial,GHashTable* dados_utilizadores_id,GHashTable* total_dados_voos,voo** hash_voos,int* passageiros_voo,int n_voos,char* caminho_inputs)
{
    
    FILE* comandos = fopen(caminho_inputs,"r"); //abrimos o ficheiro de inputs
    char linha[1024];
    int output = 0;

    while (fgets (linha,sizeof(linha),comandos))
    {
        output++;
        char querie[10];
        char detalhes[100];         //dividimos o conteudo da linha na querie e nos detalhes dados para a realizacao dela
    
        char* token;            
        char* save_tok;
        
        token = strtok_r(linha," ",&save_tok);
        strcpy (querie,token);
    
        token = strtok_r(NULL,"\n",&save_tok);
        if (token != NULL && strcmp(token,"") != 0) 
        {
            strcpy (detalhes,token);
        }
        else 
        {
            detalhes [0] = 'v';
        }
        choose_queries(anos_registos_utilizadores,anos_registos_reservas,anos_registos_voos,arvore_todos_voos,matriz_passageiros,linhas_matriz,querie
                    ,detalhes,total_dados_reservas,hash_reservas,arvores_por_inicial,dados_utilizadores_id,total_dados_voos,passageiros_voo
                    ,hash_voos,n_voos,output);
        //chamamos a funcao choose_queries com todos os dados necessarios
    }
    fclose (comandos);
}


void interpreter_testes (btree_registos_ano** anos_registos_utilizadores,btree_registos_reservas** anos_registos_reservas
                            ,btree_registos_voos** anos_registos_voos,aeroporto_arvore* arvore_todos_voos,char** matriz_passageiros
                            ,int linhas_matriz,reservas_arvore** total_dados_reservas,reserva** hash_reservas, btree_utilizadores** arvores_por_inicial
                            ,GHashTable* dados_utilizadores_id,GHashTable* total_dados_voos,voo** hash_voos,int* passageiros_voo,int n_voos
                            ,char* caminho_inputs,char* ficheiros_corretos)
{
    {
    
    FILE* comandos = fopen(caminho_inputs,"r"); //abrimos o ficheiro de inputs
    char linha[1024];
    int output = 0;
    int erradas = 0;

    while (fgets (linha,sizeof(linha),comandos))
    {
        output++;
        char querie[10];
        char detalhes[100];         //dividimos o conteudo da linha na querie e nos detalhes dados para a realizacao dela
    
        char* token;            
        char* save_tok;
        
        token = strtok_r(linha," ",&save_tok);
        strcpy (querie,token);
    
        token = strtok_r(NULL,"\n",&save_tok);
        if (token != NULL && strcmp(token,"") != 0) 
        {
            strcpy (detalhes,token);
        }
        else 
        {
            detalhes [0] = 'v';
        }
        choose_queries_testes(anos_registos_utilizadores,anos_registos_reservas,anos_registos_voos,arvore_todos_voos,matriz_passageiros,linhas_matriz,querie
                        ,detalhes,total_dados_reservas,hash_reservas,arvores_por_inicial,dados_utilizadores_id,total_dados_voos,passageiros_voo
                        ,hash_voos,n_voos,output,ficheiros_corretos,&erradas);
        //chamamos a funcao choose_queries com todos os dados necessarios
    }
    printf ("Numero de Queries Erradas : %d\n------------------------------------------------------------------------\n",erradas);
  

    fclose (comandos);
}
}