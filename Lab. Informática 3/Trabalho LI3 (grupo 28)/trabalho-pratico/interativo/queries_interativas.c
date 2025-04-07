#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <glib.h>
#include <sys/resource.h>
#include "../include/struct_reserva.h"
#include "../include/struct_utilizador.h"
#include "../include/gestor_passageiro.h"
#include "../include/gestor_voos.h"
#include "../include/struct_voos.h"
#include "../include/queries.h"
#include "../include/utilidade_tempo.h"


#define RED_TEXT "\x1b[31m"
#define GREEN_TEXT "\x1b[32m"
#define YELLOW_TEXT "\x1b[33m"
#define RESET_COLOR "\x1b[0m"


#define RESERVA 0
#define VOO 1
#define PASSAGEIROS 2
#define UTILIZADORES 3
#define MES 1
#define ANO 2
#define INATIVO "i"


void querie_2_Voos_interativo (char* username ,GHashTable* dados_utilizadores_id,char* resposta,int F)
{   
    if (! g_hash_table_contains (dados_utilizadores_id,username)) 
        {
            strcpy (resposta,"Esse utilizador nao existe\n");
            return;
        }
     utilizador* user = g_hash_table_lookup (dados_utilizadores_id,username);
        int contador = 1;
        print_voos_q2_interativo (retorna_arvore_voos(user),resposta,&contador,F);
}





void  querie10_interativa (int data[2],char** matriz_passageiros,int linhas_matriz,aeroporto_arvore* arvore_todos_voos,char* resposta
                                ,btree_registos_ano** anos_registos_utilizadores,btree_registos_reservas** anos_registos_reservas
                                ,btree_registos_voos** anos_registos_voos,int n_voos,int F)
{
    int utilizadores[31] = {0};
    int reservas[31] = {0};
    int voos [31] = {0};
    int passageiros [31] = {0};
    int passageiros_unicos [31] = {0};
    if (data[0] != 0 && data[1] != 0 )         //descrever dias do  mes
    { 
        calcular_utilizadores_data (data,anos_registos_utilizadores,utilizadores,MES);
        calcular_reservas_data (data,anos_registos_reservas,reservas,MES);
        calcular_voos_data (data,anos_registos_voos,voos,MES);
        int id_de_voos_a_contar [n_voos];
        int dia_dos_voos [n_voos];
        for (int k = 0; k < n_voos ;k++) {id_de_voos_a_contar[k] = 0; dia_dos_voos[k] = 0;}

        encontrar_voos_data (data,arvore_todos_voos,id_de_voos_a_contar,dia_dos_voos,0);
        
        contador_de_passageiros (matriz_passageiros,linhas_matriz,id_de_voos_a_contar,dia_dos_voos,passageiros,passageiros_unicos);
            if (F == 0) 
            {
                int i;
                for (i = 0;i < 31;i++)
                {
                if (utilizadores[i] != 0 || reservas[i] != 0) sprintf (resposta + strlen(resposta),"%d;%d;%d;%d;%d;%d\n",i+1,utilizadores[i],voos[i],passageiros[i],passageiros_unicos[i],reservas[i]);
                }
            }
            else
            {
                int i;
                int header = 1;
                for (i = 0;i < 31;i++)
                {
                if (utilizadores[i] != 0 || reservas[i] != 0) 
                {
                    sprintf (resposta + strlen(resposta),"--- %d ---\n",header);
                    sprintf (resposta + strlen(resposta),"day: %d\n",i+1);
                    sprintf (resposta + strlen(resposta),"users: %d\n",utilizadores[i]);
                    sprintf (resposta + strlen(resposta),"flights: %d\n",voos[i]);
                    sprintf (resposta + strlen(resposta),"passengers: %d\n",passageiros[i]);
                    sprintf (resposta + strlen(resposta),"unique_passengers: %d\n",passageiros_unicos[i]);
                    sprintf (resposta + strlen(resposta),"reservations: %d\n",reservas[i]);
                    sprintf (resposta + strlen(resposta)," \n");
                    header++;
                }
                }
            }
    }else if (data[0] != 0)           //descrever 1 ano
               {
                
                    calcular_utilizadores_data (data,anos_registos_utilizadores,utilizadores,ANO);
                    calcular_reservas_data (data,anos_registos_reservas,reservas,ANO);
                    calcular_voos_data (data,anos_registos_voos,voos,ANO);
                    int id_de_voos_a_contar [n_voos];
                    int meses_dos_voos [n_voos];
                    for (int k = 0; k < n_voos ;k++) {id_de_voos_a_contar[k] = 0; meses_dos_voos[k] = 0;}

                    encontrar_voos_data (data,arvore_todos_voos,id_de_voos_a_contar,meses_dos_voos,1);
                
                    contador_de_passageiros_mes (matriz_passageiros,linhas_matriz,id_de_voos_a_contar,meses_dos_voos,passageiros,passageiros_unicos);
                          
                      if (F == 0)
                      {
                          int i;
                          for (i = 0;i < 12;i++)
                          {
                          if (utilizadores[i] != 0 || reservas[i] != 0) sprintf (resposta + strlen (resposta),"%d;%d;%d;%d;%d;%d\n",i+1,utilizadores[i],voos[i],passageiros[i],passageiros_unicos[i],reservas[i]);
                          }
                      }
                      else
                      {
                          int i;
                          int header = 1;
                          for (i = 0;i < 12;i++)
                          {
                          if (utilizadores[i] != 0 || reservas[i] != 0) 
                          {
                              sprintf (resposta + strlen (resposta),"--- %d ---\n",header);
                              sprintf (resposta + strlen (resposta),"month: %d\n",i+1);
                              sprintf (resposta + strlen (resposta),"users: %d\n",utilizadores[i]);
                              sprintf (resposta + strlen (resposta),"flights: %d\n",voos[i]);
                              sprintf (resposta + strlen (resposta),"passengers: %d\n",passageiros[i]);
                              sprintf (resposta + strlen (resposta),"unique_passengers: %d\n",passageiros_unicos[i]);
                              sprintf (resposta + strlen (resposta),"reservations: %d\n",reservas[i]);
                              sprintf (resposta + strlen (resposta)," \n");
                              header++;
                          }
                          }
                      }
               }
                else  //nao temos dado algum (descrever anos todos)
                {
    
                    calcular_utilizadores_data (data,anos_registos_utilizadores,utilizadores,0);
                    calcular_reservas_data (data,anos_registos_reservas,reservas,0);
                    calcular_voos_data (data,anos_registos_voos,voos,0);
                    int ano_dos_voos [n_voos];
                    for (int k = 0; k < n_voos ;k++) {ano_dos_voos[k] = 0;}

                    encontrar_voos_data_todos (data,arvore_todos_voos,ano_dos_voos);
        
                    contador_de_passageiros_total (matriz_passageiros,linhas_matriz,ano_dos_voos,passageiros,passageiros_unicos);
                      


                              if (F == 0)
                          {
                              int i;
                              for (i = 0;i < 14;i++)
                              {
                              if (utilizadores[i] != 0 || reservas[i] != 0) sprintf (resposta + strlen (resposta),"%d;%d;%d;%d;%d;%d\n",i+2010,utilizadores[i],voos[i],passageiros[i],passageiros_unicos[i],reservas[i]);
                              }
                          }
                          else
                          {
                            int i;
                          int header = 1;
                          for (i = 0;i < 14;i++)
                          {
                          if (utilizadores[i] != 0 || reservas[i] != 0) 
                          {
                              sprintf (resposta + strlen (resposta),"--- %d ---\n",header);
                              sprintf (resposta + strlen (resposta),"year: %d\n",i+2010);
                              sprintf (resposta + strlen (resposta),"users: %d\n",utilizadores[i]);
                              sprintf (resposta + strlen (resposta),"flights: %d\n",voos[i]);
                              sprintf (resposta + strlen (resposta),"passengers: %d\n",passageiros[i]);
                              sprintf (resposta + strlen (resposta),"unique_passengers: %d\n",passageiros_unicos[i]);
                              sprintf (resposta + strlen (resposta),"reservations: %d\n",reservas[i]);
                              sprintf (resposta + strlen (resposta)," \n");
                              header++;
                          }
                          }
                          }
                }
}