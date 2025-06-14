#ifndef utilidade_tempo_h
#define utilidade_tempo_h
#include <stdio.h>
#include <stdlib.h>

int check_data (char*dados_linha,int data[3]);
int compara_data_tempo(int data_inicial[6], int data_final[6]);
int check_data_tempo(char*dados_linha,int data[6]);
int compara_data_utilizador(int data_nascimento[3], int data_creation[6]);
void converter_data (char letras[],int data[]);
int comparar_datas (int data1[],int data2[]) ;
int diferenca_dias_atrasos (int data_inicial[3],int data_final[3]);
int diferenca_dias (int data_inicial[3],int data_final[3]);
int c_idade (char* nascimento);
int sum (int* array,int N);

#endif