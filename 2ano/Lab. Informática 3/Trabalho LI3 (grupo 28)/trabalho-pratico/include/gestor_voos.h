#ifndef gestor_voos_h
#define gestor_voos_h
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>


typedef struct AEROPORTO_ARVORE aeroporto_arvore;
typedef struct BTREE_REGISTOS_VOOS btree_registos_voos;

/////////////////////////////////////////////////////////////// INICIALIZACAO ////////////////////////////////////////////////

btree_registos_voos** criar_lista_registos_voos();
btree_registos_voos* criar_ano_voos ();
aeroporto_arvore* criar_aeroporto_vazio();


/////////////////////////////////////////////////////////////////// INSERCOES /////////////////////////////////////////////

aeroporto_arvore* inserir_voo (aeroporto_arvore* arvore, void* Nodo_novo,int* passageiros_voo,int t);
aeroporto_arvore* inserir_voo_simple (aeroporto_arvore* arvore, void* Nodo_novo,int* passageiros_voo,int t);
aeroporto_arvore* contar_destination (aeroporto_arvore* arvore,void* voo_chegada,int* passageiros_voo,int t);


///////////////////////////////////////////////// QUERIE 5 ///////////////////////////////////////////////////////

void querie5_interativa (aeroporto_arvore* arvore_o,char* resposta,int data_inicio[6],int data_fim[6],int* contador,int F);
void print_querie_5 (aeroporto_arvore* arvore_o,FILE* ficheiro_output,int data_inicio[6],int data_fim[6],int* contador,int F);

///////////////////////////////////////////////// QUERIE 6 ///////////////////////////////////////////////////////

void querie6_interativa (GHashTable* hash_tree,int ano,int N,char* resposta,int F);
void print_querie_6(GHashTable* hash_tree,int ano, int N,FILE* ficheiro_output,int F);

///////////////////////////////////////////////// QUERIE 7 ///////////////////////////////////////////////////////

void print_querie_7 (GHashTable* hash_tree,int N,FILE *ficheiro_output,int n_voos,int F);
void querie7_interativa (GHashTable* hash_tree,int N,char* resposta,int n_voos,int F);

int fazer_mediana_aeroporto (aeroporto_arvore* aeroporto,int n_voos);
void inserir_atraso_lista (aeroporto_arvore* aeroporto,int* array,int* preenchido,size_t* tamanho_array);


//////////////////////////////////////////////////////// QUERIE 10 AUXILIARES ///////////////////////////////////////////

void inserir_registo_voo (int* data,btree_registos_voos** registos_voos);
void encontrar_voos_data (int data[2],aeroporto_arvore* arvore_todos_voos,int* voos_a_contar,int* tempo_do_voo,int tipo);
void encontrar_voos_data_todos (int data[2],aeroporto_arvore* arvore_todos_voos,int* ano_dos_voos);
void calcular_voos_data (int data[2],btree_registos_voos** anos_registos_voos,int numeros_voos[],int tipo);
int somar_ano_voos (btree_registos_voos* anos_registos_voos);

///////////////////////////////////////////////// FREES ///////////////////////////////////////////////////////

void free_aeroporto (aeroporto_arvore* aeroporto);
void free_aeroporto_lite (aeroporto_arvore* aeroporto);
void free_registos_voos(btree_registos_voos** anos_registo_voos);


#endif