import serializables.*;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class ClientInterface {
    private static boolean running = true;
    public ClienteLib lib;

    //Construtor por omissão
    public ClientInterface(){
        lib = new ClienteLib("localhost", 9092);

    }

    //Construtor parametrizado
    public ClientInterface(String host,Integer port){
        lib = new ClienteLib(host, port);
    }

    public void startClient() throws Exception {
        lib.connect();
        BufferedReader input_client = new BufferedReader(new InputStreamReader(System.in));

        while (running) {
            System.out.print("\n------------MENU------------\n");
            System.out.println("REGISTER - 1");
            System.out.println("SIGN-IN - 2");
            System.out.println("CLOSE - 0");


            int user_input;
            while(true) {
                try {
                    user_input = Integer.parseInt(input_client.readLine());
                    break;
                } catch (Exception e) {
                    System.out.println("Insira um input válido");
                }
            }


            switch(user_input) {
                case 1:
                    handleRegister(input_client);
                    break;
                case 2:
                    boolean connected = handleLogin(input_client);
                    if(connected) {
                        running = false;
                    }
                    break;
                case 0:
                    running = false;
                    lib.disconnect();
                    break;
                default:
                    System.out.println("Insira um input válido");
                    break;
            }
        }
    }

    public void showMenu(BufferedReader input_client) throws InterruptedException, IOException {
        lib.receiveMessages();
        while (running) {
            lib.printLock.lock();
            try {
                System.out.print("\n------------MENU------------\n");
                System.out.println("PUT - 1"); //entrada
                System.out.println("GET - 2"); //chave
                System.out.println("MULTI-PUT - 3"); //map de entrada
                System.out.println("MULTI-GET - 4"); //set de chaves
                System.out.println("GET-WHEN - 5");
                System.out.println("CLOSE - 0");
            } finally {
                lib.printLock.unlock();
            }

            int user_input;
            while(true) {
                try {
                    user_input = Integer.parseInt(input_client.readLine());
                    break;
                } catch (Exception e) {
                    System.out.println("Insira um input válido");
                }
            }

            switch(user_input) {
                case 1:
                    handlePut(input_client);
                    break;
                case 2:
                    handleGet(input_client);
                    break;
                case 3:
                    handleMultiPut(input_client);
                    break;
                case 4:
                    handleMultiGet(input_client);
                    break;
                case 5:
                    handleGetWhen(input_client);
                    break;
                case 0:
                    lib.disconnectWhenConnected();
                    running = false;
                    break; //check this
            }
        }
        lib.InterruptReceiveThreads();
    }

    private void handleRegister(BufferedReader input_client) throws IOException{
        System.out.print("Username: ");
        String user = input_client.readLine();
        System.out.print("Password: ");
        String password = input_client.readLine();

        lib.register(user, password);
        Response resp = lib.receiveRegister();
        System.out.println(resp.message);
    }

    private boolean handleLogin(BufferedReader input_client) throws IOException, InterruptedException {
        System.out.print("Username: ");
        String user = input_client.readLine();
        System.out.print("Password: ");
        String password = input_client.readLine();
        System.out.println("Conectando...");

        lib.login(user, password);
        Response resp = lib.receiveLogin(user);
        System.out.println(resp.message);

        if (resp.status) {
            showMenu(input_client);
            lib.disconnect();
            return true;
        }
        return false;
    }

    private void handlePut(BufferedReader input_client) throws IOException {
        String key, msg;
        lib.printLock.lock();
        try {
            System.out.print("Insira a chave: ");
            key = input_client.readLine();
            System.out.print("Insira o valor: ");
            msg = input_client.readLine();
        } finally {
            lib.printLock.unlock();
        }

        lib.sendPut(key, msg.getBytes(StandardCharsets.UTF_8));
    }

    private void handleGet(BufferedReader input_client) throws IOException {
        String key_read;
        lib.printLock.lock();
        try {
            System.out.println("Insira a chave:");
            key_read = input_client.readLine();
        } finally {
            lib.printLock.unlock();
        }

        lib.sendGet(key_read);
    }

    private void handleMultiPut(BufferedReader input_client) throws IOException {
        Map<String, byte[]> mapa = new HashMap<>();
        lib.printLock.lock();
        try {
            System.out.println("Quantos valores pretende enviar ?");

            var n_mensagens = Integer.parseInt(input_client.readLine());
            for (int i = 0; i < n_mensagens; i++)
            {
                System.out.print("Insira a chave " + (i + 1) + ": ");
                var key_multi = input_client.readLine();

                System.out.print("Insira o valor " + (i + 1) + ": ");
                var msg_multi = input_client.readLine();

                byte[] bytes_multi = msg_multi.getBytes(StandardCharsets.UTF_8);
                mapa.put(key_multi, bytes_multi);
            }
        } finally {
            lib.printLock.unlock();
        }

        lib.sendMultiPut(mapa);
    }

    private void handleMultiGet(BufferedReader input_client) throws IOException {
        Set<String> set = new HashSet<>();
        lib.printLock.lock();
        try {
            System.out.println("Quantos valores pretende consultar ?");

            var n_keys = Integer.parseInt(input_client.readLine());
            for (int i = 0; i < n_keys; i++)
            {
                System.out.print("Insira a chave " + (i + 1) + ": ");
                var get_keys = input_client.readLine();
                set.add(get_keys);
            }
        } finally {
            lib.printLock.unlock();
        }

        lib.sendMultiGet(set);
    }

    private void handleGetWhen(BufferedReader input_client) throws IOException {
        String chaveReal, chaveCondicional, valorCondicional;
        lib.printLock.lock();
        try {
            System.out.print("Insira a chave que pretende saber o valor: ");
            chaveReal = input_client.readLine();
            System.out.print("Insira a chave condicional: ");
            chaveCondicional = input_client.readLine();
            System.out.print("Insira o valor da chave condicional: ");
            valorCondicional = input_client.readLine();
        } finally {
            lib.printLock.unlock();
        }

        lib.sendGetWhen(chaveReal, chaveCondicional, valorCondicional.getBytes(StandardCharsets.UTF_8));
    }

    public static void main(String[] args) throws Exception{
        ClientInterface client = new ClientInterface();
        client.startClient();
    }
}
