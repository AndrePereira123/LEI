import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Manager  {
    private Map<String, String> users = new HashMap<>();
    private Map<String, Boolean> users_connectados = new HashMap<>();
    private ReentrantLock usersLock = new ReentrantLock();
    private Condition can_connect = usersLock.newCondition();

    private Map<String, Condition> conds_for_GetWhen = new HashMap<>();
    private ReentrantLock conditionLock = new ReentrantLock();

    private Map<String, byte[]> database = new HashMap<>();
    private ReentrantReadWriteLock databaseLock = new ReentrantReadWriteLock();

    private int active_users = 0;
    private int MaxUsers;

    public Manager(int N) {
        this.MaxUsers = N;
    }

    public boolean register(String username, String password) {
        usersLock.lock();
        try {
            if (!users.containsKey(username)) {
                this.users.put(username, password);
                this.users_connectados.put(username, false);
                return true;
            }
            return false;
        } finally {
            usersLock.unlock();
        }
    }

    public boolean login(String loginUsername, String loginPassword) {
        usersLock.lock();
        try {
            boolean valid_login = false;
            try{
                var password = this.users.get(loginUsername);
                if (password.equals(loginPassword)) {
                    valid_login = true;

                    while (active_users >= MaxUsers) {
                        can_connect.await();
                    }

                    active_users++;

                    return valid_login;
                }
                return valid_login;
            } catch (NullPointerException e) {
                return valid_login;
            }
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } finally {
            usersLock.unlock();
        }
    }

    public void disconnect(String loginUserName) {
        usersLock.lock();
        try {
            users_connectados.put(loginUserName, false);
            active_users--;
            can_connect.signalAll();
        } finally {
            usersLock.unlock();
        }
    }

    public void put(String key, byte[] value) {
        databaseLock.writeLock().lock();
        try {
            this.database.put(key,value);
            conditionLock.lock();
            try {
                if (conds_for_GetWhen.containsKey(key)) {
                    conds_for_GetWhen.get(key).signalAll();
                }
            } finally {
                conditionLock.unlock();
            }
        } finally {
            databaseLock.writeLock().unlock();
        }
    }

    void multiPut(Map<String, byte[]> pairs) {
        databaseLock.writeLock().lock();
        try {
            for(String chave: pairs.keySet()) {
                this.database.put(chave, pairs.get(chave));
                conditionLock.lock();
                try {
                    if (conds_for_GetWhen.containsKey(chave)) {
                        conds_for_GetWhen.get(chave).signalAll();
                    }
                } finally {
                    conditionLock.unlock();
                }
            }
        }
        catch (NullPointerException e)
        {
         e.printStackTrace();
        }
        finally {
            databaseLock.writeLock().unlock();
        }
    }



    public byte[] get(String key) {
        databaseLock.readLock().lock();
        try {
            byte[] as = this.database.get(key);
            return as;
        }finally {
            databaseLock.readLock().unlock();
        }
    }

    public Map<String, byte[]> multiGet(Set<String> keys)
    {
        databaseLock.readLock().lock();
        try {
            Map<String, byte[]> pairs = new HashMap<>();
            for (String key : keys)
            {
                var val = database.get(key);
                if (val != null) {
                    pairs.put(key, database.get(key));
                } else {
                    pairs.put(key, "null".getBytes(StandardCharsets.UTF_8));
                }
            }
            return pairs;
        }finally {
            databaseLock.readLock().unlock();
        }
    }

    public byte[] GetWhen(String key, String key_condicional, byte[] valor_condicional) throws InterruptedException {
        conditionLock.lock();
        try {
            conds_for_GetWhen.computeIfAbsent(key_condicional, k -> conditionLock.newCondition());

            while(true) {
                databaseLock.readLock().lock();
                try {
                    if (Arrays.equals(database.get(key_condicional), valor_condicional)) {
                        return database.get(key);
                    }
                } finally {
                    databaseLock.readLock().unlock();
                }
                conds_for_GetWhen.get(key_condicional).await();
            }
        } finally {
            conditionLock.unlock();
        }
    }
}



