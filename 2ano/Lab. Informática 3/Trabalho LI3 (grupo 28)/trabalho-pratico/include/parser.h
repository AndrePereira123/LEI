#ifndef parser_h
#define parser_h
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>


void* parser_generico_utilizadores (FILE *parse,char *** total_dados,char *ficheiro_erros,GHashTable* erros_utilizadores
                        ,int** erros_voos,char* header,int* n_utilizadores);

void parser_generico_reservas (FILE *parse,char *** total_dados,char *ficheiro_erros,GHashTable* erros_utilizadores
                            ,char* header,int* n_reservas,void* registos_reservas
                            ,GHashTable* verificar_reserva,int* maior_indice_reserva,int interativo);

void parser_generico_voos (FILE *parse,char *** total_dados,char *ficheiro_erros,int** erros_voos
                          ,char* header,int* n_voos,int* voos_validos);

void parser_generico_passageiros (FILE *parse,char *** total_dados,char *ficheiro_erros,GHashTable* erros_utilizadores,int* passageiros_voo
                                  ,int** erros_voos,char* header,GHashTable* dados_utilizadores_id,int* n_passageiros);


void free_matriz (char** total_dados_utilizadores,int numero_utilizadores);
#endif