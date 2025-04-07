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


#define MES 1
#define ANO 2

struct VOO
{
   char id[11]; 
   char airline[35];
   char model[20];
   char total_seats[4];
   char origin[4];
   char destination[4];
   int schedule_departure_date[6];
   int schedule_arrival_date[6];
   int real_departure_date[6];
   int real_arrival_date[6];
};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* parser_voos (char** total_dados_voos,GHashTable* hash_tree,int* passageiros_voo, voo*** hash_voos,int n_voos_validos,btree_registos_voos** registos_voos,aeroporto_arvore** arvore_todos_voos) 
{
    hash_tree = g_hash_table_new(g_str_hash,g_str_equal);
    voo** hash_voo = *hash_voos;
    aeroporto_arvore* arvore_todos_voos_local = NULL;

    int contador = 0;                                  
    int t = 0;

    while (contador < n_voos_validos) 
    {
        char dados_voos [13][126]; 
        char linha_temp [520];
        strcpy (linha_temp,total_dados_voos[contador]);
    
        dividirLinha_voo (linha_temp,dados_voos);    //separamos os elementos de cada voo e guardamos numa struct voo

        voo* voo = cria_voo();
        guarda_voo_T(voo,dados_voos[0],dados_voos[1],dados_voos[2],dados_voos[3],dados_voos[4],dados_voos[5],dados_voos[6], 
                         dados_voos[7],dados_voos[8],dados_voos[9],dados_voos[10],dados_voos[11],dados_voos[12]);
        


        inserir_registo_voo (voo->schedule_departure_date,registos_voos);

        
        t = atoi(dados_voos[0]) -1;


        hash_voo[t] = voo;
           
        arvore_todos_voos_local = inserir_voo (arvore_todos_voos_local,voo,passageiros_voo,t);


        gpointer hash = g_hash_table_lookup(hash_tree,voo->origin);
        gpointer hash_destination = g_hash_table_lookup(hash_tree,voo->destination);       //atraves do id do aerorporto de chegada e destino

        if(hash == NULL)
        {                                                                           //inserimos o voo e contabilizamos o numero de passageiros para ambos os aeroportos
            aeroporto_arvore* nova_arvore = NULL;
            nova_arvore = inserir_voo (nova_arvore,voo,passageiros_voo,t);
            g_hash_table_insert (hash_tree,voo->origin,nova_arvore);
        }
        else 
        {   
            inserir_voo (hash,voo,passageiros_voo,t);
        }

        if(hash_destination == NULL) 
            {
                aeroporto_arvore* arvore_vazia_destino = NULL;
                arvore_vazia_destino = contar_destination (arvore_vazia_destino,voo,passageiros_voo,t);
                g_hash_table_insert (hash_tree,voo->destination,arvore_vazia_destino);
            } 
            else 
            {
                contar_destination (hash_destination,voo,passageiros_voo,t);
            }
            
        contador++;
     }
    
    *arvore_todos_voos = arvore_todos_voos_local;
    *hash_voos = hash_voo;
    return hash_tree;
}




voo* hash_do_voo (int id_novo_voo,voo** hash_voos)
{
    return (hash_voos[id_novo_voo-1]);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




int compara_data_voos(int data_inicial[6], int data_final[6]) {
    for (int i = 0; i < 6; i++)                                 
    {
        if (data_inicial[i] > data_final[i]) 
        {
            return 1; // 1 se 1 for maior
        } else if (data_inicial[i] < data_final[i]) {
            return 2;  // 2 se 2 for maior
        }
    }
    return 3;  //3 se for igual 
}    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

voo* cria_voo() 
{
    voo* novo_aeroporto = malloc(sizeof(struct VOO));
    return novo_aeroporto;
}

voo* guarda_voo_T (voo* novo_voo ,char* id,char* airline,char* model,char* total_seats,char* origin,char* destination,char* schedule_departure_date
                         ,char* schedule_arrival_date,char* real_departure_date,char* real_arrival_date,char* pilot,char* copilot,char* notes) 
{
        
        strcpy (novo_voo->id,id);
        strcpy (novo_voo->airline,airline);
        strcpy (novo_voo->model,model);
        strcpy (novo_voo->total_seats,total_seats);
        strcpy (novo_voo->origin,origin);
        strcpy (novo_voo->destination,destination);
        for(int i = 0 ; i < 3; i++)
        {
           novo_voo->origin[i] = toupper(novo_voo->origin[i]);
           novo_voo->destination[i] = toupper(novo_voo->destination[i]);
        }
        converter_schedule (schedule_departure_date,novo_voo->schedule_departure_date);
        converter_schedule (schedule_arrival_date,novo_voo->schedule_arrival_date);     //guardamos as datas como inteiros
        converter_schedule (real_departure_date,novo_voo->real_departure_date);
        converter_schedule (real_arrival_date,novo_voo->real_arrival_date);
        return novo_voo;
}

void converter_schedule (char letras[],int data[])
{
    char* token;
    char* save_tok;
    token = strtok_r(letras,"/",&save_tok);  //pegar ano
    data[0] = atoi (token);
    token = strtok_r(NULL,"/",&save_tok); //pegar mes
    data[1] = atoi (token);
    token = strtok_r(NULL," ",&save_tok); //pegar o dia
    data[2] = atoi (token);
    token = strtok_r(NULL,":",&save_tok); //pegar as horas
    data[3] = atoi (token);
    token = strtok_r(NULL,":",&save_tok); //pegar os minutos
    data[4] = atoi (token);
    const char* delimiter = "\"";
    token = strtok_r(NULL,delimiter,&save_tok); //pegar os segundos
    data[5] = atoi (token);
}

void departure_date_simple (voo* viagem,int data_saida[3])
{
    for (int i = 0; i < 3; i++)
    {
    data_saida[i] = viagem->schedule_departure_date[i];
    }
}

void departure_date (voo* viagem,int data_saida[6])
{
    for (int i = 0; i < 6; i++)
    {
    data_saida[i] = viagem->schedule_departure_date[i];
    }
}

int* retorna_s_departure_date (voo* v)
{
    return (v->schedule_departure_date);
}

int* retorna_r_departure_date (voo* v)
{
    return (v->real_departure_date);
}

int* retorna_s_arrival_date (voo* v)
{
    return (v->schedule_arrival_date);
}

char* retorna_destination (voo* v)
{
    return v->destination;
}

char* retorna_origin (voo* v)
{
    return v->origin;
}

int retorna_id (voo* voo_novo)
{
    return (atoi (voo_novo->id));
}

char* retorna_id_char (voo* voo_novo)
{
    return (voo_novo->id);
}

char* retorna_airline (voo* voo_novo)
{
    return (voo_novo->airline);
}


char* retorna_model (voo* voo_novo)
{
    return (voo_novo->model);
}






//////////////////////////////////////////////QUERIE 1 V///////////////////////////////////////////////////////////////

int atraso_aviao (int data_real [6],int data_planeada [6]) 
{
    int segundos_r = 0;
    int segundos_p = 0;

    segundos_r += data_real[5];
    segundos_r += data_real[4] * 60;
    segundos_r += data_real[3] * 3600;
    segundos_p += data_planeada[5];
    segundos_p += data_planeada[4] * 60;
    segundos_p += data_planeada[3] * 3600;
    int res = segundos_r - segundos_p;
    return (res > 0 ? res : 0);
}

void querie1_V_interativa (voo** hash_voos,int* passageiros_voo,char* id,char*resposta,int F)
{
    int select = atoi(id) - 1;
    if (hash_voos[select] == NULL)
    {
        sprintf (resposta, "O voo nao existe\n");
    }
    else 
    {
    int atraso = atraso_aviao (hash_voos[select]->real_departure_date,hash_voos[select]->schedule_departure_date);
    if (F == 0)
    {
    sprintf(resposta,"%s;%s;%s;%s;%d/%02d/%02d %02d:%02d:%02d;%d/%02d/%02d %02d:%02d:%02d;%d;%d\n",
    hash_voos[select]->airline,
    hash_voos[select]->model,
    hash_voos[select]->origin,
    hash_voos[select]->destination,
    hash_voos[select]->schedule_departure_date [0],
    hash_voos[select]->schedule_departure_date [1],
    hash_voos[select]->schedule_departure_date [2],
    hash_voos[select]->schedule_departure_date [3],
    hash_voos[select]->schedule_departure_date [4],
    hash_voos[select]->schedule_departure_date [5],
    hash_voos[select]->schedule_arrival_date [0],
    hash_voos[select]->schedule_arrival_date [1],
    hash_voos[select]->schedule_arrival_date [2],
    hash_voos[select]->schedule_arrival_date [3],
    hash_voos[select]->schedule_arrival_date [4],
    hash_voos[select]->schedule_arrival_date [5],
    passageiros_voo [select],
    atraso
    );
     }
    else 
    {
    sprintf(resposta,"--- 1 ---\nairline: %s\nplane_model: %s\norigin: %s\ndestination: %s\nschedule_departure_date: %d/%02d/%02d %02d:%02d:%02d\nschedule_arrival_date: %d/%02d/%02d %02d:%02d:%02d\npassengers: %d\ndelay: %d\n",
    hash_voos[select]->airline,
    hash_voos[select]->model,
    hash_voos[select]->origin,
    hash_voos[select]->destination,
    hash_voos[select]->schedule_departure_date [0],
    hash_voos[select]->schedule_departure_date [1],
    hash_voos[select]->schedule_departure_date [2],
    hash_voos[select]->schedule_departure_date [3],
    hash_voos[select]->schedule_departure_date [4],
    hash_voos[select]->schedule_departure_date [5],
    hash_voos[select]->schedule_arrival_date [0],
    hash_voos[select]->schedule_arrival_date [1],
    hash_voos[select]->schedule_arrival_date [2],
    hash_voos[select]->schedule_arrival_date [3],
    hash_voos[select]->schedule_arrival_date [4],
    hash_voos[select]->schedule_arrival_date [5],
    passageiros_voo [select],
    atraso
    );
    }
    }
}

void print_querie_1_V (voo** hash_voos,int* passageiros_voo,char* id,FILE* ficheiro_output,int F)
{
    int select = atoi(id) - 1;
    if (hash_voos[select] != NULL)
    {
    int atraso = atraso_aviao (hash_voos[select]->real_departure_date,hash_voos[select]->schedule_departure_date);
    if (F == 0)
    {
    fprintf(ficheiro_output,"%s;%s;%s;%s;%d/%02d/%02d %02d:%02d:%02d;%d/%02d/%02d %02d:%02d:%02d;%d;%d\n",
    hash_voos[select]->airline,
    hash_voos[select]->model,
    hash_voos[select]->origin,
    hash_voos[select]->destination,
    hash_voos[select]->schedule_departure_date [0],
    hash_voos[select]->schedule_departure_date [1],
    hash_voos[select]->schedule_departure_date [2],
    hash_voos[select]->schedule_departure_date [3],
    hash_voos[select]->schedule_departure_date [4],
    hash_voos[select]->schedule_departure_date [5],
    hash_voos[select]->schedule_arrival_date [0],
    hash_voos[select]->schedule_arrival_date [1],
    hash_voos[select]->schedule_arrival_date [2],
    hash_voos[select]->schedule_arrival_date [3],
    hash_voos[select]->schedule_arrival_date [4],
    hash_voos[select]->schedule_arrival_date [5],
    passageiros_voo [select],
    atraso
    );
     }
    else 
    {
    fprintf(ficheiro_output,"--- 1 ---\nairline: %s\nplane_model: %s\norigin: %s\ndestination: %s\nschedule_departure_date: %d/%02d/%02d %02d:%02d:%02d\nschedule_arrival_date: %d/%02d/%02d %02d:%02d:%02d\npassengers: %d\ndelay: %d\n",
    hash_voos[select]->airline,
    hash_voos[select]->model,
    hash_voos[select]->origin,
    hash_voos[select]->destination,
    hash_voos[select]->schedule_departure_date [0],
    hash_voos[select]->schedule_departure_date [1],
    hash_voos[select]->schedule_departure_date [2],
    hash_voos[select]->schedule_departure_date [3],
    hash_voos[select]->schedule_departure_date [4],
    hash_voos[select]->schedule_departure_date [5],
    hash_voos[select]->schedule_arrival_date [0],
    hash_voos[select]->schedule_arrival_date [1],
    hash_voos[select]->schedule_arrival_date [2],
    hash_voos[select]->schedule_arrival_date [3],
    hash_voos[select]->schedule_arrival_date [4],
    hash_voos[select]->schedule_arrival_date [5],
    passageiros_voo [select],
    atraso
    );
    }
    }
}



void swap(int *indice_nova_mediana,int *indice_mediana_antiga,char aeroportos_novo[4],char aeroporto_antigo[4]) 
{
    int temp_mediana = *indice_nova_mediana;
    char temp_aeroporto[30];
    strcpy(temp_aeroporto,aeroportos_novo);

    *indice_nova_mediana = *indice_mediana_antiga;
    *indice_mediana_antiga = temp_mediana; 

    strcpy(aeroportos_novo,aeroporto_antigo);
    strcpy(aeroporto_antigo,temp_aeroporto);

}

void heapify(int medianas[], char Aeroportos[][4], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n)
    {
       if (medianas[left] > medianas[largest] || (medianas[left] == medianas[largest] && (strcmp (Aeroportos[left],Aeroportos[largest])) < 0)) {
           largest = left;
       }
    }
    if (right < n)
    {
      if (medianas[right] > medianas[largest] || (medianas[right] == medianas[largest] && (strcmp (Aeroportos[right],Aeroportos[largest])) < 0)) {
          largest = right;
      }
    }
    if (largest != i)
        {
        swap(&medianas[i], &medianas[largest], Aeroportos[i], Aeroportos[largest]);
        heapify(medianas, Aeroportos, n, largest);
        }
}

void heapsort(int medianas[], char Aeroportos[][4], int n) {
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(medianas, Aeroportos, n, i);
    }

    // One by one extract an element from the heap
    for (int i = n - 1; i > 0; i--) {
        swap(&medianas[0], &medianas[i], Aeroportos[0], Aeroportos[i]);
        heapify(medianas, Aeroportos, i, 0);
    }
}

int substituir_menor (int medianas[],int mediana_nova,char Aeroportos[][4],int N,char pais[4])
{
    int indice_menor,menor_valor,i;
    indice_menor = -1;
    menor_valor = mediana_nova;
    for (i = 0; i < N ;i++)
    {
        if (medianas [i] < menor_valor)
        {
        menor_valor = medianas[i];
        indice_menor = i;
        }
        else if (menor_valor == medianas[i])
        {
            if (indice_menor != -1 )
            {
            if (strcmp (Aeroportos[i],Aeroportos[indice_menor]) > 0) indice_menor = i;
            }
            else 
            {
            if (strcmp (Aeroportos[i],pais) > 0) indice_menor = i;
            }
        }
    }
    return indice_menor;
}

void merge(int arr[], int left[], int left_size, int right[], int right_size) {
    int i = 0, j = 0, k = 0;

    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < left_size) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < right_size) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int size) {
    if (size > 1) {
        int mid = size / 2;
        int *left = (int *)malloc(mid * sizeof(int));
        int *right = (int *)malloc((size - mid) * sizeof(int));

        for (int i = 0; i < mid; i++) {
            left[i] = arr[i];
        }

        for (int i = mid; i < size; i++) {
            right[i - mid] = arr[i];
        }

        merge_sort(left, mid);
        merge_sort(right, size - mid);
        merge(arr, left, mid, right, size - mid);

        free(left);
        free(right);
    }
}

int calcula_atraso_voo (int data_prevista[6],int data_real [6])
{
    
    int atraso = 0;
    int calcular_dias_prevista [3] = {data_prevista[0],data_prevista[1],data_prevista[2]};
    int calcular_dias_real [3] = {data_real[0],data_real[1],data_real[2]};
    atraso -= (diferenca_dias_atrasos (calcular_dias_prevista,calcular_dias_real)) * 3600 *24;
    atraso -= (data_prevista[3] - data_real[3]) * 3600;
    atraso -= (data_prevista[4] - data_real[4]) * 60;
    atraso -= data_prevista[5] - data_real[5];

    if (atraso < 0) return 0;
    return atraso;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int max_indi (int* valores,char paises[][4],int n)
{
    int maior_i = 0,i = 0;
    int valor_maior = 0;
    while (i < n)
    {
            if (valores[i] > valor_maior)
            {
                valor_maior = valores[i];
                maior_i = i;
            }
            else if (valores[i] == valor_maior && strcmp (paises [i],paises[maior_i])  < 0)
            {
                maior_i = i;
            }
        i++;
    }
    return maior_i;
}




int compara_ids_voo (char* id1,char* id2)
{
    if (atoi (id1) > atoi (id2)) 
    {
        return 1;
    }
    return 0;
}

///////////////////////////////////////////////////frees//////////////////////////////////////////////////




void free_voos (GHashTable* hash_aeroportos,voo** hash_voos)
{
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, hash_aeroportos);
    
    gpointer key, value;
    while (g_hash_table_iter_next(&iter, &key, &value)) 
    {
        aeroporto_arvore* destruir = value;
        free_aeroporto_lite (destruir);
    }
    free (hash_voos);

    g_hash_table_destroy (hash_aeroportos);
}


