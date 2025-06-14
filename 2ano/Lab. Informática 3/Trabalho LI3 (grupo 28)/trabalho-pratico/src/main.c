#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <sys/resource.h>

#include "../include/struct_utilizador.h"
#include "../include/struct_reserva.h"
#include "../include/gestor_voos.h"
#include "../include/struct_voos.h"
#include "../include/gestor_passageiro.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include "../include/modo_interativo.h"

#define TAMANHO 5000
#define TAMANHO_HASH 10000
#define HASH_PASSAGEIROS 2000
#define HASH_VOOS_ERRADOS 2000
#define HASH_ASCII 300

#define RESERVA 0
#define VOO 1
#define PASSAGEIROS 2
#define UTILIZADORES 3


int main(int argc, char **argv) // caminho de dir e ficheiro de inputs
{
   setlocale(LC_COLLATE, "en_US.UTF-8");
    if (argc == 1)
    {
        interativo();
        return 1;
    }
    char pasta[100];
    char *ficheiro_inputs = strdup(argv[2]);

    void *apontador_verificar_hotel_passageiros = NULL;
    GHashTable *verificar_hotel = NULL;
    int verificar_inicial_utilizador [HASH_ASCII] = {0};

    void *apontador_verificar_utilizador = NULL;
    GHashTable *verificar_utilizador = NULL;
    
    apontador_verificar_hotel_passageiros = check_hoteis_e_passageiro(ficheiro_inputs, verificar_hotel,verificar_inicial_utilizador);
    
    apontador_verificar_utilizador = check_utilizadores (ficheiro_inputs,verificar_utilizador);

    void* anos_registos_utilizadores = criar_lista_anos_registos();   
    void* anos_registos_voos = criar_lista_registos_voos();
    aeroporto_arvore* arvore_todos_voos = NULL;
    void* anos_registos_reservas  = criar_lista_registos_reservas();

    char **total_dados_utilizadores = malloc(TAMANHO * sizeof(char *));
    char **total_dados_reservas = malloc(TAMANHO * sizeof(char *)); 
    char **total_dados_voos = malloc(TAMANHO * sizeof(char *));
    char **total_dados_passageiros = malloc(TAMANHO * sizeof(char *));

    void *hash_todos_utilizadores = NULL;         
    GHashTable *tabela_todos_utilizadores = NULL; 

    void *dados_voos = NULL;        
    GHashTable *tabela_voos = NULL; 

    void *erros_utilizadores_void = NULL; 
    GHashTable *tabela_erros = NULL;      

    btree_utilizadores** apontador_arvore_por_iniciais = calloc (sizeof (btree_utilizadores*),HASH_ASCII);   //hashing de todos os utilizadores em arvores por inicial

    reservas_arvore **dados_reservas = calloc(sizeof(reservas_arvore *),TAMANHO_HASH);                                   

    int *erros_voos = malloc(sizeof(int) * HASH_VOOS_ERRADOS);
   

    int n_voos = 0;           
    int n_voos_validos = 0;
    int numero_utilizadores = 0;
    int n_passageiros = 0;
    int n_reservas = 0;
    int maior_id_reserva = 0;

    sprintf(pasta, "%s/users.csv", argv[1]);
    FILE *parse_u = fopen(pasta, "r");

    sprintf(pasta, "%s/reservations.csv", argv[1]);
    FILE *parse_r = fopen(pasta, "r");

    sprintf(pasta, "%s/flights.csv", argv[1]);
    FILE *parse_v = fopen(pasta, "r");

    sprintf(pasta, "%s/passengers.csv", argv[1]);
    FILE *parse_p = fopen(pasta, "r");

    if (parse_r == NULL || parse_u == NULL || parse_v == NULL || parse_p == NULL)
    {
        printf("Não foi possível abrir o ficheiro \n");
        return 0;
    }

  char *ficheiro_utilizador_erro = "Resultados/users_errors.csv";
  char *header_utilizadores = "id;name;email;phone_number;birth_date;sex;passport;country_code;address;account_creation;pay_method;account_status";
  erros_utilizadores_void = parser_generico_utilizadores(parse_u, &total_dados_utilizadores, ficheiro_utilizador_erro, tabela_erros, &erros_voos, header_utilizadores, &numero_utilizadores);

  char *ficheiro_reserva_erro = "Resultados/reservations_errors.csv";
  char *header_reservas = "id;user_id;hotel_id;hotel_name;hotel_stars;city_tax;address;begin_date;end_date;price_per_night;includes_breakfast;room_details;rating;comment";
  parser_generico_reservas(parse_r, &total_dados_reservas, ficheiro_reserva_erro, erros_utilizadores_void
                            ,header_reservas, &n_reservas,anos_registos_reservas,apontador_verificar_hotel_passageiros,&maior_id_reserva,0);

  reserva **hash_reservas = calloc(sizeof(reserva *),maior_id_reserva); 

  char *ficheiro_voos_erro = "Resultados/flights_errors.csv";
  char *header_voos = "id;airline;plane_model;total_seats;origin;destination;schedule_departure_date;schedule_arrival_date;real_departure_date;real_arrival_date;pilot;copilot;notes";
  parser_generico_voos(parse_v, &total_dados_voos, ficheiro_voos_erro, &erros_voos, header_voos,&n_voos, &n_voos_validos);

  voo **hash_voos = calloc(sizeof(voo *),n_voos);                              

  int passageiros_voo[n_voos];
  for (int i = 0; i < n_voos; i++)
      passageiros_voo[i] = 0;
    
  hash_todos_utilizadores = parser_utilizadores(total_dados_utilizadores, tabela_todos_utilizadores, apontador_arvore_por_iniciais, numero_utilizadores,anos_registos_utilizadores,verificar_inicial_utilizador,apontador_verificar_utilizador,0); // parsings nao genericos para Btrees etc.
  free_matriz(total_dados_utilizadores, numero_utilizadores);
                        

  char *passageiros_erro = "Resultados/passengers_errors.csv";
  char *header_passageiros = "flight_id;user_id";
  parser_generico_passageiros(parse_p, &total_dados_passageiros, passageiros_erro, erros_utilizadores_void, passageiros_voo, &erros_voos, header_passageiros, hash_todos_utilizadores, &n_passageiros);  free_erros(erros_utilizadores_void);
  free(erros_voos);       

  dados_reservas = parser_reserva(total_dados_reservas, dados_reservas, &hash_reservas, hash_todos_utilizadores, apontador_verificar_hotel_passageiros, n_reservas,anos_registos_reservas,0);
  free_matriz(total_dados_reservas, n_reservas);

  dados_voos = parser_voos(total_dados_voos, tabela_voos, passageiros_voo, &hash_voos, n_voos_validos,anos_registos_voos,&arvore_todos_voos);
  free_matriz(total_dados_voos, n_voos_validos);          

  parser_passageiros(total_dados_passageiros, hash_todos_utilizadores, hash_voos, n_passageiros,apontador_verificar_hotel_passageiros,0);       

  interpreter(anos_registos_utilizadores,anos_registos_reservas,anos_registos_voos,arvore_todos_voos
              ,total_dados_passageiros,n_passageiros,dados_reservas, hash_reservas, apontador_arvore_por_iniciais
              , hash_todos_utilizadores, dados_voos, hash_voos, passageiros_voo, n_voos, ficheiro_inputs); // realizaçao das queries

    free_matriz(total_dados_passageiros, n_passageiros);

    free_g_hash_utilizadores (hash_todos_utilizadores);
    free_hash_utilizadores_inicial(apontador_arvore_por_iniciais);
    free_hash_reservas_essenciais(apontador_verificar_hotel_passageiros);

    free_registos_utilizadores(anos_registos_utilizadores);
    free_registos_reservas(anos_registos_reservas);
    free_registos_voos(anos_registos_voos);

    free_tree_reservas(dados_reservas, hash_reservas);
    free_aeroporto (arvore_todos_voos);
    free_voos(dados_voos, hash_voos);
    g_hash_table_destroy (apontador_verificar_utilizador);

    fclose(parse_r);
    fclose(parse_u);
    fclose(parse_v);
    fclose(parse_p);
    free(ficheiro_inputs);

    return 0;
}
