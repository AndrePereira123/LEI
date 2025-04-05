
#include <orchestrator.h>


int main (int argc, char * argv[])
{
	unlink (ORCHESTRATOR);
	unlink (STATUS);

	if (argc != 4)
	{
		write(STDOUT_FILENO, "Invalid number of arguments.\n", 30);
		return 1;
	}

	int pasta_output = mkdir(argv[1], 0755);
	if (pasta_output != 0 && pasta_output != EEXIST) 
	{
		perror("Error creating output folder");
		return 1;
	}

	if (atoi (argv[2]) < 1) 
	{
		write(STDOUT_FILENO, "Invalid number of parallel tasks.\n", 35);
		return 1;
	}

	if ((strcmp (argv[3],"FCFS") != 0) && (strcmp (argv[3],"SJF") != 0)) 
	{
		write(STDOUT_FILENO, "Escalation policy does not exist (use FCFS or SJF).\n", 52); 
		//First Come First Served
		//Shortest job First 
		return 1;
	}

	//escolher o escalonamento
	int sjf = 0;
	if(strcmp (argv[3],"SJF") == 0) sjf = 1; 

	//registar o numero maximo de tarefas em paralelo 
	int maxTarefas = atoi(argv[2]);
	

	if (mkfifo (ORCHESTRATOR,0600) == -1) 
	{
		perror ("Error creating orchestrator fifo");
		return 1;
	}

	
	int servidor = open (ORCHESTRATOR,O_RDONLY,0600);
    if (servidor == -1)
	{
        perror ("Error opening orchestrator fifo");
        return 1;
    }


	iniciar_processos (maxTarefas);
	Fila* fila = initQueue();
	Msg m;

	int n_processos = 0; //registo do numero de processos ativos

	int numero_task = 1; //contador para decidir nome de task(tarefas)

	while (1)
	{
		int filhos_terminados;
		if (waitpid(-1, &filhos_terminados, WNOHANG) > 0) n_processos = n_processos - 1;
		//remover processos antigos do numero total de processos ativos

			
		if ((read (servidor, &m, sizeof(Msg))) != 0)
		{
				char CLIENT_X [100];
        		sprintf (CLIENT_X,"tmp/fifo_cliente_%d",m.pid);
	
				int cliente = open (CLIENT_X,O_WRONLY,0600);
        		if (cliente == -1)
				{
            		perror ("Error opening client fifo in orchestrator");
            		return 1;
        		}


				if(m.identificador == -2) //identificador de comando status
				{
					status(fila, argv[1],maxTarefas);
					
					write (cliente,&m,sizeof(Msg));
				}
				else if (m.identificador == -3) // identificador de comando close
				{
					write (cliente,&m,sizeof(Msg));
					break;
				}
				else
				{
					if (gettimeofday(&m.inicio, NULL) == -1) 
					{
  					  	perror("Error in gettimeofday");
  					  	return 1;
  					}
 
					m.identificador = numero_task; //atualizar identificador da mensagem 
					numero_task++;
					addQueue(fila, m, sjf);

					write (cliente,&m,sizeof(Msg)); //resposta para cliente 
				}
				close (cliente);
		}


		if (!isEmpty (fila) && n_processos < maxTarefas)
		{
			
			Msg mensagem = deQueue(fila); //carregar proximo comando 
			n_processos++; //registar aumento do numero de processos 


			
			if(fork() == 0)
			{
				int processo = adiciona_processo (mensagem.comando,mensagem.identificador,maxTarefas);
				struct timeval fim;

	
	
				char OUTPUT_TASK [100];
				sprintf (OUTPUT_TASK,"%s/TASK_%d" ,argv[1],mensagem.identificador);
				int output = open (OUTPUT_TASK,O_RDWR | O_CREAT ,0600);
				if (output == -1)
				{
            		perror ("Error opening task output file");
            		return 1;
        		}

				if (mensagem.pipeline == 0) // execucao simples -u
				{
				   	char** argumentos = separa_argumentos (mensagem.comando);
	
				   	int pid = fork(); 
				   	if(pid == 0)   //filho executa comando
				   	{
				   		dup2(output, 1);
						dup2(output, STDERR_FILENO);
				   		if(execvp (argumentos[0], argumentos) == -1)
						{
							_exit(1);
						}
						_exit(0);
				   	}

				   	int status;
				   	wait(&status); //pai espera 

				   	int i = 0;
				   	while (argumentos[i] != NULL) //argumentos libertados da memoria
				   	{
				   		free (argumentos [i]);
				   		i++;
				   	}
				   	free(argumentos);
				}
				else 
				{
					int n_comandos;
					char** comandos = separa_comandos (mensagem.comando, &n_comandos);
					//registamos o numero de comandos e separamo-los 

				   	int pipes[n_comandos - 1][2]; 
					for(int i = 0; i < n_comandos; i++)
    				{
						char** comando_separado = separa_argumentos (comandos[i]); 

        				// não estamos no último processo
        				if(i < n_comandos - 1)
        				{
        				    pipe(pipes[i]); //criamos pipes suficientes
        				}

        				if(fork() == 0)
        				{
            				if(i == 0) // primeiro processo
            				{
            				    // Redireciona o std output e para o pipe e std error para o output
            				    dup2(pipes[i][1], STDOUT_FILENO);
								dup2(output, STDERR_FILENO);
            				    close(pipes[i][0]);
            				    close(pipes[i][1]);

								if(execvp (comando_separado[0],comando_separado) == -1)
								{
									_exit(1);
								}
            				    _exit(0);
            				}    
            				else if(i == n_comandos - 1) // último processo
            				{
            				    // Redireciona o std input para o pipe anterior
            				    dup2(pipes[i - 1][0], STDIN_FILENO);
            				    close(pipes[i - 1][0]);
            				    close(pipes[i - 1][1]);
								dup2(output, STDERR_FILENO);
								dup2 (output,STDOUT_FILENO);

            				    if(execvp (comando_separado[0],comando_separado) == -1)
								{
									_exit(1);
								}
            				    _exit(0);
            				}
            				else // processo intermediário
            				{
								dup2(output, STDERR_FILENO);
            				    // Redireciona o std output para o pipe
    							dup2(pipes[i][1], STDOUT_FILENO);
    							close(pipes[i][0]);
    							close(pipes[i][1]);
    							// Redireciona o std input para o pipe anterior
    							dup2(pipes[i - 1][0], STDIN_FILENO);
    							close(pipes[i - 1][0]);
    							close(pipes[i - 1][1]);

    							if(execvp (comando_separado[0],comando_separado) == -1)
								{
									_exit(1);
								}
            				    _exit(0);
            				}
        				}			

        				if(i == 0)
        				{			
        				    close(pipes[i][1]);
        				}
        				else if(i == n_comandos - 1)
        				{
        				    close(pipes[i - 1][0]);
        				}    
        				else
        				{    
        				    close(pipes[i - 1][0]);
        				    close(pipes[i][1]);
        				}
							int i = 0;
				   		while (comando_separado[i] != NULL) //comando_separado libertados da memoria
				   		{
				   			free (comando_separado [i]);
				   			i++;
				  	 	}
				   		free(comando_separado);

    				}

    				for(int i = 0; i < n_comandos; i++)
    				{
    				    int status;
    				    wait(&status); 
    				}

				   	int i = 0;
				   	while (comandos[i] != NULL)
				   	{
				   		free (comandos [i]);
				   		i++;
				   	}
				   	free(comandos);
				}		
				
				if (gettimeofday(&fim, NULL) == -1) 
				{
    				perror("Error in gettimeofday ");
    				return 1;
  				}
	
				long long microsegundos = (fim.tv_sec - (m.inicio).tv_sec) * 1000000 + fim.tv_usec - (m.inicio).tv_usec;
 				double tempo_milisegundos = microsegundos / 1000.0;


				logTask(argv[1], mensagem, tempo_milisegundos);
				remove_processo (processo);

				close (output);
				_exit(0);
			}
		}
	}

	write(STDOUT_FILENO, "Orchestrator finished.\n", 24);
	free(fila);
	close (servidor);
	return 0;
}
