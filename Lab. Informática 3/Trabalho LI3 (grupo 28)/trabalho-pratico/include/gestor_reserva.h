#ifndef gestor_reserva_h
#define gestor_reserva_h
#include <stdio.h>
#include <stdlib.h>



typedef struct Reservas_Arvore reservas_arvore;
typedef struct BTREE_RESERVAS_UTILIZADOR btree_reservas_utilizador;
typedef struct ARRAY_RESERVAS array_reservas;
typedef struct BTREE_REGISTOS_RESERVAS btree_registos_reservas;

/////////////////////////////////////////////////////////////// INICIALIZACAO ////////////////////////////////////////////////

btree_registos_reservas** criar_lista_registos_reservas();
btree_registos_reservas* criar_ano_reservas ();
reservas_arvore* criar_arvore_vazia();
btree_reservas_utilizador* criar_arvore_reservas_utilizador ();

/////////////////////////////////////////////////////////////////// INSERCOES E BALANCEAMENTO ///////////////////////////////

reservas_arvore* inserir_reserva (reservas_arvore* arvore, void* Nodo_novo);
btree_reservas_utilizador* inserir_reserva_utilizador (void* reserva_nova,btree_reservas_utilizador** arvore_mae);
reservas_arvore* Rodar_reserva_direita (reservas_arvore *y);
reservas_arvore* Rodar_reserva_esquerda (reservas_arvore* x);
int fator_balanceamento (reservas_arvore* arvore);
int Maior_altura (reservas_arvore* arvore);

///////////////////////////////////////////////////////////// GETTERS ///////////////////////////////////////////////////////

void retorna_data_reserva(int data[3],btree_reservas_utilizador* res);

btree_reservas_utilizador* esquerda_arvore_reservas(btree_reservas_utilizador* arvore);
btree_reservas_utilizador* direita_arvore_reservas(btree_reservas_utilizador* arvore);


//////////////////////////////////////////////////////////// QUERIE 2 //////////////////////////////////////////////////////

void print_reservas_q2_interativo(btree_reservas_utilizador* arvore,char* resposta,int* contador,int F);
void print_reservas_q2 (btree_reservas_utilizador* arvore, FILE *ficheiro_output);
void print_reservas_q2F (btree_reservas_utilizador* arvore, FILE *ficheiro_output,int* contador);
void guardar_btree_id_reservas (array_reservas** reservas,btree_reservas_utilizador* dados_utilizadores_id);

//////////////////////////////////////////////////////////// QUERIE 3 AUXILIARES ///////////////////////////////////////////

int Somar_ratings (reservas_arvore* total_dados,double* N_nodos);
double total_price (int* data_inicio,int* data_fim,char* preco,char* imposto);

//////////////////////////////////////////////////////////// QUERIE 4 //////////////////////////////////////////////////////

void querie4_interativa   (reservas_arvore* arvore,char* resposta,int* contador,int F);
void print_querie_4 (reservas_arvore* arvore,FILE* ficheiro_output,int* contador,int F);
void print_nodo_q4F(void* nodo,FILE* ficheiro_output);
void print_nodo_q4(void* nodo,FILE* ficheiro_output);

//////////////////////////////////////////////////////////// QUERIE 8 //////////////////////////////////////////////////////

void querie8_interativa(reservas_arvore* arvore,char* resposta,int data_inicio[3],int data_fim[3]);
void print_querie_8(reservas_arvore* arvore,FILE* ficheiro_output,int data_inicio[3],int data_fim[3]);
int somar_lucro (reservas_arvore* arvore,int data_inicio[3],int data_fim[3]);


/////////////////////////////////////////////////////////// QUERIE 10 AUXILIARES ///////////////////////////////////////////

void calcular_reservas_data (int data[2],btree_registos_reservas** anos_registos_reservas,int numeros_reservas[],int tipo);
int somar_ano_reservas (btree_registos_reservas* anos_registos_reservas);

//////////////////////////////////////////////////////////// FREES ///////////////////////////////////////////////////////////

void free_registos_reservas(btree_registos_reservas** anos_registo_reservas);
void  free_reserva (reservas_arvore* arvore_r);
void free_arvore_reservas (btree_reservas_utilizador* arvore);
void free_tree_reservas (reservas_arvore** dados_reservas,void* hash_reservas);
void fazer_registo_reserva (int data_inicio_reserva[3],btree_registos_reservas** registos_reservas);



#endif