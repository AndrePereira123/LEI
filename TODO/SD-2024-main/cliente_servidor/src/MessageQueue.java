import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class MessageQueue {
    private List<Frame> queue = new ArrayList<>();
    private ReentrantLock lock = new ReentrantLock();
    private Condition cond = lock.newCondition();
    public Boolean running = true;

    public void enqueue(Frame frame) {
        lock.lock();
        try {
            queue.add(frame);
            cond.signal();
        } finally {
            lock.unlock();
        }
    }

    public Frame dequeue() throws InterruptedException {
        lock.lock();
        try {
            while(queue.isEmpty() && running) {
                cond.await();
            }

            if (!running) {
                return null;
            }

            return queue.removeFirst();
        } finally {
            lock.unlock();
        }
    }

    public void stop() {
        lock.lock();
        try {
            running = false;
            cond.signal();
        } finally {
            lock.unlock();
        }
    }
}
