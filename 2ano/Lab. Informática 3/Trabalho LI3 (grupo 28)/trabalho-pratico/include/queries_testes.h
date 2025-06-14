#ifndef queries_testes_h
#define queries_testes_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>


void choose_queries_testes(btree_registos_ano** anos_registos_utilizadores,btree_registos_reservas** anos_registos_reservas
                          ,btree_registos_voos** anos_registos_voos,aeroporto_arvore* arvore_todos_voos,char** matriz_passageiros,int linhas_matriz
                          ,char* querie,char* detalhes,reservas_arvore** total_dados_reservas,reserva** hash_reservas,btree_utilizadores** arvores_por_inicial
                          ,GHashTable* dados_utilizadores_id,GHashTable* total_dados_voos,int* passageiros_voo,voo** hash_voos,int n_voos,int output,char* ficheiros_corretos,int* q_erradas);

#endif