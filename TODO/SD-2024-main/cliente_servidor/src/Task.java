import serializables.Entrada;
import serializables.MapEntrada;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.time.Instant;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.locks.ReentrantLock;

public class Task implements Runnable {
        private final int PUT = 1;
        private final int GET = 2;
        private final int MULTI_PUT = 3;
        private final int MULTI_GET = 4;
        private final int GET_WHEN = 5;
        private String key;
        private String key_condicional;
        private int type;
        private byte[] data;
        private Manager manager;
        private MessageQueue queue;
        private Map<String, byte[]> pairs;
        private Set<String> keys;
        private static ReentrantLock lock_escrita_get = new ReentrantLock();
        private static ReentrantLock lock_escrita_put = new ReentrantLock();

        public Task(String key, String key_condicional, byte[] data, MessageQueue queue, Manager manager) {
            this.type = GET_WHEN;
            this.key = key;
            this.key_condicional = key_condicional;
            this.data = data;
            this.queue = queue;
            this.manager = manager;
        }

        public Task(String key,byte[] data,Manager manager) {
            this.type = PUT;
            this.key = key;
            this.data = data;
            this.manager = manager;
        }

        public Task(Map<String, byte[]> pairs,Manager manager)
        {
            this.type = MULTI_PUT;
            this.pairs = pairs;
            this.manager = manager;
        }

        public Task(String key, Manager manager, MessageQueue queue) {
            this.type = GET;
            this.key = key;
            this.data = null;
            this.manager = manager;
            this.queue = queue;
        }

        public Task(Set<String> keys,Manager manager, MessageQueue queue)
        {
            this.type = MULTI_GET;
            this.keys = keys;
            this.manager = manager;
            this.queue = queue;
        }

        @Override
        public void run() {
            long startTime = System.nanoTime();
            try {
                switch (this.type){
                    case PUT:
                        manager.put(this.key,this.data);
                        break;
                    case GET:
                        var bytes = manager.get(this.key);
                        if(bytes != null){
                            try {
                                queue.enqueue(new Frame(3, new Entrada(key, bytes)));
                            } catch (Exception e) {
                                throw new RuntimeException(e);
                            }
                        }else
                        {
                            try {
                                queue.enqueue(new Frame(3, new Entrada(key, "null".getBytes(StandardCharsets.UTF_8))));
                            } catch (Exception e) {
                                throw new RuntimeException(e);
                            }
                        }
                        break;
                    case MULTI_PUT:
                        manager.multiPut(pairs);
                        break;
                    case MULTI_GET:
                        var mapa_bytes = manager.multiGet(keys);
                        if (!mapa_bytes.isEmpty()) {
                            try {
                                queue.enqueue(new Frame(5, new MapEntrada(mapa_bytes)));
                            } catch (Exception e) {
                                throw new RuntimeException(e);
                            }
                        }
                        break;
                    case GET_WHEN:
                        try {
                            var getwhen_bytes = manager.GetWhen(key, key_condicional, data);
                            if (getwhen_bytes != null) {
                                queue.enqueue(new Frame(8, new Entrada(key, getwhen_bytes)));
                            } else {
                                queue.enqueue(new Frame(8, new Entrada(key, "null".getBytes(StandardCharsets.UTF_8))));
                            }
                        } catch (InterruptedException e) {
                            throw new RuntimeException(e);
                        }
                        break;
                }
            } catch (Exception e) {
                throw new RuntimeException(e);
            } finally {
                long endTime = System.nanoTime();
                long duration = endTime - startTime;
                if (type == PUT)
                    PutlogExecutionTime(duration);
                else if (type == GET)
                    GetlogExecutionTime(duration);
            }

        }

    private void PutlogExecutionTime(long duration) {
        String logFileName = "put_execution_times.log";
        lock_escrita_put.lock();
        try {
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(logFileName, true))) {
                writer.write(duration + " " + System.nanoTime() + "\n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        } finally {
            lock_escrita_put.unlock();
        }
    }

    private void GetlogExecutionTime(long duration) {
        String logFileName = "get_execution_times.log";
        lock_escrita_get.lock();
        try {
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(logFileName, true))) {
                writer.write(duration + " " + System.nanoTime() + "\n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        } finally {
            lock_escrita_get.unlock();
        }
    }





}
