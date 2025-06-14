#ifndef queries_interativas_h
#define queries_interativas_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

void querie_2_Voos_interativo (char* username ,GHashTable* dados_utilizadores_id,char* resposta,int F);

void  querie10_interativa (int data[2],char** matriz_passageiros,int linhas_matriz,aeroporto_arvore* arvore_todos_voos,char* resposta
                                ,btree_registos_ano** anos_registos_utilizadores,btree_registos_reservas** anos_registos_reservas
                                ,btree_registos_voos** anos_registos_voos,int n_voos,int F);

#endif