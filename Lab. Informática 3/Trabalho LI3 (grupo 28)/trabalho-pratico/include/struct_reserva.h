#ifndef struct_reserva_h
#define struct_reserva_h
#include <stdio.h>
#include <stdlib.h>
#include "../include/gestor_reserva.h"

typedef struct Reserva reserva;

reservas_arvore** parser_reserva (char** total_dados_reserva,reservas_arvore** hash_tree,reserva*** hash_reserva
                                    ,GHashTable* hash_todos_utilizadores,GHashTable* verificar_reserva,int n_reservas
                                    ,btree_registos_reservas** registos_reservas,int interativo);


////////////////////////////////////////////////////////////////////// PRE PROCESSAMENTO //////////////////////////////////////////////

void* check_hoteis_e_passageiro (char* ficheiro_inputs,GHashTable* verificar_hotel_passageiro,int* verificar_inicial_utilizador);

/////////////////////////////////////////// GETTERS //////////////////////////////////////////

int* retorna_data_inicio (reserva* r);
int* retorna_data_fim (reserva* r);
char* retorna_id_reserva (reserva* r);
char* retorna_classificacao_reserva (reserva* r);
char* retorna_id_utilizador_res (reserva* r);
char* retorna_preco_noite (reserva* r);
char* retorna_imposto_cidade (reserva* r);

//////////////////////////////////////////////////// INICIALIZACAO DE RESERVAS //////////////////////////////////////
reserva* cria_reserva();
reserva* guarda_reserva_T (reserva* Nova_reserva ,char* id_reserva,char* id_utilizador,char* id_hotel,char* nome_hotel,char* estrelas_hotel,char* imposto_cidade,char* morada_hotel
                         ,int data_inicio[3],char* data_fim,char* preco_noite,char* inclui_pequeno_almoco,char* detalhes_quarto,char* classificacao,char* comentario);


//////////////////////////////////////// QUERIE 1 AUXILIARES ///////////////////////////////////////////////

void querie1_R_interativa (reserva** hash_reserva,char* id,char resposta[1000],int F);
void print_querie_1_R (reserva** hash_reserva,char* id,FILE* ficheiro_output,int F);

//AUXILIAR
int compara_ids (char* id1,char* id2);

//////////////////////////////////////// FREES ///////////////////////////////////////////////

void  free_hash_reservas_essenciais (GHashTable* hash_verificacao);
void free_key(gpointer key);
void free_value(gpointer value);




#endif