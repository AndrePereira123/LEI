#ifndef struct_utilizador_h
#define struct_utilizador_h
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "../include/gestor_utilizador.h"


typedef struct Utilizador utilizador;



void* parser_utilizadores(char** total_dados_utilizador,GHashTable* lista_todos_utilizadores,btree_utilizadores** hash_table_inicial
                         ,int n_utilizadores,btree_registos_ano** registos,int* verificar_inicial_utilizador,GHashTable* apontador_verificar_utilizador
                        ,int interativo);

/////////////////////////////////////////////////////////////// PRE PROCESSAMENTO ////////////////////////////////////////////////

void* check_utilizadores (char* ficheiro_inputs,GHashTable* verifcar_utilizador);

///////////////////////////////////////////////////////////// GETTERS ///////////////////////////////////////////////////////

char* retorna_nome (btree_utilizadores* arvore);
void* retorna_arvore_voos(utilizador* user);
void* retorna_arvore_voos_mae (utilizador* user);
void* retorna_arvore_reservas(utilizador* user);
void* retorna_arvore_voos_mae (utilizador* user);
char* retorna_nome_utilizador (utilizador* user);
char* retorna_id_utilizador (utilizador* user);
void converter_criacao_conta (char letras[],int data[]);

///////////////////////////////////////////////////////////// INICIALIZACAO DE UTILIZADORES ////////////////////////////////////////

utilizador* cria_utilizador ();

utilizador* guarda_utilizador_T (utilizador* Novo_utilizador,char* id,char* nome,char* email,char* telemovel,char* nascimento,
                                         char* sexo,char* passaporte,char* codigo_pais,char* morada,char* criacao_conta,char* metodo_pagamento,char* estado_conta);

// AUXILIAR
int compara_nomes (char* nome1,char* nome2);

/////////////////////////////////////////////////////// AUXILIARES DE PARSING ///////////////////////////////////////////////

void aumentar_reservas (char hash_para_reserva[],GHashTable* hash_todos_utilizadores,double dinheiro,void* reserva_nova);
void aumentar_numero_voos (char hash_para_voo[],GHashTable* hash_todos_utilizadores);

////////////////////////////////////////////////////// QUERIE 1 ////////////////////////////////////////////////////////////

void  querie1_U_interativa (GHashTable* hash_todos_utilizadores,char* hash,char* resposta,int F);
void print_querie_1_U (GHashTable* hash_todos_utilizadores,char* hash,FILE* ficheiro_output,int F);

////////////////////////////////////////////////////// QUERIE 2 ////////////////////////////////////////////////////////////

void querie_2_Reservas (char* username,GHashTable* dados_utilizadores_id,FILE *ficheiro_output,int F);
void querie_2_Reservas_interativo (char* username,GHashTable* dados_utilizadores_id,char* resposta,int F);

///////////////////////////////////////////// QUERIE 9 AUXILIAR ///////////////////////////////////////

int verificar_prefixo (char* nome, char* prefixo);

/////////////////////////////////////////////// FREES ////////////////////////////////////////////////////

void free_g_hash_utilizadores (GHashTable* lista_inativos);
void free_hash_utilizadores_inicial (btree_utilizadores** hash_por_inicial);
void free_erros(GHashTable* lista_erros);
void free_utilizador (utilizador* utilizador);
void free_single_tree (btree_utilizadores* arvore);



#endif