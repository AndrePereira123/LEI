
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include "queue.h"

char** separa_argumentos (char* comando );

char** separa_comandos (char* comandos,int* numero_comandos);

Fila* initQueue();

int isEmpty(Fila *fila);

void addQueue(Fila *fila, Msg msg, int sjf); 

Msg deQueue(Fila *fila);

void logTask(char* pasta, Msg mensagem, double tempo);

void status(Fila* fila, char* logs,int n_processos_paralelos);

void remove_processo (int identificador_ficheiro);

int adiciona_processo (char* comando,int identificador,int n_processos_paralelos);

void iniciar_processos (int n_processos_paralelos);