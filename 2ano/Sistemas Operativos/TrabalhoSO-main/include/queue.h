#ifndef QUEUE_H
#define QUEUE_H

#include <msg.h>

typedef struct nodo {
    Msg mensagem;
    struct nodo *prox;
} Nodo;

typedef struct {
    Nodo *inicio;
    Nodo *fim;
    int tamanho;
} Fila;

#endif
