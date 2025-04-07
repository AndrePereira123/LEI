#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <glib.h>
#include "../include/erros.h"
#include "../include/struct_utilizador.h"
#include "../include/struct_reserva.h"
#include "../include/utilidade_tempo.h"


#define RESERVA 0
#define VOO 1
#define PASSAGEIROS 2
#define UTILIZADORES 3
#define ERRO 0
#define CERTO 1
#define MES 0
#define DIA 1



//Verifica se é inteiro
int is_integer(char* str)
{
    int i;
    for(i = 0; str[i] != '\0'; i++)
    {
        if(!isdigit(str[i]))
        {
            return ERRO;
        }
    }
    return CERTO;
}

/////////////////////////////PASSAGEIROS/////////////////////////////

int dividir_linha_passageiro (char* linha,char dados_passageiro [2][40]) //serve para dividir a linha nos 2 campos de cada passsageiro
{
    char* save_tok;
    char* token;
    token = strtok_r (linha,";",&save_tok);
    if (token == NULL) 
    {
    return ERRO;
    }
    strcpy (dados_passageiro[0],token);
    token = strtok_r (NULL,";",&save_tok);
    if (token == NULL) 
    {
    return ERRO;
    }
    strcpy (dados_passageiro[1],token);
    return CERTO;
}

int testar_linha_passageiros(char* linha,GHashTable* erros_utilizadores,int* erros_voos,GHashTable* dados_utilizadores_id,int* passageiros_voo)
{
    int res;
    char temp[60];
    strcpy (temp,linha);
    char dados_passageiro [2][40];

    res = dividir_linha_passageiro (temp,dados_passageiro); 
    if (res == ERRO)         //Garante que não há espaços em branco
    {               
        return ERRO;
    }

    int v = atoi (dados_passageiro[0]) - 1;

    if (erros_voos [v] == 0)    //procura do voo para garantir que nao é passageiro de um voo invalido
    {
     return ERRO;
    }
    

    if (dados_passageiro[1] != NULL) 
    {
    dados_passageiro[1][strlen(dados_passageiro[1]) - 1] = '\0';
    if (g_hash_table_contains(erros_utilizadores, dados_passageiro[1])) 
    {
        return ERRO;
    } 
    }

   
    (passageiros_voo[v]) += 1;
    aumentar_numero_voos (dados_passageiro[1],dados_utilizadores_id);


    return CERTO;
}


/////////////////////////////UTILIZADORES/////////////////////////////

int dividirLinha_utilizador(char *linha, char dados_linha[12][150])  //serve para dividir a linha nos 12 campos de cada utilizador
{                                                                   //e garantir que nenhum esta vazio
    int i = 0;
    char *token;
    char *save_tok;
    int contador = 0;

    token = strtok_r(linha, ";", &save_tok);
    while (token != NULL) {
        strcpy(dados_linha[i], token);
        token = strtok_r(NULL, ";", &save_tok);
        i++;
        contador++;
    }
    if(contador != 12) //erro se exitir um campo vazio pois todos sao obrigatorios
    {
        return ERRO;
    }
    return CERTO;
}


int testar_linha_utilizador(char* linha) 
{
    char temp [1000];
    strcpy(temp,linha);
    char dados_linha_u[12][150];
    int res = dividirLinha_utilizador(temp,dados_linha_u);
   
    //Verifica que todos os dados existem
    if(res == ERRO)
    {
    
        return ERRO;  
    }

     //Verificar country_code 
    if(strlen(dados_linha_u[7]) != 2)
    {
    
        return ERRO;
    }
    else
    {
        for(int i = 0; i < 2;i++)
        {
            if(!isalpha(dados_linha_u[7][i]))
            {
            
                return ERRO;
            }
        }    
    }

    //Verifica birth_date
    int data_nascimento[3];
    if(check_data(dados_linha_u[4],data_nascimento) == ERRO)
    {
    
        return ERRO;
    }


    //Verifica account_creation
    int data_creation[6];
    if(check_data_tempo(dados_linha_u[9],data_creation) == ERRO)
    {
    
        return ERRO;
    }

    //nascimento < criação
    if(compara_data_utilizador(data_nascimento,data_creation) == ERRO) //não acrescentou
    {
    
        return ERRO;
    } 


    //Verificar email    
    if (verifica_email(dados_linha_u[2]) == ERRO)
    {
    
        return ERRO;
    }
    

    //Verificar account_status
    for(int i = 0; dados_linha_u[11][i] != '\0'; i++)
    {
        dados_linha_u[11][i] = tolower(dados_linha_u[11][i]);
    }

    if(strcmp(dados_linha_u[11],"active\n") == 0 || (strcmp(dados_linha_u[11],"inactive\n") == 0))
    {
    
        return CERTO;
    }
    else
    {                   //erro se uma conta nao for ativa nem inativa
    
        return ERRO;
    }

    return ERRO;
}



int verifica_email (char* str)
{
    char* token;                    //garantimos o formato requisitado 
    char* save_token;

    token = strtok_r(str,"@",&save_token);
    
    if(strlen(token) < 1)
    {
        return ERRO;
    }
    else
    {
        token = strtok_r(NULL,".",&save_token);
        if(token == NULL || strlen(token) < 1)
        {
            return ERRO;
        }
        else
        {
            token = strtok_r(NULL,".",&save_token);
            if(token == NULL || strlen(token) < 2)
            {
                return ERRO;
            }
        }
    }
 return CERTO;

}


/////////////////////////////RESERVAS/////////////////////////////

int dividirLinha(char *linha, char dados_linha[14][126]) //serve para dividir a linha nos 14 campos de cada reserva
{
    int contador = 0;
    char* token;
    token = strsep(&linha,";");

    while (contador < 8)
    {
         strcpy(dados_linha[contador],token);
         if (strcmp (dados_linha[contador],"") == 0) 
        {
            return ERRO;            //retorna erro na falta de um dos campos obrigatorios
        }
        token = strsep(&linha,";");
        contador++;
    };
    

    while (contador < 14)
    {
        strcpy(dados_linha[contador],token);
        contador++;
        token = strsep(&linha,";");
    };

    if (strcmp ((dados_linha[10]),"") == 0)      //preenchemos campos para representar o equivalente a um campo vazio
    {
        strcpy (dados_linha[10],"false");
    }
    if (strcmp (dados_linha[12],"") == 0)
    {
        strcpy (dados_linha[12],"vazio");
    }   

 
    return CERTO;
}


int includes_breakfast (char* str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }

    if(strcmp(str,"f") != 0 && strcmp(str,"false") != 0 && strcmp(str,"0") != 0 &&
       strcmp(str,"t") != 0 && strcmp(str,"true") != 0 && strcmp(str,"1") != 0)
    {
        return ERRO;  //erro caso nao corresponda a nenhuma das strings possiveis
    }

    return CERTO;
}

int verifica_rating(char* str)
{
    if(strlen(str) != 1)
    {
        return ERRO;
    }
    else
    {
        if(strcmp(str,"vazio") == 0)
        {
            return CERTO;
        }
        else if(is_integer(str) == ERRO)
        {
            return ERRO;                        //garante que o rating é "vazio" ou uma string entre 1 e 5 inclusive
        }
        else
        {
            int rating = atoi(str);
            if(rating < 1 || rating > 5)
            {
                return ERRO;
            }
        }
    }
    return CERTO;
}


int testar_linha_reserva(char* linha,GHashTable* erros_utilizadores,void* registos_reservas,GHashTable* verificar_reserva,int interativo)
{
    int res;
    char temp[1000]; 
    strcpy (temp,linha);
    char dados_linha[14][126];
    res = dividirLinha(temp,dados_linha);

    // Verifica id, user_id, hotel_id, hotel_name, adress (campos obrigatorios)
    if (res == ERRO )
    {
        return ERRO;
    }

    char* hashing = strdup (dados_linha[1]);
     if(g_hash_table_lookup (erros_utilizadores,hashing) != NULL) //garante que a reserva nao pertence a um utilizador invalido
    {
        free (hashing);
        return ERRO;
    }
    free (hashing);

    //Verificar sintaxe das Datas 
    int data_inicial[3];
    if(check_data(dados_linha[7],data_inicial) == ERRO)
    {
    
        return ERRO;
    }

       //Verifica se as estrelas estão corretas
    if(strlen(dados_linha[4]) != 1)
    {
    
        return ERRO;
    }
    else
    {
        int estrelas = atoi(dados_linha[4]);
        if(estrelas < 1 || estrelas > 5)
        {
        
            return ERRO;
        }
    }

     //Verifica preço por noite e imposto da cidade
    if(is_integer(dados_linha[5]) == ERRO || is_integer(dados_linha[9]) == ERRO)
    {
    
        return ERRO;
    }
    else
    {
        if(atoi(dados_linha[5]) <0 || atoi(dados_linha[9]) < 1)
        {
        
            return ERRO;
        }
    }

      //Verifica o inclui pequeno almoço
    if(includes_breakfast(dados_linha[10]) == ERRO)
    {
    
        return ERRO;
    }
    
    //Verifica o Rating
    if(verifica_rating(dados_linha[12]) == ERRO)
    {
    
        return ERRO;
    }


    //Garantir que a data inicial é menor que a final
    int data_final[3];
    if(check_data(dados_linha[8],data_final) == ERRO)
    {
    
        return ERRO;
    }
    else
    {
        if(data_inicial[0] > data_final[0] || (data_inicial[0] == data_final[0] && (data_inicial[1] > data_final[1])) 
           || (data_inicial[0] == data_final[0] && data_inicial[1] == data_final[1] && (data_inicial[2] > data_final[2])))
        {
        
            return ERRO;
        }
    }
    
    if (interativo == 0)
    {
    if (!(g_hash_table_contains(verificar_reserva,dados_linha[0]) || g_hash_table_contains(verificar_reserva,dados_linha[1]) ||g_hash_table_contains(verificar_reserva,dados_linha[2])))
    {
    fazer_registo_reserva (data_inicial,registos_reservas);
    return 2; //excessao
    }
    }
    return CERTO;
}

///////////////////////////////////VOOS///////////////////////////////////

int dividirLinha_voo(char *linha, char dados_linha[13][126]) //serve para dividir a linha nos 13 campos de cada voo
{
    char buffer[1000];
    strcpy(buffer,linha);
    char* ptr = buffer;
    char* token;
    int i;
    for(i = 0; i < 12; i++)
    {
        token = strsep(&ptr,";");
        if(token[0] == '\0')
        {
            return ERRO;            //retorna erro caso falte algum dos 12 primeiros campos 
        } 
        strcpy(dados_linha[i],token);
    }
    token = strsep(&ptr,";");
    strcpy(dados_linha[i],token); 
    return CERTO;
} 

int testar_linha_voo(char* linha)
{
    char dados_linha[13][126];
    int res = dividirLinha_voo(linha,dados_linha);

    //Verifica id,airplane,plane-model,pilot,copilot (campos obrigatorios)
    if(res == ERRO)
    {
        return ERRO;
    }

     //Verificar que o numero de lugares é um numero 
     if (is_integer (dados_linha[3]) == ERRO)
     {
        return ERRO;
     }

     //Verificar que a origem e destino tem tamanho 3 
      if(strlen(dados_linha[4]) != 3 || strlen(dados_linha[5]) != 3)     
    {
        return ERRO;
    }

    //Verificar sintaxe das datas
    int schedule_departure_data[6];
    if(check_data_tempo(dados_linha[6],schedule_departure_data) == ERRO)
    {
        return ERRO;
    }

    int schedule_arrival_data[6];
    if(check_data_tempo(dados_linha[7],schedule_arrival_data) == ERRO)
    {
        return ERRO;
    }
    
    if(compara_data_tempo(schedule_departure_data,schedule_arrival_data) == ERRO)
    {
        return ERRO;
    }

//Verificar datas reais
 int real_departure_data[6];
    if(check_data_tempo(dados_linha[8],real_departure_data) == ERRO)
    {
        return ERRO;
    }

    int real_arrival_data[6];
    if(check_data_tempo(dados_linha[9],real_arrival_data) == ERRO)
    {
        return ERRO;
    }
    
    if(compara_data_tempo(real_departure_data,real_arrival_data) == ERRO)
    {
        return ERRO;
    }


    return CERTO;
}

   

