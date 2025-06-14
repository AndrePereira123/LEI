#ifndef struct_voos_h
#define struct_voos_h
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

typedef struct VOO voo;
typedef struct AEROPORTO_ARVORE aeroporto_arvore;
typedef struct BTREE_REGISTOS_VOOS btree_registos_voos;


void* parser_voos (char** total_dados_voos,GHashTable* hash_tree,int* passageiros_voo, voo*** hash_voos,int n_voos
                    ,btree_registos_voos** registos,aeroporto_arvore** arvore_todos_voos);

///////////////////////////////////////////////// INICIALIZACAO DE VOOS //////////////////////////////////////////////////

voo* cria_voo();
voo* guarda_voo_T (voo* novo_voo ,char* id,char* airline,char* model,char* total_seats,char* origin,char* destination,char* schedule_departure_date
                         ,char* schedule_arrival_date,char* real_departure_date,char* real_arrival_date,char* pilot,char* copilot,char* notes);

///////////////////////////////////////////////// GETTERS //////////////////////////////////////////////////

int* retorna_s_arrival_date (voo* v);
int* retorna_s_departure_date (voo* v);
char* retorna_destination (voo* v);
char* retorna_origin (voo* v);
char* retorna_airline (voo* voo_novo);
char* retorna_id_char (voo* voo_novo);
char* retorna_model (voo* voo_novo);
int* retorna_r_departure_date (voo* v);
int retorna_id (voo* voo_novo);
void departure_date (voo* viagem,int data_saida[6]);
void departure_date_simple (voo* viagem,int data_saida[3]);
void converter_schedule (char letras[],int data[]);

////////////////////////////////////////// AUXILIARES ///////////////////////////////////////////

int compara_ids_voo (char* id1,char* id2);
voo* hash_do_voo (int id_novo_voo,voo** hash_voos);
int compara_data_voos(int data_inicial[6], int data_final[6]);

///////////////////////////////////////////////// QUERIE 1 //////////////////////////////////////////////////

void print_querie_1_V (voo** hash_voos,int* passageiros_voo,char* id,FILE* ficheiro_output,int F);
void querie1_V_interativa (voo** hash_voos,int* passageiros_voo,char* id,char*resposta,int F);

/////////////////////////////////////////////////////////////// QUERIE 7 AUXILIARES /////////////////////////////////////////////////////////

void swap(int *indice_nova_mediana,int *indice_mediana_antiga,char aeroportos_novo[4],char aeroporto_antigo[4]);
void merge(int arr[], int left[], int left_size, int right[], int right_size);
void merge_sort(int arr[], int size);
int substituir_menor (int medianas[],int mediana_nova,char Aeroportos[][4],int N,char pais[4]);
void heapify(int medianas[], char Aeroportos[][4], int n, int i);
void heapsort(int medianas[], char Aeroportos[][4], int n);
int atraso_aviao (int data_real [6],int data_planeada [6]);
int calcula_atraso_voo (int data_prevista[6],int data_real [6]);
int max_indi (int* valores,char paises[][4],int n);


//FREE
void free_voos (GHashTable* hash_aeroportos,voo** hash_voos);

#endif


