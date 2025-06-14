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



void choose_queries_testes(btree_registos_ano** anos_registos_utilizadores,btree_registos_reservas** anos_registos_reservas
                          ,btree_registos_voos** anos_registos_voos,aeroporto_arvore* arvore_todos_voos,char** matriz_passageiros,int linhas_matriz
                          ,char* querie,char* detalhes,reservas_arvore** total_dados_reservas,reserva** hash_reservas,btree_utilizadores** arvores_por_inicial
                          ,GHashTable* dados_utilizadores_id,GHashTable* total_dados_voos,int* passageiros_voo,voo** hash_voos,int n_voos,int output,char* ficheiros_corretos,int* q_erradas)
{
    char nome_out [100];
    char nome_out_correto [100];
    sprintf(nome_out, "Resultados/command%d_output.txt", output); //definimos o nome do ficheiro de output
    sprintf(nome_out_correto,"%s/command%d_output.txt",ficheiros_corretos,output);

    FILE* ficheiro_output = fopen(nome_out,"w");  //abrimos o ficheiro de output 
    int num_querie = atoi (querie);

    clock_t begin = clock ();
        
      if (num_querie == 1)    //QUERIE 1
    {
        int resultado = identificar_id (detalhes);
        if (strlen (querie) == 2)
        {
           if (resultado == RESERVA)  {print_querie_1_R (hash_reservas,detalhes,ficheiro_output,1);}
           else if (resultado == VOO) {print_querie_1_V (hash_voos,passageiros_voo,detalhes,ficheiro_output,1);}
           else                       {print_querie_1_U (dados_utilizadores_id,detalhes,ficheiro_output,1);}
        }
        else 
        {
           if (resultado == RESERVA)  {print_querie_1_R (hash_reservas,detalhes,ficheiro_output,0);}
           else if (resultado == VOO) {print_querie_1_V (hash_voos,passageiros_voo,detalhes,ficheiro_output,0);}
           else                       {print_querie_1_U (dados_utilizadores_id,detalhes,ficheiro_output,0);}
        }
    }

    if (num_querie == 2)    //QUERIE 2
    {
        int F = 0;
        if (strlen (querie) == 2) F++;

        char temp[100];
        strcpy(temp,detalhes);
        char* token;
        char id[50];
        char* save_tok;
        token = strtok_r(temp," ",&save_tok);
        strcpy (id,token);

        if (g_hash_table_lookup (dados_utilizadores_id,id) == NULL) 
        {
            fclose (ficheiro_output);
            return;
        }
          token = strtok_r (NULL,"\n",&save_tok);
          if (token == NULL)                           {querie_2_Total (id,dados_utilizadores_id,ficheiro_output,F);}                  
          else if (strcmp ("flights",token) == 0)      {querie_2_Voos (id,dados_utilizadores_id,ficheiro_output,F);}
          else if (strcmp ("reservations",token) == 0) {querie_2_Reservas (id,dados_utilizadores_id,ficheiro_output,F);}
    }


    if (num_querie == 3)    //QUERIE 3
    {
    if (strlen (querie) == 2)  
    {
        fprintf(ficheiro_output,"--- 1 ---\n");
        fprintf(ficheiro_output,"rating: ");        //imprimimos o header se o F estiver incluido na chamada da querie
        }
        int hash = atoi(detalhes + 3);        //fazemos a hash para pesquisar apenas na arvore necessaria
        double res;

        double Rating_total = 0.000;
        double Numero_nodos = 0.000;

        Rating_total = Somar_ratings (total_dados_reservas[hash],&Numero_nodos); //a funcao somar ratings retorna o rating total 
                                                            //e guarda o numero de nodos no endereço passado como argumento
    
        if (Rating_total == 0) res = 0;
        else  res = (Rating_total/Numero_nodos); // media de ratings
        fprintf(ficheiro_output,"%.3f\n",res);

    }

    if (num_querie == 4)  //QUERIE 4
    {
        int hash = atoi (detalhes + 3);
        int contador = 1;
        if (strlen (querie) == 2)
        {
        print_querie_4 (total_dados_reservas[hash],ficheiro_output,&contador,1);
        }
        else 
        {
        print_querie_4 (total_dados_reservas[hash],ficheiro_output,&contador,0);
        }
    }

    if (num_querie == 5) //QUERIE 5
    {
        char temp[100];
        strcpy(temp,detalhes);
        char* token;
        char* save_token;
        token = strtok_r(temp," ",&save_token);
       
        aeroporto_arvore* arvore_de_voos = g_hash_table_lookup (total_dados_voos,token);
        
        const char* delimiter = "\"";
        token = strtok_r(NULL,delimiter,&save_token);
        int data_inicio[6];
        converter_schedule(token,data_inicio);     //convertemos as datas de strings para inteiros 

        token = strtok_r(NULL,delimiter,&save_token);
        token = strtok_r(NULL,delimiter,&save_token);
        int data_fim[6];
        converter_schedule(token,data_fim);
        
        int contador = 1;
        if (strlen (querie) == 2)
        {
        print_querie_5 (arvore_de_voos,ficheiro_output,data_inicio,data_fim,&contador,1);
        }
        else 
        {
        print_querie_5 (arvore_de_voos,ficheiro_output,data_inicio,data_fim,&contador,0);
        }

    }

    if (num_querie == 6) //QUERIE 6
     {
        char temp[100];
        strcpy(temp,detalhes);
        char* token;
        char* save_token;
        token = strtok_r(temp," ",&save_token);

        int ano = atoi (token);
        token = strtok_r(NULL," ",&save_token);     //guardamos o ano e o N numero de paises a considerar como inteiros
        int N = atoi (token);
        
         if (strlen (querie) == 2)
         {
          print_querie_6(total_dados_voos,ano,N,ficheiro_output,1);
         }
         else 
         {
          print_querie_6(total_dados_voos,ano,N,ficheiro_output,0);
         }

     }

    if (num_querie == 7) //QUERIE 7
    {
        
        int numero_aeroportos = atoi (detalhes);
        if (strlen (querie) == 2) 
        {
            print_querie_7 (total_dados_voos,numero_aeroportos,ficheiro_output,n_voos,1);
        }
        else print_querie_7 (total_dados_voos,numero_aeroportos,ficheiro_output,n_voos,0);

    }

    if(num_querie == 8)  //QUERIE 8
    {
        char temp[100];
        strcpy(temp,detalhes);
        char* token;
        char* save_token;
        token = strtok_r(temp," ",&save_token);
        
        int hash = atoi(token + 3);               //calculamos a hash para selecionar apenas a arvore necessaria

        token = strtok_r(NULL," ",&save_token);
        int data_inicio[3];
        converter_data(token,data_inicio);          //convertemos as datas em inteiros
        token = strtok_r(NULL," ",&save_token);
        int data_fim[3];
        converter_data(token,data_fim);
        if (strlen (querie) == 2)  
        {
            fprintf(ficheiro_output,"--- 1 ---\n"); //imprimimos o cabecalho em caso de chamada "8F"
            fprintf(ficheiro_output,"revenue: ");
        }
        print_querie_8(total_dados_reservas[hash],ficheiro_output,data_inicio,data_fim);

    }

    if (num_querie == 9) //QUERIE 9
    {

        int tipo = 0;
        if (strlen (querie) == 2) tipo = 1; //print F

        int contador = 1;
        int ASCII = 0; 
        if (detalhes[0] != '\"') {ASCII =  (int)detalhes[0];}
        else ASCII =  (int)detalhes[1];
        if (ASCII < 0) ASCII += 128;
        print_querie_9 (arvores_por_inicial[ASCII],detalhes,ficheiro_output,&contador,tipo);

        
    }
    
    if (num_querie == 10 )  //QUERIE 10
    {
        int data[2] = {0};
        if (detalhes[0] != 'v')
        {
        char temp[100];
        strcpy(temp,detalhes);
        char* token;
        char* save_token;
        token = strtok_r(temp," ",&save_token);

        if (token != NULL)
        {
            data[0] = atoi (token);
            token = strtok_r(NULL," ",&save_token);
            if (token != NULL)
            {
                data[1] = atoi (token);
                token = strtok_r(NULL," ",&save_token);
            }
        }
        }
       if (querie [2] == 'F') //se tiver F
       {
        querie10 (data,matriz_passageiros,linhas_matriz,arvore_todos_voos,ficheiro_output,anos_registos_utilizadores
                 ,anos_registos_reservas,anos_registos_voos,n_voos,1);
        }
      else
        {
        querie10 (data,matriz_passageiros,linhas_matriz,arvore_todos_voos,ficheiro_output,anos_registos_utilizadores
                 ,anos_registos_reservas,anos_registos_voos,n_voos,0);
        }

        

    }

    char resposta [100];
    fclose (ficheiro_output);
    ficheiro_output = fopen(nome_out,"r");
    FILE* ficheiro_output_correto = fopen (nome_out_correto,"r");
    int erro = verificar_querie (ficheiro_output,ficheiro_output_correto,resposta);
    if (erro == 1) (*q_erradas)++;
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Querie ");

    if (num_querie == 1 || num_querie == 4 ||num_querie == 7 || num_querie == 10 ) printf (YELLOW_TEXT "%d\n" RESET_COLOR ,num_querie);
    else if (num_querie == 2 || num_querie == 5 ||num_querie == 8) printf (RED_TEXT "%d\n" RESET_COLOR ,num_querie);
    else printf (GREEN_TEXT "%d\n" RESET_COLOR ,num_querie);

    printf ("Input Numero %d : %f segundos  => ",output,time_spent);
    if (erro == 1) 
    {
        printf (RED_TEXT "%s\n" RESET_COLOR,resposta);
    }
    else printf ("%s\n",resposta);
    
    printf ("------------------------------------------------------------------------\n");
    
    fclose (ficheiro_output);
    fclose (ficheiro_output_correto);
}







int verificar_querie (FILE* ficheiro_output,FILE* ficheiro_output_correto,char* resposta)
{
   int resultado = 0;
   char linha [512];
   char linha2 [512];
   int n_linha = 0;
   while (fgets(linha,512,ficheiro_output) && fgets (linha2,512,ficheiro_output_correto))
   {
        n_linha++;
        if (strcmp (linha,linha2) != 0) 
        {
            resultado = 1;
          break;
        }
   }
   
   if (fgets(linha,512,ficheiro_output) != NULL || fgets (linha2,512,ficheiro_output_correto) != NULL)
   {
     resultado = 1;
   }
    
    if (resultado == 0) {strcpy (resposta,"Passou no teste"); return 0;}
    else {sprintf (resposta,"Nao passou no teste -- Erro na linha %d",n_linha);return 1;} 
}
