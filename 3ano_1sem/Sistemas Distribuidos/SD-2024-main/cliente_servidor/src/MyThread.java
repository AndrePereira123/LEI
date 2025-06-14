import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class MyThread extends Thread {
    ThreadPool threadPool;
    public MyThread() {
        this.threadPool = new ThreadPool();
    }


    class myThread extends Thread {
        private ArrayList<Task> tasks;
        static private ReentrantLock lock_global;
        static private Condition lock_global_condition;

        public myThread(ArrayList<Task> tasks, ReentrantLock tasks_lock, Condition queue_is_empty) {
            this.tasks = tasks;
            lock_global = tasks_lock;
            lock_global_condition = queue_is_empty;
        }

        @Override
        public void run(){
            Task task;
            while (true) {
                    lock_global.lock();
                    try {
                        while (tasks.isEmpty()) {
                           // System.out.println("Waiting for tasks...");
                            lock_global_condition.await();
                        }
                        task = threadPool.dequeueTask();
                    } catch (InterruptedException e) {
                        throw new RuntimeException(e);
                    } finally {
                        lock_global.unlock();
                    }
                task.run();
                }
        }
}
    class ThreadPool {
        static private ReentrantLock tasks_lock;
        static private Condition queue_is_full;
        static private Condition queue_is_empty;

        private List<Thread> pool_threads;
        static private ArrayList<Task> tasks_queue;
        private int max_queue_size = 25000;
        private int max_threads = 3;

        public ThreadPool() {
            this.pool_threads = new ArrayList<>();
            tasks_queue = new ArrayList<>();
            tasks_lock = new ReentrantLock();
            queue_is_full = tasks_lock.newCondition();
            queue_is_empty = tasks_lock.newCondition();

            for (int i = 0; i < max_threads; i++) {
                var t = new myThread(tasks_queue,tasks_lock,queue_is_empty);
                t.start();
                pool_threads.add(t);
            }
        }

        public void enqueueTask(Task tarefa) throws InterruptedException {
            tasks_lock.lock();
            try {
                while (tasks_queue.size() >= max_queue_size) {
                    queue_is_full.await();
                }
                tasks_queue.add(tarefa);
                queue_is_empty.signalAll();
            } finally {
                tasks_lock.unlock();
            }
        }

        public Task dequeueTask() throws InterruptedException {
            tasks_lock.lock();
            try {
                while (tasks_queue.isEmpty()) {
                    queue_is_empty.await();
                }

                Task task = tasks_queue.removeFirst();
                queue_is_full.signalAll();
                return task;

            } finally {
                tasks_lock.unlock();
            }
        }
}


}