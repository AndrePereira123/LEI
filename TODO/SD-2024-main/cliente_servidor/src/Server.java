import serializables.*;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.locks.ReentrantLock;

public class Server extends Thread{
    private Manager manager;
    private MyThread threading;
    private final int port = 9092;

    public Server(int N){
        this.manager = new Manager(N);
        this.threading = new MyThread();
    }

    public void start_server() throws IOException{
        try(ServerSocket server_socket = new ServerSocket(port))
        {
            Socket s = null;
            while(true)
            {
                Socket client_socket = server_socket.accept();
                MessageQueue queue = new MessageQueue();
                TaggedConnection c = new TaggedConnection(client_socket);
                new Thread(new Client_handler(c, this.manager,this.threading, queue)).start();
                new Thread(new Client_Send_Queue(c, queue)).start();
            }
        }catch (IOException exception)
        {
            throw new IOException(exception);
        }
    }

    private class Client_Send_Queue implements Runnable {
        private TaggedConnection c;
        private MessageQueue queue;

        public Client_Send_Queue(TaggedConnection c, MessageQueue queue) {
            this.c = c;
            this.queue = queue;
        }

        @Override
        public void run() {
            try {
                while(queue.running) {
                    Frame frame = queue.dequeue();
                    if (frame != null) {
                        c.send(frame.tag, frame.data);
                    }
                }
            } catch (InterruptedException | IOException e) {
                throw new RuntimeException("Erro de escrita no cliente" + e);
            }
        }
    }

    private class Client_handler implements Runnable {
        private TaggedConnection c;
        private Manager manager;
        private MyThread threading;
        private MessageQueue queue;
        //private static int total_tasks = 0;
        //private static ReentrantLock lock_geral = new ReentrantLock();



        public Client_handler(TaggedConnection c, Manager manager,MyThread threading, MessageQueue queue) {
            this.c = c;
            this.manager = manager;
            this.threading = threading;
            this.queue = queue;
        }


        @Override
        public void run() {
            try {
                boolean running = true;
                while(running) {

                    Frame frame = c.receive();
                    if (frame == null) {
                        return;
                    }

                    switch (frame.tag) {
                        // terminar cliente
                        case 0:
                            running = false;
                            break;

                        // registar utilizador
                        case 1:
                            Account toRegister = (Account) frame.data;
                            var reg = manager.register(toRegister.user, toRegister.password);
                            if (reg) {
                                queue.enqueue(new Frame(0, new Response("Registo efetuado com sucesso!", true)));
                            } else {
                                queue.enqueue(new Frame(0, new Response("Já existe um utilizador com o mesmo nome!", false)));
                            }
                            break;

                        // iniciar sessão do utilizador
                        case 2:
                            Account toLogin = (Account) frame.data;
                            var logged = manager.login(toLogin.user, toLogin.password);
                            if (logged) {
                                queue.enqueue(new Frame(0, new Response("Login efetuado com sucesso!", true)));
                                server_after_login(c, threading);
                            } else {
                                queue.enqueue(new Frame(0, new Response("Credenciais erradas!", false)));
                            }
                            break;

                        default:
                            queue.enqueue(new Frame(0, new Response("Input inválido!", false)));
                            break;

                    }

                }

                c.close();

            } catch (IOException exception) {

            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        private void server_after_login(TaggedConnection c,MyThread threading) throws IOException, InterruptedException {
           // int val = 0; // serve para o modo de testes
            while (true) {
                Frame frame = c.receive();
                if (frame == null)
                {
                    System.out.println("Cliente terminou sessão");
                    return;
                }
                
                //System.out.println(val);
                switch (frame.tag) {
                    // entrada
                    case 3:
                        Entrada entrada = (Entrada) frame.data;
                        Task task = new Task(entrada.chave,entrada.valor,manager);
                        threading.threadPool.enqueueTask(task);
                        break;

                    // chave
                    case 4:
                        Chave chave = (Chave) frame.data;
                        Task task_read = new Task(chave.chave,this.manager,queue);
                        threading.threadPool.enqueueTask(task_read);
                        break;

                    // mapa
                    case 5:
                        MapEntrada mapa = (MapEntrada) frame.data;
                        Task put_multi = new Task(mapa.mapa,manager);
                        threading.threadPool.enqueueTask(put_multi);
                        break;

                    // set
                    case 6:
                        SetChave set = (SetChave) frame.data;
                        Task get_multi = new Task(set.chaves,manager,queue);
                        threading.threadPool.enqueueTask(get_multi);
                        break;

                    // getWhen
                    case 7:
                        GetWhen getWhen = (GetWhen) frame.data;
                        Task get_when = new Task(getWhen.chave_real, getWhen.chave_condicional, getWhen.valor_condicional, queue, manager);
                        threading.threadPool.enqueueTask(get_when);
                        break;

                    // disconnect
                    case 0:
                        Response username_to_disconnect = (Response) frame.data;
                        manager.disconnect(username_to_disconnect.message);
                        queue.stop();
                        return;
                }
            }
        }
    }




        public static void main(String[] args) throws IOException {
            Server server = new Server(100); // N é o número de utilizadores que podem utilizar o programa concorrentemente, utilizar este valor para os testes -> valor recomendável: >=10
            server.start_server();
        }

}