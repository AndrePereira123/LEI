#include <auxiliares.h>
#include <queue.h>

// para comandos unicos
char** separa_argumentos (char* comando)
{
    char** argumentos = malloc (10 * sizeof(char**)); //GARANTIR Q NAO PASSA DE 100 ARGUMENTOS
    char* save = strdup(comando);

    char* token;
    char* save_tok;
    int contador = 0;

    token = strtok_r(save, " ", &save_tok);
    argumentos[contador] = strdup (token);
    contador++;

    while ((token = strtok_r(NULL, " ", &save_tok)))
    {
        argumentos[contador] = strdup (token);
        contador++;
    }
    argumentos[contador] = NULL;

    free(save);
    return argumentos; 
}

// para pipelines
char** separa_comandos (char* comandos, int* numero_comandos)
{
    //execute time -p "prog-a [args] | prog-b [args] | prog-c [args]"
    char* save = strdup(comandos);
    //fazer separacao 
    char** argumentos = malloc (300 * sizeof(char*));
    char* token;
    char* save_tok;
    int contador = 0;
    
    token = strtok_r(save, "|", &save_tok);
    argumentos[contador] = strdup (token);
    contador++;

    while ((token = strtok_r(NULL, "|", &save_tok)))
    {
        argumentos[contador] = strdup (token);
        contador++;
    }

    argumentos[contador] = NULL;
    *numero_comandos = contador;       
    free(save);
    return argumentos;
}

Fila* initQueue()
{
    Fila *fila = (Fila*)malloc(sizeof(Fila));
    if(fila != NULL)
    {
        fila->inicio = NULL;
        fila->fim = NULL;
        fila->tamanho = 0;
    }

    return fila;
}

int isEmpty(Fila *fila)
{
    if(fila->inicio == NULL ) return 1;

    return 0;
}

// adiciona ao fim da queue FIFO
void addQueue(Fila *fila, Msg msg, int sjf) 
{
    Nodo *newNodo = (Nodo *)malloc(sizeof(Nodo));
    if (newNodo == NULL) 
    {
        perror("Error allocating memory to new node");
        exit(1);
    }

    newNodo->mensagem = msg;
    newNodo->prox = NULL;

    if (fila->inicio == NULL ) 
    {
        // se a queue tiver vazia, adiciona na cabeça
        fila->inicio = newNodo;
        fila->fim = newNodo;
    } 
    else 
    {
        if (!sjf)
        {
            // caso contrario, adiciona no final
            fila->fim->prox = newNodo;
            fila->fim = newNodo;
        }
        else 
        {
            int fim = 0;
            Nodo* anterior = NULL;

            Nodo* current_node = fila->inicio;
            while (current_node != fila->fim  && fim == 0)
            {
                if (msg.tempo < current_node->mensagem.tempo) 
                {
                    // se sjf estiver acionado e o tempo da nova msg for menor, adiciona antes
                    newNodo->prox = current_node;
                    if (anterior != NULL) anterior->prox = newNodo;
                    else fila->inicio = newNodo;
                    fim = 1;
                }
                else 
                {
                    anterior = current_node;
                    current_node = current_node->prox;
                }
            }

            if (current_node == fila->fim) 
            {
                current_node->prox = newNodo;
                fila->fim = newNodo;
            }
        }
    }
    fila->tamanho++;
}


// dequeue do primeiro elemento da queue
Msg deQueue(Fila *fila)
{
    if(fila->inicio == NULL)
    {
        perror("Error the queue is empty");
        exit(1);
    }

    Nodo *temp = fila->inicio;
    fila->inicio = fila->inicio->prox;
    Msg mensagem = temp->mensagem;
    if(fila->inicio == NULL) fila->fim = NULL;
    fila->tamanho--;

    free(temp);

    return mensagem;
}


// logs
void logTask(char* pasta, Msg mensagem, double tempo) 
{
    char LOGS [100];
	sprintf (LOGS,"%s/logs.txt", pasta);
    int log = open(LOGS, O_WRONLY | O_CREAT | O_APPEND, 0600);
    if (log == -1) 
    {
        perror("Error opening log file");
        exit(1);
    }

    char text[400];
    sprintf(text, "%d  %s  %.4f ms\n", mensagem.identificador, mensagem.comando, tempo);

    ssize_t bytes_written = write(log, text, strlen(text));
    if (bytes_written == -1) 
    {
        perror("Error writing to log file");
        exit(1);
    }

    close(log);
}



//status
void status(Fila* fila, char* logs,int n_processos_paralelos)
{
    

	int ficheiro = open (STATUS, O_WRONLY | O_CREAT, 0600);
    if (ficheiro == -1)
    {
        printf ("Error opening status file\n");
        return;
    }


    Nodo* save = fila->inicio;

    // tarefas em execução
   
    write (ficheiro, "\nExecuting\n",11);
    char linha[2048];
    int bytes_read;

    for (int i = 1; i <= n_processos_paralelos;i++)
    {
        char processo [50];
        sprintf (processo,"tmp/processo_paralelo_%d",i);
        int fd = open(processo, O_RDONLY, 0600);
        if (fd == -1) 
        {
            perror("Error opening parallel process file"); 
            return;
        }

        if ((bytes_read = read(fd, linha, sizeof(linha))) > 0) 
        {
        write (ficheiro,linha,bytes_read);
        }

        close (fd);
    }


    
    
    // tarefas em fila de espera
    
    write (ficheiro, "\nScheduled\n",12);
    while (save != NULL)
    {
        Msg m = save->mensagem;

        char text[400];
        sprintf (text, "%d %s\n",m.identificador,m.comando);
        write (ficheiro,text,strlen(text));
        save = save->prox;
    }
    
    // tarefas terminadas
    write (ficheiro,"\nCompleted\n",12);

    char linha2[2048];
    int bytes_read2;
    char LOGS [100];
	sprintf (LOGS,"%s/logs.txt", logs);

    int fd = open(LOGS, O_RDONLY | O_CREAT, 0600);
    if (fd == -1) {
        perror("Error opening log file"); 
        return;
    }

    while ((bytes_read2 = read(fd, linha2, sizeof(linha2))) > 0) 
    {
        write (ficheiro,linha2,bytes_read2);
    }

    if (bytes_read2 == -1) {
        perror("Error reading file");  
        close(fd);
        return;
    }
    close (ficheiro);
    close(fd);
}



int adiciona_processo (char* comando,int identificador,int n_processos_paralelos)
{
    char processo [50];
    int resultado = 1;
    int sair = 0;
    for (int i = 1; i <= n_processos_paralelos && sair == 0;i++)
    {
        sprintf (processo,"tmp/processo_paralelo_%d",i);

        int fd = open(processo,O_RDWR, 0600);
        if (fd == -1) 
        {
            perror("Error opening parallel process file"); 
            return -1;
        }

        char buffer [5];
        if (read(fd,buffer,5) == 0)
        {
            char text[400];
            sprintf(text, "%d  %s\n",identificador,comando);
            write (fd,text,strlen(text));
            resultado = i;
            sair = 1;
        }
        close (fd);
    }
    return resultado;
}

void remove_processo (int id_processo)
{
    char processo [50];
    
    sprintf (processo,"tmp/processo_paralelo_%d",id_processo);

    int fd = open (processo,O_TRUNC,0600);
    if (fd == -1) 
    {
        perror("Error opening parallel process file"); 
        return;
    }
    close (fd);
}

void iniciar_processos (int n_processos_paralelos)
{
    char processo [50];
    for (int i = 1; i <= n_processos_paralelos;i++)
    {
        sprintf (processo,"tmp/processo_paralelo_%d",i);

        int fd = open(processo, O_CREAT, 0600);
        if (fd == -1) 
        {
            perror("Error opening parallel process file"); 
            return;
        }
        close (fd);
    }
}
