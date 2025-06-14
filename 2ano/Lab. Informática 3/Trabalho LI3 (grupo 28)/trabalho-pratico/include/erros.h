#ifndef erros_h
#define erros_h
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

int dividirLinha(char *linha, char dados_linha[14][126]);
int is_integer(char* str);
int includes_breakfast (char* str);
int verifica_rating(char* str);
int testar_linha_voo(char* linha);
int testar_linha_utilizador(char* linha);
int verifica_email (char* str);
int dividirLinha_utilizador(char *linha, char dados_linha[12][150]);
int testar_linha_reserva(char* linha,GHashTable* erros_utilizadores,void* registos_reservas,GHashTable* verificar_reserva,int interativo);
int dividirLinha_voo(char *linha, char dados_linha[13][126]);
int testar_linha_passageiros(char* linha,GHashTable* erros_utilizadores,int* erros_voos,GHashTable* dados_utilizadores_id,int* passageiros_voo);
int dividir_linha_passageiro (char* linha,char dados_passageiro [2][40]);

#endif