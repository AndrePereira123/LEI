#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <glib.h>
#include <wchar.h>
#include "../include/gestor_passageiro.h"
#include "../include/struct_voos.h"
#include "../include/struct_utilizador.h"
#include "../include/struct_reserva.h"
#include "../include/utilidade_tempo.h"

struct ARRAY_VOOS 
{
    int id;
    int data[3];
    array_voos* prox;
};
struct ARRAY_RESERVAS
{
    char id_reserva[15];
    int data[3];
    array_reservas* prox;
};

struct BTREE_ID_VOOS 
{
    int id_voo;
    int data[6];   // (schedule departure date)
    btree_id_voos* dir;
    btree_id_voos* esq;
};



void parser_passageiros (char** total_dados_passageiro,GHashTable* hash_id_utilizadores,void* hash_voos,int n_passageiros,GHashTable* verificar_passageiros,int interativo)
{
    int contador = 0;
    
    while (contador < n_passageiros)
    {
        char* id;
        char* save_id;
        char linha_temp [100];
        strcpy(linha_temp,total_dados_passageiro[contador]);

        id = strtok_r(linha_temp,";",&save_id);

        int id_novo_voo = atoi (id);
        id = strtok_r(NULL,"\n",&save_id); //agora id utilizador
        
        if (interativo == 1 || g_hash_table_contains (verificar_passageiros,id))
        {
        inserir_passageiro (id,hash_id_utilizadores,hash_voos,id_novo_voo);
        }
    contador++;
    }
    return;
}

btree_id_voos* criar_id_voo (int id,btree_id_voos* arvore,int data[6])
{
    arvore = malloc (sizeof (btree_id_voos)); //id data dir esq
    arvore->id_voo = id;
    arvore->data[0] = data[0];
    arvore->data[1] = data[1];
    arvore->data[2] = data[2];
    arvore->data[3] = data[3];
    arvore->data[4] = data[4];
    arvore->data[5] = data[5];
    arvore->dir = NULL;
    arvore->esq = NULL;
    return arvore;
}

btree_id_voos* inserir_id_voo (int id,btree_id_voos* arvore,int data[6])
{
    if (arvore == NULL)
    {
        arvore = criar_id_voo (id,arvore,data);
    }
    else 
    {
        int valor = compara_data_voos (arvore->data,data); 

        if (valor == 0)
        {
            arvore->dir = inserir_id_voo (id,arvore->dir,data);
        }
        else if (valor == 1)
        {
            arvore->esq = inserir_id_voo (id,arvore->esq,data);
        }
        else 
        {
            if (id > (arvore->id_voo))
            {
                arvore->dir = inserir_id_voo (id,arvore->dir,data);
            }
            else 
            {
                arvore->esq = inserir_id_voo (id,arvore->esq,data);
            }
        }
    }
    return arvore;
}

void inserir_passageiro (char* id_user,GHashTable* hash_id_users,void* hash_voos,int id_novo_voo)
{
            
        utilizador* user = g_hash_table_lookup(hash_id_users,id_user);
        if (user == NULL) return;
        btree_id_voos** arvore = retorna_arvore_voos_mae (user);

        if(*arvore == NULL)
        {
            btree_id_voos* raiz = NULL;
            int data[6];
            departure_date (hash_do_voo (id_novo_voo,hash_voos),data);
            raiz = criar_id_voo (id_novo_voo,raiz,data);
            *arvore = raiz;
        }
        else 
        {
           int data[6];
           departure_date (hash_do_voo (id_novo_voo,hash_voos),data);
           *arvore = inserir_id_voo (id_novo_voo,*arvore,data); 
        }
}

/////////////////////////////////////////////////QUERIE 2////////////////////////////////////////////////////////////////////
void querie_2_Total_interativo (char* username,GHashTable* dados_utilizadores_id,char* resposta,int F)
{
    if (!g_hash_table_contains (dados_utilizadores_id,username)) {sprintf (resposta,"O utilizador nao existe\n"); return;}
    utilizador* user = g_hash_table_lookup (dados_utilizadores_id,username);

    int contador = 1;
    
    print_querie2_Total_interativo (username,retorna_arvore_reservas (user),retorna_arvore_voos(user),resposta,&contador,F);

    print_querie2_Total_interativo (username,retorna_arvore_reservas (user),retorna_arvore_voos(user),resposta,&contador,F);
}

void print_querie2_Total_interativo(char *username,void* dados_utilizadores_id,btree_id_voos* dados_passageiros,char* resposta,int *contador,int F)
{
    array_voos* voos = NULL; 
    guardar_btree_id_voos (&voos,dados_passageiros);
    array_reservas* reservas = NULL;
    guardar_btree_id_reservas (&reservas,dados_utilizadores_id);

    array_voos* atual_voos = voos;
    array_reservas* atual_reservas = reservas;

    while (atual_voos != NULL && atual_reservas != NULL)
    {
        int res = comparar_datas (atual_voos->data, atual_reservas->data); // 1 = primeiro é maior e 0 = o segundo é maior
        if (F == 0)
    {
        if (res == 1)
        {
            sprintf (resposta + strlen(resposta),"%010d;%d/%02d/%02d;flight\n",atual_voos->id,atual_voos->data[0],atual_voos->data[1],atual_voos->data[2]);
            atual_voos = atual_voos->prox;
        }
        else if (res == 0)
        {
            sprintf (resposta + strlen(resposta),"%s;%d/%02d/%02d;reservation\n",atual_reservas->id_reserva,atual_reservas->data[0],atual_reservas->data[1],atual_reservas->data[2]);
            atual_reservas = atual_reservas->prox;
        }
        else
        {
            if (atual_voos->id < atoi(atual_reservas->id_reserva + 4))
            {
                sprintf (resposta + strlen(resposta),"%010d;%d/%02d/%02d;flight\n",atual_voos->id,atual_voos->data[0],atual_voos->data[1],atual_voos->data[2]);
                atual_voos = atual_voos->prox;
            }
            else 
            {
                sprintf (resposta + strlen(resposta),"%s;%d/%02d/%02d;reservation\n",atual_reservas->id_reserva,atual_reservas->data[0],atual_reservas->data[1],atual_reservas->data[2]);
                atual_reservas = atual_reservas->prox;
            }
        }
    while (atual_reservas != NULL)
    {
        sprintf (resposta + strlen(resposta),"%s;%d/%02d/%02d;reservation\n",atual_reservas->id_reserva,atual_reservas->data[0],atual_reservas->data[1],atual_reservas->data[2]);
        atual_reservas = atual_reservas->prox;
    }
    while (atual_voos != NULL)
    {
        sprintf (resposta + strlen(resposta),"%010d;%d/%02d/%02d;flight\n",atual_voos->id,atual_voos->data[0],atual_voos->data[1],atual_voos->data[2]);
        atual_voos = atual_voos->prox;
    }
    }
    else 
    {
        if (res == 1)
        {
            querie2_print_voo_F_interativo (atual_voos->id,atual_voos->data,resposta,contador);
            atual_voos = atual_voos->prox;
        }
        else if (res == 0)
        {
            querie2_print_reserva_F_interativo (atual_reservas->id_reserva,atual_reservas->data,resposta,contador);
            atual_reservas = atual_reservas->prox;
        }
        else 
        {
            if (atual_voos->id < atoi(atual_reservas->id_reserva + 4))
            {
                querie2_print_voo_F_interativo (atual_voos->id,atual_voos->data,resposta,contador);
                atual_voos = atual_voos->prox;
            }
            else 
            {
                querie2_print_reserva_F_interativo (atual_reservas->id_reserva,atual_reservas->data,resposta,contador);
                atual_reservas = atual_reservas->prox;
            }
        }

    while (atual_reservas != NULL)
    {
        querie2_print_reserva_F_interativo (atual_reservas->id_reserva,atual_reservas->data,resposta,contador);
        atual_reservas = atual_reservas->prox;
    }
    while (atual_voos != NULL)
    {
        querie2_print_voo_F_interativo (atual_voos->id,atual_voos->data,resposta,contador);
        atual_voos = atual_voos->prox;
    }
    }
    }
    free_stack_voos (voos);
    free_stack_reservas (reservas);
}

void querie_2_Total (char *username,GHashTable* dados_utilizadores_id,FILE *ficheiro_output,int F)
{
     if (!g_hash_table_contains (dados_utilizadores_id,username)) return;
    utilizador* user = g_hash_table_lookup (dados_utilizadores_id,username);

    int contador = 1;
    if(F == 1) 
    {
        print_querie2_totalF(username,retorna_arvore_reservas (user),retorna_arvore_voos(user),ficheiro_output,&contador);
    }
    else 
    {
        print_querie2_total(username,retorna_arvore_reservas (user),retorna_arvore_voos(user),ficheiro_output);
    }
}

void print_querie2_total(char *username,void* dados_utilizadores_id,btree_id_voos* dados_passageiros,FILE* ficheiro_output)
{
    array_voos* voos = NULL; 
    guardar_btree_id_voos (&voos,dados_passageiros);
    array_reservas* reservas = NULL;
    guardar_btree_id_reservas (&reservas,dados_utilizadores_id);
    
    array_voos* atual_voos = voos;
    array_reservas* atual_reservas = reservas;
    
    while (atual_voos != NULL && atual_reservas != NULL)
    {
        int res = comparar_datas (atual_voos->data, atual_reservas->data); // 1 = primeiro é maior e 0 = o segundo é maior
        if (res == 1)
        {
            fprintf (ficheiro_output,"%010d;%d/%02d/%02d;flight\n",atual_voos->id,atual_voos->data[0],atual_voos->data[1],atual_voos->data[2]);
            atual_voos = atual_voos->prox;
        }
        else if (res == 0)
        {
            fprintf (ficheiro_output,"%s;%d/%02d/%02d;reservation\n",atual_reservas->id_reserva,atual_reservas->data[0],atual_reservas->data[1],atual_reservas->data[2]);
            atual_reservas = atual_reservas->prox;
        }
        else
        {
            if (atual_voos->id < atoi(atual_reservas->id_reserva + 4))
            {
                fprintf (ficheiro_output,"%010d;%d/%02d/%02d;flight\n",atual_voos->id,atual_voos->data[0],atual_voos->data[1],atual_voos->data[2]);
                atual_voos = atual_voos->prox;
            }
            else 
            {
                fprintf (ficheiro_output,"%s;%d/%02d/%02d;reservation\n",atual_reservas->id_reserva,atual_reservas->data[0],atual_reservas->data[1],atual_reservas->data[2]);
                atual_reservas = atual_reservas->prox;
            }
        }
    }

    while (atual_reservas != NULL)
    {
        fprintf (ficheiro_output,"%s;%d/%02d/%02d;reservation\n",atual_reservas->id_reserva,atual_reservas->data[0],atual_reservas->data[1],atual_reservas->data[2]);
        atual_reservas = atual_reservas->prox;
    }
    while (atual_voos != NULL)
    {
        fprintf (ficheiro_output,"%010d;%d/%02d/%02d;flight\n",atual_voos->id,atual_voos->data[0],atual_voos->data[1],atual_voos->data[2]);
        atual_voos = atual_voos->prox;
    }

    free_stack_voos (voos);
    free_stack_reservas (reservas);
}



void print_querie2_totalF(char *username,void* dados_utilizadores_id,btree_id_voos* dados_passageiros,FILE* ficheiro_output,int *contador)
{
    
    array_voos* voos = NULL; 
    guardar_btree_id_voos (&voos,dados_passageiros);
    array_reservas* reservas = NULL;
    guardar_btree_id_reservas (&reservas,dados_utilizadores_id);;

    array_voos* atual_voos = voos;
    array_reservas* atual_reservas = reservas;
    
    while (atual_voos != NULL && atual_reservas != NULL)
    {
        int res = comparar_datas (atual_voos->data, atual_reservas->data); // 1 = primeiro é maior e 0 = o segundo é maior
        if (res == 1)
        {
            querie2_print_voo_F (atual_voos->id,atual_voos->data,ficheiro_output,contador);
            atual_voos = atual_voos->prox;
        }
        else if (res == 0)
        {
            querie2_print_reserva_F (atual_reservas->id_reserva,atual_reservas->data,ficheiro_output,contador);
            atual_reservas = atual_reservas->prox;
        }
        else 
        {
            if (atual_voos->id < atoi(atual_reservas->id_reserva + 4))
            {
                querie2_print_voo_F (atual_voos->id,atual_voos->data,ficheiro_output,contador);
                atual_voos = atual_voos->prox;
            }
            else 
            {
                querie2_print_reserva_F (atual_reservas->id_reserva,atual_reservas->data,ficheiro_output,contador);
                atual_reservas = atual_reservas->prox;
            }
        }
    }

    while (atual_reservas != NULL)
    {
        querie2_print_reserva_F (atual_reservas->id_reserva,atual_reservas->data,ficheiro_output,contador);
        atual_reservas = atual_reservas->prox;
    }
    while (atual_voos != NULL)
    {
        querie2_print_voo_F (atual_voos->id,atual_voos->data,ficheiro_output,contador);
        atual_voos = atual_voos->prox;
    }

    free_stack_voos (voos);
    free_stack_reservas (reservas);
}


void querie2_print_reserva_F_interativo (char* id,int data[3],char* resposta,int* contador)
{
    
        sprintf (resposta + strlen (resposta),"--- %d ---\n",*contador);
        (*contador)++;
        sprintf (resposta + strlen (resposta),"id: %s\n",id);
        sprintf (resposta + strlen (resposta),"date: %d/%02d/%02d\n",data[0],data[1],data[2]);
        sprintf (resposta + strlen (resposta),"type: reservation\n \n");
}

void querie2_print_reserva_F (char* id,int data[3],FILE* ficheiro_output,int* contador)
{
     if (*contador > 1) fprintf (ficheiro_output,"\n");
        fprintf (ficheiro_output,"--- %d ---\n",*contador);
        (*contador)++;
        fprintf (ficheiro_output,"id: %s\n",id);
        fprintf (ficheiro_output,"date: %d/%02d/%02d\n",data[0],data[1],data[2]);
        fprintf (ficheiro_output,"type: reservation\n");
}

void querie2_print_voo_F_interativo (int id,int data[3],char* resposta,int* contador)
{
    
        sprintf (resposta + strlen (resposta),"--- %d ---\n",*contador);
        (*contador)++;
        sprintf (resposta+ strlen(resposta),"id: %010d\n",id);
        sprintf (resposta+ strlen(resposta),"date: %d/%02d/%02d\n",data[0],data[1],data[2]);
        sprintf (resposta+ strlen(resposta),"type: flight\n \n");
}

void querie2_print_voo_F (int id,int data[3],FILE* ficheiro_output,int* contador)
{
     if (*contador > 1) fprintf (ficheiro_output,"\n");
        fprintf (ficheiro_output,"--- %d ---\n",*contador);
        (*contador)++;
        fprintf (ficheiro_output,"id: %010d\n",id);
        fprintf (ficheiro_output,"date: %d/%02d/%02d\n",data[0],data[1],data[2]);
        fprintf (ficheiro_output,"type: flight\n");
}

void guardar_btree_id_voos (array_voos** voos,btree_id_voos* arvore_id_voos) //guardar em array para print ordenado na querie 2
{
    if (arvore_id_voos != NULL)
    {
        guardar_btree_id_voos (voos,arvore_id_voos->esq);
        if ((*voos) == NULL)
        {
            array_voos* novo = malloc (sizeof(array_voos));
            novo->prox = NULL;
            novo->id = arvore_id_voos->id_voo;
            novo->data[0] = arvore_id_voos->data[0];
            novo->data[1] = arvore_id_voos->data[1];
            novo->data[2] = arvore_id_voos->data[2];
            (*voos) = novo;
        }
        else 
        {
            array_voos* atual = (*voos);
            array_voos* novo = malloc (sizeof (array_voos));
            novo->prox = atual;
            novo->id = arvore_id_voos->id_voo;
            novo->data[0] = arvore_id_voos->data[0];
            novo->data[1] = arvore_id_voos->data[1];
            novo->data[2] = arvore_id_voos->data[2]; 
            (*voos) = novo;
        }
        guardar_btree_id_voos (voos,arvore_id_voos->dir);
    }
}
   
////////////////////////////////////////////////////QUERIE 2 VOOS///////////////////////////////////////////////////////

void print_voos_q2_interativo (btree_id_voos* arvore_voos,char* resposta,int* contador,int F)
{
    if (arvore_voos == NULL) return;
    if (F == 0)
    {
        print_voos_q2_interativo (arvore_voos->dir,resposta,contador,F);
        sprintf (resposta + strlen(resposta),"%010d;%d/%02d/%02d\n",arvore_voos->id_voo,arvore_voos->data[0],arvore_voos->data[1],arvore_voos->data[2]);
        print_voos_q2_interativo (arvore_voos->esq,resposta,contador,F);
    }
    {
        print_voos_q2_interativo (arvore_voos->dir,resposta,contador,F);
        
        sprintf (resposta + strlen(resposta),"--- %d ---\n",*contador);
        (*contador)++;
        sprintf (resposta + strlen(resposta),"id: %010d\n",arvore_voos->id_voo);
        sprintf (resposta + strlen(resposta),"date: %d/%02d/%02d\n",arvore_voos->data[0],arvore_voos->data[1],arvore_voos->data[2]);
        sprintf (resposta + strlen(resposta)," \n");
        print_voos_q2_interativo (arvore_voos->esq,resposta,contador,F);
    }
}

void querie_2_Voos (char* username,GHashTable* dados_utilizadores_id,FILE* ficheiro_output,int F)
{

    utilizador* user = g_hash_table_lookup (dados_utilizadores_id,username);
        int contador = 1;
        if (F == 1) {print_voos_q2_F (retorna_arvore_voos(user),ficheiro_output,&contador);} 
        else print_voos_q2 (retorna_arvore_voos(user),ficheiro_output);
}

void print_voos_q2 (btree_id_voos* arvore_voos,FILE* ficheiro_output)
{
    if (arvore_voos == NULL) return;
        print_voos_q2 (arvore_voos->dir,ficheiro_output);
        fprintf (ficheiro_output,"%010d;%d/%02d/%02d\n",arvore_voos->id_voo,arvore_voos->data[0],arvore_voos->data[1],arvore_voos->data[2]);
        print_voos_q2 (arvore_voos->esq,ficheiro_output);
}

void print_voos_q2_F (btree_id_voos* arvore_voos,FILE* ficheiro_output,int* contador)
{
     if (arvore_voos == NULL) return;
        print_voos_q2_F (arvore_voos->dir,ficheiro_output,contador);
        if (*contador > 1) fprintf (ficheiro_output,"\n");
        fprintf (ficheiro_output,"--- %d ---\n",*contador);
        (*contador)++;
        fprintf (ficheiro_output,"id: %010d\n",arvore_voos->id_voo);
        fprintf (ficheiro_output,"date: %d/%02d/%02d\n",arvore_voos->data[0],arvore_voos->data[1],arvore_voos->data[2]);
        print_voos_q2_F (arvore_voos->esq,ficheiro_output,contador);
}

///////////////////////////////////////////////////////////////QUERIE 10//////////////////////////////////////////
void contador_de_passageiros (char** matriz_passageiros,int linhas_matriz,int* voos_a_contar,int* dia_do_voo,int* passageiros,int* passageiros_unicos)
{
    GHashTable* hash_tables[31];

    
    for (int i = 0; i < 31; ++i) {
        hash_tables[i] = g_hash_table_new(g_str_hash, g_str_equal);
    }

    int linhas_percorridas = 0;
    char* token;
    char* save_token;
    int id_voo,dia;
    
    while(linhas_percorridas < linhas_matriz)
    {
        id_voo = atoi(matriz_passageiros[linhas_percorridas]) - 1;
    
       
        if (voos_a_contar[id_voo] == 1) //se for para contar o voo
        {

        dia = dia_do_voo[id_voo];
        passageiros[dia - 1]++;  //somamos um passageiro no dia correto

        token = strtok_r (matriz_passageiros[linhas_percorridas] + 11,"\n",&save_token);

        
         if (g_hash_table_add (hash_tables[dia-1],(token)))  //se conseguiu adicionar um novo passageiro na hash
        {
            passageiros_unicos[dia - 1]++;   //guardamos nos passageiros unicos
        }
        }
        
        linhas_percorridas++;
    }

    for (int i = 0; i < 31; ++i) {
        g_hash_table_destroy (hash_tables[i]);
    }
}


void contador_de_passageiros_mes (char** matriz_passageiros,int linhas_matriz,int* voos_a_contar,int* mes_do_voo,int* passageiros,int* passageiros_unicos)
{
    GHashTable* hash_tables[12];

    
    for (int i = 0; i < 12; ++i) {
        hash_tables[i] = g_hash_table_new(g_str_hash, g_str_equal);
    }

    int linhas_percorridas = 0;
    char* token;
    char* save_token;
    int id_voo,mes;
    
    while(linhas_percorridas < linhas_matriz)
    {
        id_voo = atoi(matriz_passageiros[linhas_percorridas]) - 1;

        if (voos_a_contar[id_voo] == 1) //se for para contar o voo
        {
        mes = mes_do_voo[id_voo];
        passageiros[mes - 1]++;  //somamos um passageiro no mes correto

        token = strtok_r (matriz_passageiros[linhas_percorridas] + 11,"\n",&save_token);

         if ( g_hash_table_add (hash_tables[mes-1],(token))) //se conseguiu adicionar um novo passageiro na hash
        {
            passageiros_unicos[mes - 1]++;   //guardamos nos passageiros unicos
        }
        }
        linhas_percorridas++;
    }


    for (int i = 0; i < 12; ++i) {
        g_hash_table_destroy (hash_tables[i]);
    }
}

void contador_de_passageiros_total (char** matriz_passageiros,int linhas_matriz,int* ano_do_voo,int* passageiros,int* passageiros_unicos)
{
    GHashTable* hash_tables[14];

    
    for (int i = 0; i < 14; ++i) {
        hash_tables[i] = g_hash_table_new(g_str_hash, g_str_equal);
    }
    int linhas_percorridas = 0;
    char* token;
    char* save_token;
    int id_voo,ano;
    
    while(linhas_percorridas < linhas_matriz)
    {
        id_voo = atoi(matriz_passageiros[linhas_percorridas]) - 1;
    
        
        ano = ano_do_voo[id_voo];
        passageiros[ano - 2010]++;  //somamos um passageiro no ano correto

        token = strtok_r (matriz_passageiros[linhas_percorridas] + 11,"\n",&save_token);

         if (g_hash_table_add (hash_tables[ano-2010],(token))) //se conseguiu adicionar um novo passageiro na hash
        {
            passageiros_unicos[ano - 2010]++;   //guardamos nos passageiros unicos  
        }
    
        linhas_percorridas++;
    }

    for (int i = 0; i < 14; ++i) {
        g_hash_table_destroy (hash_tables[i]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void free_arvore_id_voos (btree_id_voos* arvore)
{
    if (arvore != NULL)
    {
        free_arvore_id_voos(arvore->dir);
        free_arvore_id_voos(arvore->esq);
    }
    free(arvore);
}

void  free_stack_voos (array_voos* v)
{
    if (v == NULL) return;
   if (v->prox != NULL)
   {
    free_stack_voos (v->prox);
   }
   free(v);
}
void free_stack_reservas (array_reservas* r)
{
    if (r == NULL) return;
    if (r->prox != NULL)
    {
        free_stack_reservas (r->prox);
    }
    free(r);
}