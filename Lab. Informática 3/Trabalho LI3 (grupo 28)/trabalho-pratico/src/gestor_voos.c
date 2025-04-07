#include <stdlib.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/gestor_voos.h"
#include "../include/struct_voos.h"
#include "../include/struct_reserva.h"
#include "../include/struct_utilizador.h"
#include "../include/utilidade_tempo.h"
#include "../include/erros.h"

#define HASH_VOOS_ID 6000
#define MES 1
#define ANO 2


struct AEROPORTO_ARVORE
{
    int vinte_um;
    int vinte_dois;
    int vinte_tres;
    char pais[4];
    void* Nodo;
    aeroporto_arvore* esquerda;
    aeroporto_arvore* direita;
};


struct BTREE_REGISTOS_VOOS
{
    int Ano_todo;
    int Janeiro [31];
    int Fevereiro [31];
    int Marco [31];
    int Abril [31];
    int Maio [31];
    int Junho [31];
    int Julho [31];
    int Agosto [31];
    int Setembro [31];
    int Outubro [31];
    int Novembro [31];
    int Decembro [31];
};



btree_registos_voos** criar_lista_registos_voos() 
{
    btree_registos_voos** registos = malloc (sizeof(btree_registos_voos*) * 14); //2003 a 2023
    int i = 0;
    while (i < 14)
    {
    registos [i] = criar_ano_voos ();
    i++;
    }
    return registos;
}

btree_registos_voos* criar_ano_voos ()
{
    btree_registos_voos* novo_ano = calloc (1,sizeof(btree_registos_voos));
    return novo_ano;
}


aeroporto_arvore* criar_aeroporto_vazio() 
{
    aeroporto_arvore* arvore = malloc (sizeof (aeroporto_arvore));
    arvore->vinte_um = 0;
    arvore->vinte_dois = 0;
    arvore->vinte_tres = 0;
    arvore->Nodo = NULL;
    arvore->direita = NULL;
    arvore->esquerda = NULL;
    return arvore;
}


aeroporto_arvore* inserir_voo (aeroporto_arvore* arvore, void* Nodo_novo,int* passageiros_voo,int t) //empate ver pelo id
    {
        if (arvore == NULL)             //insere e contabiliza voos pelo aeroporto de origem
        {
            arvore = criar_aeroporto_vazio();
            strcpy (arvore->pais,retorna_origin (Nodo_novo));
            if (retorna_s_departure_date(Nodo_novo)[0] == 2021) {arvore->vinte_um   += passageiros_voo[t];}
            else if (retorna_s_departure_date(Nodo_novo)[0] == 2022) {arvore->vinte_dois += passageiros_voo[t];}
            else {arvore->vinte_tres += passageiros_voo[t];}  
            arvore->Nodo = Nodo_novo;  
        }
        else 
        { 
        //strcpy (arvore->pais,Nodo_novo->origin);    
        if (retorna_s_departure_date(Nodo_novo)[0] == 2021) {arvore->vinte_um   += passageiros_voo[t];}
        else if (retorna_s_departure_date(Nodo_novo)[0] == 2022) {arvore->vinte_dois += passageiros_voo[t];}
        else {arvore->vinte_tres += passageiros_voo[t];}   
         
        if (arvore->Nodo == NULL)
        {
            arvore->Nodo = Nodo_novo;
        }
        else 
        {
        int valor;
        valor = compara_data_voos(retorna_s_departure_date(arvore->Nodo),retorna_s_departure_date(Nodo_novo)); //Compara datas está na reservas

        if (valor == 1)
            {
                arvore->esquerda = inserir_voo_simple (arvore->esquerda,Nodo_novo,passageiros_voo,t);  
            }
            else if (valor == 2)
            {
                 arvore->direita = inserir_voo_simple (arvore->direita,Nodo_novo,passageiros_voo,t); 
            }
             else                        //desempate pelo id de voo
             {
               if (compara_ids_voo(retorna_id_char (arvore->Nodo),retorna_id_char (Nodo_novo)) == 1)
                 {
                    arvore->direita = inserir_voo_simple (arvore->direita,Nodo_novo,passageiros_voo,t);
                 }
                 else 
                 {
                     arvore->esquerda = inserir_voo_simple (arvore->esquerda,Nodo_novo,passageiros_voo,t);
                 }
             }
            }
        }
        return arvore;
    } 

aeroporto_arvore* inserir_voo_simple (aeroporto_arvore* arvore, void* Nodo_novo,int* passageiros_voo,int t) //empate ver pelo id
    {
        if (arvore == NULL)             //insere e contabiliza voos pelo aeroporto de origem
        {
            arvore = criar_aeroporto_vazio();
            arvore->Nodo = Nodo_novo;  
        }
        else 
        { 
        if (arvore->Nodo == NULL)
        {
            arvore->Nodo = Nodo_novo;
        }
        else 
        {
        int valor;
        valor = compara_data_voos(retorna_s_departure_date(arvore->Nodo),retorna_s_departure_date(Nodo_novo)); //Compara datas está na reservas

        if (valor == 1)
            {
                arvore->esquerda = inserir_voo_simple (arvore->esquerda,Nodo_novo,passageiros_voo,t);  
            }
            else if (valor == 2)
            {
                 arvore->direita = inserir_voo_simple (arvore->direita,Nodo_novo,passageiros_voo,t); 
            }
             else                        //desempate pelo id de voo
             {
               if (compara_ids_voo(retorna_id_char(arvore->Nodo),retorna_id_char(Nodo_novo)) == 1)
                 {
                    arvore->direita = inserir_voo_simple (arvore->direita,Nodo_novo,passageiros_voo,t);
                 }
                 else 
                 {
                     arvore->esquerda = inserir_voo_simple (arvore->esquerda,Nodo_novo,passageiros_voo,t);
                 }
             }
            }
        }
        return arvore;
    }

aeroporto_arvore* contar_destination (aeroporto_arvore* arvore,void* voo_chegada,int* passageiros_voo,int t) //contabiliza os passageiros nos aeroportos de destino
{
         if (arvore == NULL)
        {
            arvore = criar_aeroporto_vazio();
            strcpy (arvore->pais,retorna_destination(voo_chegada)); 
                        
            if (retorna_s_departure_date(voo_chegada)[0] == 2021) {arvore->vinte_um   += passageiros_voo[t];}
            else if (retorna_s_departure_date(voo_chegada)[0] == 2022) {arvore->vinte_dois += passageiros_voo[t];}
            else {arvore->vinte_tres += passageiros_voo[t];}  
        }
        else 
        {
            
        if (retorna_s_departure_date(voo_chegada)[0] == 2021) {arvore->vinte_um   += passageiros_voo[t];}
        else if (retorna_s_departure_date(voo_chegada)[0] == 2022) {arvore->vinte_dois += passageiros_voo[t];}
        else {arvore->vinte_tres += passageiros_voo[t];}  

        }
        return arvore;
}


/////////////////////////////////////////////////QUERIE 5////////////////////////////////////////////////////////////////////

void querie5_interativa (aeroporto_arvore* arvore_o,char* resposta,int data_inicio[6],int data_fim[6],int* contador,int F)
{
    if (arvore_o == NULL) return;
    if (arvore_o->Nodo == NULL) return;


    if (compara_data_voos ((retorna_s_departure_date(arvore_o->Nodo)),data_fim) == 1 ) // se a data for maior q a data de fim do range
    {
        querie5_interativa (arvore_o->esquerda,resposta,data_inicio,data_fim,contador,F);  //esquerda pq data era muito recente
    }    
    else if (compara_data_voos ((retorna_s_departure_date(arvore_o->Nodo)),data_inicio) == 2) // se a data for menor q a data de inicio do range
    { 
        querie5_interativa (arvore_o->direita,resposta,data_inicio,data_fim,contador,F); //direita pq data era muito antiga
    }
    else 
    {
    querie5_interativa (arvore_o->direita,resposta,data_inicio,data_fim,contador,F); 
    
    if (F == 0)
    {
    sprintf(resposta + strlen(resposta),"%s;",retorna_id_char (arvore_o->Nodo)); 
    sprintf(resposta + strlen(resposta),"%d/%02d/%02d ",retorna_s_departure_date(arvore_o->Nodo)[0],retorna_s_departure_date(arvore_o->Nodo)[1],retorna_s_departure_date(arvore_o->Nodo)[2]);
    sprintf(resposta + strlen(resposta),"%02d:%02d:%02d;",retorna_s_departure_date(arvore_o->Nodo)[3],retorna_s_departure_date(arvore_o->Nodo)[4],retorna_s_departure_date(arvore_o->Nodo)[5]);
    sprintf(resposta + strlen(resposta),"%s;%s;%s\n",retorna_destination (arvore_o->Nodo),retorna_airline(arvore_o->Nodo),retorna_model(arvore_o->Nodo)); 
    }
    else
    {
    sprintf(resposta+ strlen (resposta),"--- %d ---\n",*contador);
    (*contador)++;

    sprintf(resposta+ strlen (resposta),"id: %s\n",retorna_id_char (arvore_o->Nodo)); 

    sprintf(resposta+ strlen (resposta),"schedule_departure_date: "); 
    sprintf(resposta+ strlen (resposta),"%d/%02d/%02d ",retorna_s_departure_date(arvore_o->Nodo)[0],retorna_s_departure_date(arvore_o->Nodo)[1],retorna_s_departure_date(arvore_o->Nodo)[2]);
    sprintf(resposta+ strlen (resposta),"%02d:%02d:%02d\n",retorna_s_departure_date(arvore_o->Nodo)[3],retorna_s_departure_date(arvore_o->Nodo)[4],retorna_s_departure_date(arvore_o->Nodo)[5]);
    
    sprintf(resposta+ strlen (resposta),"destination: %s\nairline: %s\nplane_model: %s\n",retorna_destination (arvore_o->Nodo),retorna_airline(arvore_o->Nodo),retorna_model(arvore_o->Nodo));
    sprintf(resposta+ strlen (resposta)," \n"); 
    }

    querie5_interativa (arvore_o->esquerda,resposta,data_inicio,data_fim,contador,F);   
    }
}

void print_querie_5 (aeroporto_arvore* arvore_o,FILE* ficheiro_output,int data_inicio[6],int data_fim[6],int* contador,int F)
{
    if (arvore_o == NULL) return;
    if (arvore_o->Nodo == NULL) return;

    if (compara_data_voos ((retorna_s_departure_date(arvore_o->Nodo)),data_fim) == 1 ) // sigual a print_querie5
    {
        print_querie_5 (arvore_o->esquerda,ficheiro_output,data_inicio,data_fim,contador,F);  
    }    
    else if (compara_data_voos ((retorna_s_departure_date(arvore_o->Nodo)),data_inicio) == 2) 
    { 
        print_querie_5 (arvore_o->direita,ficheiro_output,data_inicio,data_fim,contador,F); 
    }
    else 
    {
    print_querie_5 (arvore_o->direita,ficheiro_output,data_inicio,data_fim,contador,F); 

    if (F == 1)
    {
    if (*contador != 1) fprintf(ficheiro_output,"\n");
    fprintf(ficheiro_output,"--- %d ---\n",*contador);
    (*contador)++;

    fprintf(ficheiro_output,"id: %s\n",retorna_id_char (arvore_o->Nodo)); 

    fprintf(ficheiro_output,"schedule_departure_date: "); 
    fprintf(ficheiro_output,"%d/%02d/%02d ",retorna_s_departure_date(arvore_o->Nodo)[0],retorna_s_departure_date(arvore_o->Nodo)[1],retorna_s_departure_date(arvore_o->Nodo)[2]);
    fprintf(ficheiro_output,"%02d:%02d:%02d\n",retorna_s_departure_date(arvore_o->Nodo)[3],retorna_s_departure_date(arvore_o->Nodo)[4],retorna_s_departure_date(arvore_o->Nodo)[5]);
    
    fprintf(ficheiro_output,"destination: %s\nairline: %s\nplane_model: %s\n",retorna_destination (arvore_o->Nodo),retorna_airline(arvore_o->Nodo),retorna_model(arvore_o->Nodo)); 
    }
    else 
    {
    fprintf(ficheiro_output,"%s;",retorna_id_char (arvore_o->Nodo)); 

    fprintf(ficheiro_output,"%d/%02d/%02d ",retorna_s_departure_date(arvore_o->Nodo)[0],retorna_s_departure_date(arvore_o->Nodo)[1],retorna_s_departure_date(arvore_o->Nodo)[2]);
    fprintf(ficheiro_output,"%02d:%02d:%02d;",retorna_s_departure_date(arvore_o->Nodo)[3],retorna_s_departure_date(arvore_o->Nodo)[4],retorna_s_departure_date(arvore_o->Nodo)[5]);
    
    fprintf(ficheiro_output,"%s;%s;%s\n",retorna_destination (arvore_o->Nodo),retorna_airline(arvore_o->Nodo),retorna_model(arvore_o->Nodo)); 

    }
    
    print_querie_5 (arvore_o->esquerda,ficheiro_output,data_inicio,data_fim,contador,F); 
    }
}

//////////////////////////////////////////////////////QUERIE 9////////////////////////////////////////////////////////////////////

void encontrar_voos_data (int data[2],aeroporto_arvore* arvore_todos_voos,int* voos_a_contar,int* tempo_do_voo,int tipo)
{
    if (arvore_todos_voos == NULL) return;
    if (arvore_todos_voos->Nodo == NULL) return;

    if (tipo == 0)
    {
        if (retorna_s_departure_date(arvore_todos_voos->Nodo)[0] < data[0])  //ano maior q arvore
        {
            encontrar_voos_data (data,arvore_todos_voos->direita,voos_a_contar,tempo_do_voo,0);
        }
        else if (retorna_s_departure_date(arvore_todos_voos->Nodo)[0] > data[0]) //ano menor q arvore
        {
            encontrar_voos_data (data,arvore_todos_voos->esquerda,voos_a_contar,tempo_do_voo,0);
        }
        else  if (retorna_s_departure_date(arvore_todos_voos->Nodo)[1] < data[1])  //mes maior q arvore
        {
            encontrar_voos_data (data,arvore_todos_voos->direita,voos_a_contar,tempo_do_voo,0);
        }
        else if (retorna_s_departure_date(arvore_todos_voos->Nodo)[1] > data[1]) //mes menor q arvore
        {
            encontrar_voos_data (data,arvore_todos_voos->esquerda,voos_a_contar,tempo_do_voo,0);
        }
        else 
        {
            encontrar_voos_data (data,arvore_todos_voos->esquerda,voos_a_contar,tempo_do_voo,0);
            
            voos_a_contar [atoi (retorna_id_char(arvore_todos_voos->Nodo))-1] = 1;
            tempo_do_voo [atoi (retorna_id_char(arvore_todos_voos->Nodo))-1] = retorna_s_departure_date(arvore_todos_voos->Nodo)[2];
            
    
            encontrar_voos_data (data,arvore_todos_voos->direita,voos_a_contar,tempo_do_voo,0);
    }
    }
    else 
    {
            if (retorna_s_departure_date(arvore_todos_voos->Nodo)[0] < data[0])  //ano maior q arvore
       {
           encontrar_voos_data (data,arvore_todos_voos->direita,voos_a_contar,tempo_do_voo,1);
       }
       else if (retorna_s_departure_date(arvore_todos_voos->Nodo)[0] > data[0]) //ano menor q arvore
       {
           encontrar_voos_data (data,arvore_todos_voos->esquerda,voos_a_contar,tempo_do_voo,1);
       }
       else    
       {
          encontrar_voos_data (data,arvore_todos_voos->esquerda,voos_a_contar,tempo_do_voo,1);
           
           voos_a_contar [atoi (retorna_id_char(arvore_todos_voos->Nodo))-1] = 1;
           tempo_do_voo [atoi (retorna_id_char(arvore_todos_voos->Nodo))-1] = retorna_s_departure_date(arvore_todos_voos->Nodo)[1];
           
   
           encontrar_voos_data (data,arvore_todos_voos->direita,voos_a_contar,tempo_do_voo,1);
    }
    }
}

void encontrar_voos_data_todos (int data[2],aeroporto_arvore* arvore_todos_voos,int* ano_dos_voos)
{
    if (arvore_todos_voos == NULL) return;
    if (arvore_todos_voos->Nodo == NULL) return;

    encontrar_voos_data_todos (data,arvore_todos_voos->esquerda,ano_dos_voos);
           
           
    ano_dos_voos [atoi (retorna_id_char(arvore_todos_voos->Nodo))-1] = retorna_s_departure_date(arvore_todos_voos->Nodo)[0];
           

    encontrar_voos_data_todos (data,arvore_todos_voos->direita,ano_dos_voos);
}



int fazer_mediana_aeroporto (aeroporto_arvore* aeroporto,int n_voos) 
{
    if (aeroporto->Nodo == NULL) return 0;
    size_t tamanho_array = (n_voos/2);
    int preenchido = 0;
    int array [tamanho_array];
    for (int i = 0; i < tamanho_array;i++) {array[i] = 0;}
    int resultado;
    inserir_atraso_lista (aeroporto,array,&preenchido,&tamanho_array);

    int array_calcular[preenchido];
    for (int i = 0; i < preenchido;i++) {array_calcular[i] = array[i];}

    merge_sort (array_calcular,preenchido);
    if (preenchido %2 == 1)
    {                       
        resultado = (array_calcular [preenchido/2]);
    }
    else
    {
        resultado = ((array_calcular[preenchido/2 - 1] + array_calcular[preenchido/2])/2);
    }
    return resultado;
}

void inserir_atraso_lista (aeroporto_arvore* aeroporto,int* array,int* preenchido,size_t* tamanho_array)
{
    if (aeroporto == NULL) return;
    if (aeroporto->Nodo == NULL) return;
    inserir_atraso_lista (aeroporto->esquerda,array,preenchido,tamanho_array);


    array[*preenchido] = calcula_atraso_voo (retorna_s_departure_date(aeroporto->Nodo),retorna_r_departure_date (aeroporto->Nodo));
    (*preenchido)++;

    inserir_atraso_lista (aeroporto->direita,array,preenchido,tamanho_array);
}

void inserir_registo_voo (int* data,btree_registos_voos** registos_voos)
    {
        int indice_registo = data[0] - 2010;
        registos_voos [indice_registo]->Ano_todo++;
        int endereco_dia = data[2] - 1;
        if      (data[1] == 1) registos_voos [indice_registo]->Janeiro[endereco_dia]++;
        else if (data[1] == 2) registos_voos [indice_registo]->Fevereiro[endereco_dia]++;
        else if (data[1] == 3) registos_voos [indice_registo]->Marco[endereco_dia]++;
        else if (data[1] == 4) registos_voos [indice_registo]->Abril[endereco_dia]++;
        else if (data[1] == 5) registos_voos [indice_registo]->Maio[endereco_dia]++;
        else if (data[1] == 6) registos_voos [indice_registo]->Junho[endereco_dia]++;
        else if (data[1] == 7) registos_voos [indice_registo]->Julho[endereco_dia]++;
        else if (data[1] == 8) registos_voos [indice_registo]->Agosto[endereco_dia]++;
        else if (data[1] == 9) registos_voos [indice_registo]->Setembro[endereco_dia]++;
        else if (data[1] ==10) registos_voos [indice_registo]->Outubro[endereco_dia]++;
        else if (data[1] ==11) registos_voos [indice_registo]->Novembro[endereco_dia]++;
        else if (data[1] ==12) registos_voos [indice_registo]->Decembro[endereco_dia]++;    
    }

void calcular_voos_data (int data[2],btree_registos_voos** anos_registos_voos,int numeros_voos[],int tipo)
{
    int ano = data[0] - 2010;
    int i = 0;
    if (tipo == MES)
    {
        switch (data[1])
        {
            case 1:
                    for (i = 0; i < 31;i++)  numeros_voos[i] = anos_registos_voos[ano]->Janeiro[i];
                    break;
            case 2:
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Fevereiro[i];
                    break;
            case 3:
                   for (i = 0; i < 31;i++)  numeros_voos[i] = anos_registos_voos[ano]->Marco[i];
                    break;
            case 4:
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Abril[i];
                    break;
            case 5:
                   for (i = 0; i < 31;i++)  numeros_voos[i] = anos_registos_voos[ano]->Maio[i];
                    break;
            case 6:
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Junho[i];
                    break;
            case 7:     
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Julho[i];
                    break;
            case 8:
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Agosto[i];
                    break;
            case 9:
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Setembro[i];
                    break;
            case 10:
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Outubro[i];
                    break;
            case 11:
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Novembro[i];
                    break;
            case 12:
                    for (i = 0; i < 31;i++) numeros_voos[i] = anos_registos_voos[ano]->Decembro[i];
                    break;
        }
    } else if (tipo == ANO)
    {
        numeros_voos[0] = sum (anos_registos_voos[ano]->Janeiro,30);
        numeros_voos[1] = sum (anos_registos_voos[ano]->Fevereiro,30);
        numeros_voos[2] = sum (anos_registos_voos[ano]->Marco,30);
        numeros_voos[3] = sum (anos_registos_voos[ano]->Abril,30);
        numeros_voos[4] = sum (anos_registos_voos[ano]->Maio,30);
        numeros_voos[5] = sum (anos_registos_voos[ano]->Junho,30);
        numeros_voos[6] = sum (anos_registos_voos[ano]->Julho,30);
        numeros_voos[7] = sum (anos_registos_voos[ano]->Agosto,30);
        numeros_voos[8] = sum (anos_registos_voos[ano]->Setembro,30);
        numeros_voos[9] = sum (anos_registos_voos[ano]->Outubro,30);
        numeros_voos[10] = sum (anos_registos_voos[ano]->Novembro,30);
        numeros_voos[11] = sum (anos_registos_voos[ano]->Decembro,30);
    }
    else 
    {
        numeros_voos[0]  = somar_ano_voos (anos_registos_voos[0]);
        numeros_voos[1]  = somar_ano_voos (anos_registos_voos[1]);
        numeros_voos[2]  = somar_ano_voos (anos_registos_voos[2]);
        numeros_voos[3]  = somar_ano_voos (anos_registos_voos[3]);
        numeros_voos[4]  = somar_ano_voos (anos_registos_voos[4]);
        numeros_voos[5]  = somar_ano_voos (anos_registos_voos[5]);
        numeros_voos[6]  = somar_ano_voos (anos_registos_voos[6]);
        numeros_voos[7]  = somar_ano_voos (anos_registos_voos[7]);
        numeros_voos[8]  = somar_ano_voos (anos_registos_voos[8]);
        numeros_voos[9]  = somar_ano_voos (anos_registos_voos[9]);
        numeros_voos[10] = somar_ano_voos (anos_registos_voos[10]);
        numeros_voos[11] = somar_ano_voos (anos_registos_voos[11]);
        numeros_voos[12] = somar_ano_voos (anos_registos_voos[12]);
        numeros_voos[13] = somar_ano_voos (anos_registos_voos[13]);
    }
}





void querie6_interativa (GHashTable* hash_tree,int ano,int N,char* resposta,int F)
{

    size_t tamanho_array = 35;
    int indice = 0;
    int valores [tamanho_array];
    char paises[tamanho_array][4];
    int e = 0;

    for (int p = 0; p < tamanho_array;p++)
    {
        valores[p] = 0;
    }

    GHashTableIter iter;
    g_hash_table_iter_init(&iter, hash_tree);
    
    gpointer key, value;
    aeroporto_arvore* arvore = NULL;
    while (g_hash_table_iter_next(&iter, &key, &value)) 
    {
        arvore = value;
        if (ano == 2021) {valores [indice] = arvore->vinte_um; e = 1;}
        if (ano == 2022) {valores [indice] = arvore->vinte_dois;e = 1;}
        if (ano == 2023) {valores [indice] = arvore->vinte_tres; e = 1;}
        strcpy (paises[indice],arvore->pais);
        indice++;
    }

    if (e == 0) 
    {
        strcpy (resposta, "Nao existem dados para esse ano!");
        return;
    }
    
    int contador = 0;
    if (N > indice) N = indice;
    if (F == 0)
    {
    while (contador < N)
        {
        int maior = max_indi (valores,paises,indice);            //a funcao retorna o indice com o maior numero de passageiros 
        sprintf (resposta + strlen (resposta),"%s;%d\n",paises[maior],valores[maior]); //sao imprimidos n vezes 
        valores[maior] = 0; //valor alterado para 0 pois ja foi imprimido
        contador++;         
        }                   //nao foi levado em conta o caso de N > contador mas nesse caso seria imprimido um pais com 0 passageiros no ano repetidamente
    }
    else 
    {
    while (contador < N)
        {
        int maior = max_indi (valores,paises,indice);
        sprintf(resposta + strlen (resposta),"--- %d ---\n",contador + 1);           //apenas adicionamos o cabecalho 
        sprintf (resposta + strlen (resposta),"name: %s\npassengers: %d\n",paises[maior],valores[maior]);
        sprintf(resposta + strlen (resposta)," \n");
        valores[maior] = 0;
        contador++;
        }
    }

}

void print_querie_6(GHashTable* hash_tree,int ano, int N,FILE* ficheiro_output,int F)
{
    size_t tamanho_array = 35;
    int indice = 0;
    int valores [tamanho_array];
    char paises [tamanho_array][4];

    for (int p = 0; p < tamanho_array;p++)
    {
        valores[p] = 0;
    }

    GHashTableIter iter;
    g_hash_table_iter_init(&iter, hash_tree);
    
    gpointer key, value;
    aeroporto_arvore* arvore = NULL;
    while (g_hash_table_iter_next(&iter, &key, &value)) 
    {
        arvore = value;
        if (ano == 2021) valores [indice] = arvore->vinte_um;
        if (ano == 2022) valores [indice] = arvore->vinte_dois;
        if (ano == 2023) valores [indice] = arvore->vinte_tres;
        strcpy (paises[indice],arvore->pais);
        indice++;
    }
    
    int contador = 0;
    if (N > indice) N = indice;
    if (F == 0)
    {
    while (contador < N)
        {
        int maior = max_indi (valores,paises,indice);            //a funcao retorna o indice com o maior numero de passageiros 
        fprintf (ficheiro_output,"%s;%d\n",paises[maior],valores[maior]); //sao imprimidos n vezes 
        valores[maior] = 0; //valor alterado para 0 pois ja foi imprimido
        contador++;         
        }                   //nao foi levado em conta o caso de N > contador mas nesse caso seria imprimido um pais com 0 passageiros no ano repetidamente
    }
    else 
    {
    while (contador < N)
        {
        int maior = max_indi (valores,paises,indice);
        if (contador != 0) fprintf(ficheiro_output,"\n");
        fprintf(ficheiro_output,"--- %d ---\n",contador + 1);           //apenas adicionamos o cabecalho 
        fprintf (ficheiro_output,"name: %s\npassengers: %d\n",paises[maior],valores[maior]);
        valores[maior] = 0;
        contador++;
        }
    }

}

///////////////////////////////////////////////////////////QUERIE 7//////////////////////////////////////////////////////////////

void querie7_interativa (GHashTable* hash_tree,int N,char* resposta,int n_voos,int F)
{
    int medianas [N];
    char Aeroportos [N][4];
    int preenchido = 0;


    GHashTableIter iter;
    g_hash_table_iter_init(&iter, hash_tree);

    gpointer key, value;
    aeroporto_arvore* arvore = NULL;
    while (g_hash_table_iter_next(&iter, &key, &value)) 
    {
        arvore = value;
        if (arvore->Nodo != NULL)
        {
        if (preenchido < N )
            {
            medianas [preenchido] = fazer_mediana_aeroporto (arvore,n_voos);
            strcpy (Aeroportos [preenchido], arvore->pais);
            preenchido++;
            }
            else 
            {
                int mediana_nova = fazer_mediana_aeroporto (arvore,n_voos);
                int indice = substituir_menor (medianas,mediana_nova,Aeroportos,N,arvore->pais); //desempate pelo nome
                if (indice != -1) 
                {
                strcpy (Aeroportos[indice],arvore->pais);
                medianas[indice] = mediana_nova;
                } 
             } 
        }
    }


    heapsort (medianas,Aeroportos,preenchido);


    if(F == 1)
    {
        int counter = 1;

        for(int j = preenchido - 1; j >= preenchido - N && j >= 0; j--)
        {
            sprintf(resposta + strlen (resposta),"--- %d ---\n",counter);
            counter++;
            sprintf(resposta + strlen (resposta),"name: %s\n",Aeroportos[j]);
            sprintf(resposta + strlen (resposta),"median: %d\n",medianas[j]);
            sprintf(resposta + strlen (resposta)," \n");
        }
    }
    else  //quando F == 0
    {
        for(int j = preenchido - 1; j >= preenchido - N && j >= 0; j--)
        {
            sprintf(resposta + strlen (resposta),"%s;%d\n",Aeroportos[j],medianas[j]);
        }
    }

}

void print_querie_7 (GHashTable* hash_tree,int N,FILE *ficheiro_output,int n_voos,int F)
{
    int medianas [N];
    char Aeroportos [N][4];
    int preenchido = 0;


    GHashTableIter iter;
    g_hash_table_iter_init(&iter, hash_tree);

    gpointer key, value;
    aeroporto_arvore* arvore = NULL;
    while (g_hash_table_iter_next(&iter, &key, &value)) 
    {
        arvore = value;
        if (arvore->Nodo != NULL)
        {
        if (preenchido < N )
            {
            medianas [preenchido] = fazer_mediana_aeroporto (arvore,n_voos);
            strcpy (Aeroportos [preenchido], arvore->pais);
            preenchido++;
            }
            else 
            {
                int mediana_nova = fazer_mediana_aeroporto (arvore,n_voos);
                int indice = substituir_menor (medianas,mediana_nova,Aeroportos,N,arvore->pais); //desempate pelo nome
                if (indice != -1) 
                {
                strcpy (Aeroportos[indice],arvore->pais);
                medianas[indice] = mediana_nova;
                } 
             } 
        }
    }


    heapsort (medianas,Aeroportos,preenchido);


    if(F == 1)
    {
        int counter = 1;

        for(int j = preenchido - 1; j >= preenchido - N && j >= 0; j--)
        {
            if(counter != 1) fprintf(ficheiro_output,"\n");
            fprintf(ficheiro_output,"--- %d ---\n",counter);
            counter++;
            fprintf(ficheiro_output,"name: %s\n",Aeroportos[j]);
            fprintf(ficheiro_output,"median: %d\n",medianas[j]);
        }
    }
    else  //quando F == 0
    {
        for(int j = preenchido - 1; j >= preenchido - N && j >= 0; j--)
        {
            fprintf(ficheiro_output,"%s;%d\n",Aeroportos[j],medianas[j]);
        }
    }

}

int somar_ano_voos (btree_registos_voos* anos_registos_voos)
{
    int valor = 0;
       valor+= sum (anos_registos_voos->Janeiro,30);
       valor+= sum (anos_registos_voos->Fevereiro,30);
       valor+= sum (anos_registos_voos->Marco,30);
       valor+= sum (anos_registos_voos->Abril,30);
       valor+= sum (anos_registos_voos->Maio,30);
       valor+= sum (anos_registos_voos->Junho,30);
       valor+= sum (anos_registos_voos->Julho,30);
       valor+= sum (anos_registos_voos->Agosto,30);
       valor+= sum (anos_registos_voos->Setembro,30);
       valor+= sum (anos_registos_voos->Outubro,30);
       valor+= sum (anos_registos_voos->Novembro,30);
       valor+= sum (anos_registos_voos->Decembro,30);
    return valor;
}


//////////////////////////////////////////////////////////////////FREES/////////////////////////////////////////////

void free_registos_voos(btree_registos_voos** anos_registo_voos)
{
    int p ;
    for (p = 0; p < 14;p++)
    {
        free (anos_registo_voos[p]);
    }
    free (anos_registo_voos);
}


void free_aeroporto (aeroporto_arvore* aeroporto)
{
    if (aeroporto != NULL)
    {
        free_aeroporto(aeroporto->esquerda);
        free(aeroporto->Nodo);
        free_aeroporto(aeroporto->direita);
        free (aeroporto);
    }
}

void free_aeroporto_lite (aeroporto_arvore* aeroporto)
{
    if (aeroporto != NULL)
    {
        free_aeroporto_lite(aeroporto->esquerda);
        free_aeroporto_lite(aeroporto->direita);
        free (aeroporto);
    }
}