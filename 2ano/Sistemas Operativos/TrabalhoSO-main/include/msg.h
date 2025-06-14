#ifndef DEFS_H
#define DEFS_H

#include <sys/time.h>

#define ORCHESTRATOR "tmp/fifo_orchestrator"
#define STATUS "tmp/save_status"
#define CONCURRENT "tmp/processos_ativos"
#define INPUT_SIZE 500  

typedef struct {
    int identificador;
    int pipeline;
    int pid;
    double tempo;
    char comando[301];
    struct timeval inicio;
} Msg;

#endif