#ifndef interpreter_h
#define interpreter_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void interpreter(btree_registos_ano** anos_registos_utilizadores,btree_registos_reservas** anos_registos_reservas,btree_registos_voos** anos_registos_voos,aeroporto_arvore* arvore_todos_voos,char** matriz_passageiros,int linhas_matriz,reservas_arvore** total_dados_reservas,reserva** hash_reservas, btree_utilizadores** arvores_por_inicial,GHashTable* dados_utilizadores_id,GHashTable* total_dados_voos,voo** hash_voos,int* passageiros_voo,int n_voos,char* caminho_inputs);

void interpreter_testes (btree_registos_ano** anos_registos_utilizadores,btree_registos_reservas** anos_registos_reservas
                            ,btree_registos_voos** anos_registos_voos,aeroporto_arvore* arvore_todos_voos,char** matriz_passageiros
                            ,int linhas_matriz,reservas_arvore** total_dados_reservas,reserva** hash_reservas, btree_utilizadores** arvores_por_inicial
                            ,GHashTable* dados_utilizadores_id,GHashTable* total_dados_voos,voo** hash_voos,int* passageiros_voo,int n_voos
                            ,char* caminho_inputs,char* ficheiros_corretos);

#endif