#include <client.h>




int main(int argc, char * argv[])
{
    if(argc == 5)
    {
        if (strcmp(argv[1],"execute") != 0)
        {
            write(STDOUT_FILENO, "Command does not exist.\n", 25);
            return 1;
        }
        if (atoi (argv[2]) <= 0)
        {
            write(STDOUT_FILENO, "Invalid task expected time.\n", 29);            
            return 1;
        }
        if (strcmp (argv[3],"-p") != 0 && strcmp (argv[3],"-u") != 0)
        {
            write(STDOUT_FILENO, "Error in the indicated flag, use -u for a program or -p for a pipeline.\n", 72);
            return 1;
        }
        if (strlen (argv[4]) > 300) 
        {
            write(STDOUT_FILENO, "Command line too long, maximum length is 300 characters.\n", 58);
            return 1;
        }
    }
    else if(argc == 2)
    {
        if (strcmp(argv[1],"status") != 0 && strcmp(argv[1],"close") != 0)
        {
            write(STDOUT_FILENO, "Command does not exist.\n", 25);
            return 1;
        }
    }
    else 
    {
		write(STDOUT_FILENO, "Invalid number of arguments.\n", 30);
		return 1;
	}


    int servidor = open(ORCHESTRATOR, O_WRONLY, 0600);
    if (servidor == -1) {
        perror("Error opening orchestrator fifo");
        close(servidor);
    }


    Msg m;
    if(argc == 5)
    {
        if (strcmp (argv[3],"-p") == 0) m.pipeline = 1; 
        else m.pipeline = 0;

        m.identificador = -1;       //para distinguir status e execute
        m.tempo = atoi (argv[2]);
        strcpy (m.comando,argv[4]);
    }
    else if(argc == 2)
    {
        if (strcmp(argv[1],"status") == 0 ) m.identificador = -2;
        if (strcmp(argv[1],"close") == 0 ) m.identificador = -3;
    }
    m.pid = getpid(); //pid para identificar fifo exclusivo 



    char CLIENT_X [100];
    sprintf (CLIENT_X,"tmp/fifo_cliente_%d",m.pid);

    if (mkfifo (CLIENT_X,0600) == -1) 
	{
        perror ("Error creating client fifo");
        return 1;
    }


    write(servidor, &m, sizeof(m)); //escrever no fifo do server



    int cliente = open(CLIENT_X, O_RDONLY, 0600);
    if (cliente == -1) 
    {
        perror("Error opening client fifo");  
        return 1;
    }


    Msg response;

    if (read(cliente,&response,sizeof (Msg)) != 0) //servidor escreve no fifo do cliente
	{
        if(response.identificador > 0)
        {
            char task_str[40];
            sprintf(task_str, "TASK %d Received\n", response.identificador);
            
            write(STDOUT_FILENO, task_str, strlen(task_str));
        }
        else if (response.identificador == -2)
        {
            
	        int ficheiro = open (STATUS,O_RDONLY,0600);
            if (ficheiro == -1)
            {
                perror ("Error opening status file");
                return 1;
            }

            char linha[2048];
            int bytes_read;
            while ((bytes_read = read (ficheiro,linha,sizeof (linha))) > 0)
            {
                write (STDOUT_FILENO,linha,bytes_read);
            }
            close (ficheiro);

        }
        else 
        {   
             write (STDOUT_FILENO, "Orchestrator Finished Successfully.\n", 37);
        }
    }
        

        close(cliente);
        close(servidor);

    return 0;
}