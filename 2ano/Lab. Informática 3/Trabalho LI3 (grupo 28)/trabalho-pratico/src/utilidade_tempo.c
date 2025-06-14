#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ERRO 0
#define CERTO 1


int sum (int* array,int N)
{
    int res = 0;
    while (N >= 0)
    {
        res += array[N];
        N--;
    }
    return res;
}

int check_data (char*dados_linha,int data[3])
{
    char *token;
    char *save_tok;
    token = strtok_r(dados_linha, "/", &save_tok);
    int token_int = atoi(token);

    //Verificar sintaxe das datas
    if(strlen(token) != 4)
    {
        return ERRO;
    }
    else
    {
        data[0] = token_int;
        token = strtok_r(NULL, "/", &save_tok);
        token_int = atoi(token);
        if(strlen(token) != 2)
        {
            return ERRO;
        }
        else
        {
            data[1] = token_int;
            token = strtok_r(NULL, "/", &save_tok);
            token_int = atoi(token);
            if(strlen(token) != 2)
            {
                return ERRO;
            }
            else
            {
                data[2] = token_int;
            }
        }
    }
//Verificar se não temos uma data errada (meses ou dias impossiveis)
    if(data[1] < 1 || data[1] > 12 || data[2] < 1 || data[2] > 31)   
    {
        return ERRO;
    } 
    
    return CERTO;
 } 

int compara_data_tempo(int data_inicial[6], int data_final[6]) {
    for (int i = 0; i < 6; i++)                                 //retorna erro caso a data inicial seja > data final 
    {
        if (data_inicial[i] > data_final[i]) 
        {
            return ERRO;
        } else if (data_inicial[i] < data_final[i]) {
            break;
        }
    }
    return CERTO; 
}

int check_data_tempo(char*dados_linha,int data[6])
{
    char *token;
    char *save_tok;
    token = strtok_r(dados_linha, "/", &save_tok);
    int token_int = atoi(token);

     //Verificar sintaxe das datas (datas com horas minutos e segundos)
    if(strlen(token) != 4)
    {
        return ERRO;
    }
    else
    {
        data[0] = token_int;
        token = strtok_r(NULL, "/", &save_tok);
        token_int = atoi(token);
        if(strlen(token) != 2)
        {
            return ERRO;
        }
        else
        {
            data[1] = token_int;
            token = strtok_r(NULL, " ", &save_tok);
            token_int = atoi(token);
            if(strlen(token) != 2)
            {
                return ERRO;
            }
            else
            {
                data[2] = token_int;
                token = strtok_r(NULL, ":", &save_tok);
                token_int = atoi(token);
                if(strlen(token) != 2)
                {
                    return ERRO;
                }
                else
                {
                    data[3] = token_int;
                    token = strtok_r(NULL, ":", &save_tok);
                    token_int = atoi(token);
                    if(strlen(token) != 2)
                    {
                        return ERRO;
                    }   
                    else
                    {
                        data[4] = token_int;
                        token = strtok_r(NULL, ":", &save_tok);
                        token_int = atoi(token);
                        if(strlen(token) != 2)
                        {
                            return ERRO;
                        }   
                        else
                        {
                            data[5] = token_int;
                        }
                    }
                }
            }
            
        }
    }
    //garante que nao existem valores invalidos
    if(data[1] < 1 || data[1] > 12 || data[2] < 1 || data[2] > 31 || data[3] < 0 || data[3] > 23 || data[4] < 0 || data[4] > 59 || data[5] < 0 || data[5] > 59)   
    {
        return ERRO;
    } 

    return CERTO;
    
}

int compara_data_utilizador(int data_nascimento[3], int data_creation[6]) {
    for (int i = 0; i < 3; i++) 
    {
        if (data_nascimento[i] > data_creation[i])              //garante que a data de nascimento é menor que a de criaçao da conta
        {
            return ERRO;
        } else if (data_nascimento[i] < data_creation[i]) 
        {
             return CERTO;
        }
    }
    return CERTO; 
}

void converter_data (char letras[],int data[]) //converte as datas para vetores de inteiros
    {
    char* token;
    char* save_tok;
    int i = 0;
    token = strtok_r(letras,"/",&save_tok);
    while (token != NULL && i < 3)
    {
        data[i] = atoi (token);
        i++;
        token = strtok_r(NULL,"/",&save_tok);
    }
    }

int c_idade (char* nascimento) 
{
    char* copia = strdup (nascimento);
    int data[3];
    int idade = 0;
    char *token;
    char *save_tok;

    token = strtok_r(copia, "/", &save_tok);
    int token_int = atoi(token);
    data[0] = token_int;

    token = strtok_r(NULL, "/", &save_tok);
    token_int = atoi(token);
    data[1] = token_int;

    token = strtok_r(NULL, "/", &save_tok);
    token_int = atoi(token);
    data[2] = token_int;

    idade = 2023 - data[0];          //2023 / 10 / 1
    if (data [1] > 10 || (data[0] > 1 && data[1] == 10))           
    {                                  
        idade -= 1;
    }

    free (copia);
    return idade;        
}

int comparar_datas (int data1[],int data2[])  
   {
     if (data1[0] > data2[0])       //compara anos
     {
        return 1;
     } 
     else  if (data1[0] < data2[0])
           {
            return 0;
           }
         else if (data1[1] > data2[1])      //compara meses
            {
                return 1;
            }
            else if (data1[1] < data2[1])
                {
                    return 0;
                }
              else if (data1[2] > data2[2])     //compara dias
                    {
                        return 1;
                    }
                    else if (data1[2] < data2[2])
                        {
                            return 0;
                        }
            return 3;  //retorna 3 se forem datas iguais
   }

int diferenca_dias_atrasos (int data_inicial[3],int data_final[3])
{                                                                       //calcula a diferenca exata de dias entre 2 datas
    int meses[12] = {0,31,59,90,120,151,181,212,243,273,304,334}; //valores de dias passados a cada mes para simplificar calculos
    int valor_anos = 0; 
    int valor_meses = 0;                    
    int valor_dias = 0;
    int resultado = 0;
    valor_anos = (data_final[0] - data_inicial[0]) * 365;
    valor_meses = meses[data_final[1]-1] - meses [data_inicial[1]-1];
    valor_dias = data_final[2] - data_inicial[2];
    resultado = (valor_anos + valor_meses + valor_dias);
                                                //VER PRINT DAS NOITES NA QUERIE 1
    return (resultado);
}

int diferenca_dias (int data_inicial[3],int data_final[3])
{                                                                       //calcula a diferenca exata de dias entre 2 datas
    int meses[12] = {0,31,59,90,120,151,181,212,243,273,304,334}; //valores de dias passados a cada mes para simplificar calculos
    int valor_meses = 0;                    
    int valor_dias = 0;
    int resultado = 0;
    valor_meses = meses[data_final[1]-1] - meses [data_inicial[1]-1];
    valor_dias = data_final[2] - data_inicial[2];
    resultado = (valor_meses + valor_dias);
                                                //VER PRINT DAS NOITES NA QUERIE 1
    return (resultado);
}