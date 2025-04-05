import java.io.*;
import java.nio.charset.StandardCharsets;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class TestLoadGenerator {

    public TestLoadGenerator() {
    }

    private float calculateAverageTime(String logFileName,Long curr) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(logFileName))) {
            String line;
            long total = 0;
            int count = 0;

            while ((line = reader.readLine()) != null) {
                try {
                    String firstPart = line.split(" ")[1];
                    long duration = Long.parseLong(firstPart);
                    var temp = duration;
                    duration -= curr;
                    curr = temp;                    
                    total += duration / 1000;
                    count++;
                } catch (NumberFormatException e) {
                    System.out.println("Linha ignorada: " + line);
                }
            }

            if (count == 0) {
                throw new IOException("O arquivo está vazio.");
            }

            return (float) total / (count * 1000);
        }
    }

    private float calculateMaxTime(String logFileName,Long curr) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(logFileName))) {
            String line;
            long max = Long.MIN_VALUE;
            boolean hasValues = false;

            while ((line = reader.readLine()) != null) {
                try {
                    String firstPart = line.split(" ")[0];
                    long duration = Long.parseLong(firstPart);
                    var temp = duration;
                    duration -= curr;
                    curr = temp;                    
                    if (duration > max) {
                        max = duration;
                    }
                    hasValues = true;
                } catch (NumberFormatException e) {
                    System.out.println("Linha ignorada: " + line);
                }
            }

            if (!hasValues) {
                throw new IOException("O arquivo está vazio.");
            }

            return (float) max / 1000000;
        }
    }

    private float calculateMinTime(String logFileName,Long curr) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(logFileName))) {
            String line;
            long min = Long.MAX_VALUE;
            boolean hasValues = false;

            while ((line = reader.readLine()) != null) {
                try {
                    String firstPart = line.split(" ")[1];
                    long duration = Long.parseLong(firstPart);
                    var temp = duration;
                    duration -= curr;
                    duration /= 1000;
                    curr = temp;                    
                    if (duration < min) {
                        min = duration;
                    }
                    hasValues = true;
                } catch (NumberFormatException e) {
                    System.out.println("Linha ignorada: " + line);
                }
            }

            if (!hasValues) {
                throw new IOException("O arquivo está vazio.");
            }

            return (float) min / 1000;
        }
    }

    private float calculateExecutionTimeFromTwoFiles(String logFileName1, String logFileName2) throws IOException {
        long minEpochTime = Long.MAX_VALUE;
        long maxEpochTime = Long.MIN_VALUE;

        long[] results1 = processLogFile(logFileName1);
        minEpochTime = Math.min(minEpochTime, results1[0]);
        maxEpochTime = Math.max(maxEpochTime, results1[1]);

        long[] results2 = processLogFile(logFileName2);
        minEpochTime = Math.min(minEpochTime, results2[0]);
        maxEpochTime = Math.max(maxEpochTime, results2[1]);

        if (minEpochTime == Long.MAX_VALUE || maxEpochTime == Long.MIN_VALUE) {
            throw new IOException("Os arquivos não contêm dados válidos.");
        }

        return (float) (maxEpochTime - minEpochTime) / 1000000000;
    }

    private long[] processLogFile(String logFileName) throws IOException {
        long minEpochTime = Long.MAX_VALUE;
        long maxEpochTime = Long.MIN_VALUE;

        try (BufferedReader reader = new BufferedReader(new FileReader(logFileName))) {
            String line;
            while ((line = reader.readLine()) != null) {
                try {
                    String[] parts = line.split(" ");
                    if (parts.length < 2) {
                        continue;
                    }

                    long epochTime = Long.parseLong(parts[1].trim());
                    minEpochTime = Math.min(minEpochTime, epochTime);
                    maxEpochTime = Math.max(maxEpochTime, epochTime);

                } catch (NumberFormatException e) {
                    System.out.println("Linha ignorada: " + line);
                }
            }
        }

        return new long[]{minEpochTime, maxEpochTime};
    }




    public static void main(String[] args) throws Exception {
        TestLoadGenerator test = new TestLoadGenerator();
        var curr = System.nanoTime();
        int numClients = 10;
        int numOperations = 10000;
        List<Thread> threads = new ArrayList<>();

        
        try (FileWriter fw = new FileWriter("get_execution_times.log",false)) {
            // Abrir no modo de gravação limpa o conteúdo automaticamente.
        } catch (IOException e) {
            e.printStackTrace();
        }
        try (FileWriter fw = new FileWriter("put_execution_times.log",false)) {
            // Abrir no modo de gravação limpa o conteúdo automaticamente.
        } catch (IOException e) {
            e.printStackTrace();
        }

        for (int i = 0; i < numClients; i++) {
            final String id = Integer.toString(i);
            Thread t;
            if (i % 2 == 0) {
                 t = new Thread(() -> {
                    try {
                        ClientInterface client = new ClientInterface();
                        client.lib.connect();
                        client.lib.register(id, id);
                        client.lib.login(id, id);

                        for (int j = 0; j < numOperations; j++) {
                            Random random = new Random();

                            Thread.sleep(1);
                            client.lib.sendGet(Integer.toString(random.nextInt(1000)));
                        }

                        client.lib.disconnect();

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                });
             } else { 
                t = new Thread(() -> {
                    try {
                        ClientInterface client = new ClientInterface();
                        client.lib.connect();
                        client.lib.register(id, id);
                        client.lib.login(id, id);

                        for (int j = 0; j < numOperations; j++) {
                            // Exemplo de operação PUT
                            Random random = new Random();

                            Thread.sleep(1);
                            client.lib.sendPut(Integer.toString(random.nextInt(1000)), Integer.toString(random.nextInt(1000)).getBytes(StandardCharsets.UTF_8));
                        }

                        client.lib.disconnect();

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }); 
            }

            threads.add(t);
        }

        for (Thread t: threads)
            t.start();

        for (Thread t: threads)
            t.join();


        // Como o servidor não consegue processar instantaneamente as frames recebidas,
        // esta leitura serve para garantir que todos os dados enviados pelo cliente são recebidos antes deste socket terminar
        BufferedReader input_client = new BufferedReader(new InputStreamReader(System.in));

        while (true) {
            System.out.println("Escreva exit quando desejar parar o teste: ");
            String input = input_client.readLine();
            if (input.equals("exit"))
                break;
        }
        
        DecimalFormat df = new DecimalFormat("#.####");
        System.out.println("Média do tempo de execução do get: " + df.format(test.calculateAverageTime("get_execution_times.log",curr)) + "ms");
        System.out.println("Valores do get - Máx: " + df.format(test.calculateMaxTime("get_execution_times.log",curr)) + "ms Mín: " + df.format(test.calculateMinTime("get_execution_times.log",curr)) + "ms");
        System.out.println("Média do tempo de execução do put; " + df.format(test.calculateAverageTime("put_execution_times.log",curr)) + "ms");
        System.out.println("Valores do put - Máx: " + df.format(test.calculateMaxTime("put_execution_times.log",curr)) + "ms Mín: " + df.format(test.calculateMinTime("put_execution_times.log",curr)) + "ms");
        System.out.println("Tempo total de execução das tarefas: " + df.format(test.calculateExecutionTimeFromTwoFiles("put_execution_times.log", "get_execution_times.log")) + " s");
    }
}
