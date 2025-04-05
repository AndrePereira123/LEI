import serializables.*;

import java.io.IOException;
import java.net.Socket;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.locks.ReentrantLock;

public class ClienteLib {
    public String username;
    public ReentrantLock printLock = new ReentrantLock();
    private String host;
    private int port;
    private TaggedConnection c;
    private Thread receiver;

    public ClienteLib() {
        this.host = "localhost";
        this.port = 9092;
    }

    public ClienteLib(String host,Integer port){
        this.host = host;
        this.port = port;
    }

    public void connect() throws IOException {
        Socket socket = new Socket(this.host, this.port);
        this.c = new TaggedConnection(socket);
    }

    public void register(String username, String password) throws IOException {
        this.c.send(1, new Account(username, password));
    }

    public Response receiveRegister() {
        return (Response) this.c.receive().data;
    }

    public void login(String username, String password) throws IOException {
        this.c.send(2, new Account(username, password));
    }

    public Response receiveLogin(String username) {
        Response resp = (Response) this.c.receive().data;
        if (resp.status) {
            this.username = username;
        }
        return resp;
    }

    public void disconnect() throws IOException {
        this.c.close();
    }

    public void disconnectWhenConnected() throws IOException {
        this.c.send(0, new Response(username, true));
    }

    public void receiveMessages() {
        receiver = new Thread(() -> {
            try {
                receiveMessage();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        receiver.start();
    }

    public void InterruptReceiveThreads() throws InterruptedException, IOException {
        c.close();
        receiver.interrupt();
        receiver.join();
    }

    private void receiveMessage() throws IOException {
        while (true) {
            Frame frame = c.receive();
            if (frame == null) return;
            if (frame.tag == 3) {
                Entrada entrada = (Entrada) frame.data;
                if (entrada != null) {
                    printLock.lock();
                    try {
                        System.out.println(entrada.toString());
                    } finally {
                        printLock.unlock();
                    }
                } else {
                    return;
                }
            } else if (frame.tag == 5) {
                MapEntrada mapEntrada = (MapEntrada) frame.data;
                if (mapEntrada != null) {
                    printLock.lock();
                    try {
                        System.out.println(mapEntrada.toString());
                    } finally {
                        printLock.unlock();
                    }
                } else {
                    return;
                }
            } else if (frame.tag == 8) {
                Entrada entrada = (Entrada) frame.data;
                if (entrada != null) {
                    printLock.lock();
                    try {
                        System.out.println("\nValor da chave condicional");
                        System.out.println(entrada.toString());
                    } finally {
                        printLock.unlock();
                    }
                } else {
                    return;
                }
            }
        }
    }

    public void sendPut(String key, byte[] bytes) throws IOException {
        c.send(3, new Entrada(key, bytes));
    }

    public void sendGet(String key) throws IOException {
        c.send(4, new Chave(key));
    }

    public void sendMultiPut(Map<String, byte[]> mapa) throws IOException {
        c.send(5, new MapEntrada(mapa));
    }

    public void sendMultiGet(Set<String> set) throws IOException {
        c.send(6, new SetChave(set));
    }

    public void sendGetWhen(String chaveReal, String chaveCondicional, byte[] valorCondicional) throws IOException {
        c.send(7, new GetWhen(chaveReal, chaveCondicional, valorCondicional));
    }


}


