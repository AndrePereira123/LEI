#ifndef menu_h
#define menu_h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

int calcular_pagina_final (char* resposta_original,int querie,int y_max);
void print_pagina (char* resposta,int querie,int pagina,WINDOW* caminho_win,int y_max,int x_max);
int menu();
void path();
void processar(char *caminho,WINDOW *caminho_win,int y_max,int x_max);
int select_querie();
int escolha_output(int querie);
char* write_querie(int querie,int n_res,int n_voos,int n_utili);

#endif