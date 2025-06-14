import serializables.*;

import java.io.*;
import java.net.Socket;
import java.util.concurrent.locks.ReentrantLock;

public class TaggedConnection implements AutoCloseable {
    private Socket socket;
    private DataOutputStream dos;
    private DataInputStream dis;
    private ReentrantLock lockSend = new ReentrantLock();
    private ReentrantLock lockReceive = new ReentrantLock();

    public TaggedConnection(Socket socket) throws IOException {
        this.socket = socket;
        this.dos = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));
        this.dis = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
    }

    public void send(Frame frame) throws IOException {
        lockSend.lock();
        try {
            dos.writeInt(frame.tag);
            frame.data.serialize(dos);
            dos.flush();
        } finally {
            lockSend.unlock();
        }
    }

    public void send(int tag, Serialize data) throws IOException {
        send(new Frame(tag, data));
    }

    public Frame receive() {
        lockReceive.lock();
        try {
            int tag = dis.readInt();
            Serialize data;

            switch (tag) {
                // response
                case 0:
                    data = new Response().deserialize(dis);
                    break;


                // register
                case 1, 2:
                    data = new Account().deserialize(dis);
                    break;

                // entrada
                case 3:
                    data = new Entrada().deserialize(dis);
                    break;

                // chave
                case 4:
                    data = new Chave().deserialize(dis);
                    break;

                // MapEntrada
                case 5:
                    data = new MapEntrada().deserialize(dis);
                    break;

                // SetChave
                case 6:
                    data = new SetChave().deserialize(dis);
                    break;

                // GetWhen
                case 7:
                    data = new GetWhen().deserialize(dis);
                    break;

                // GetWhen resposta
                case 8:
                    data = new Entrada().deserialize(dis);
                    break;


                default:
                    throw new IOException("tag inválido");
            }

            return new Frame(tag, data);
        }catch (IOException e){
            return null;
        } finally {
            lockReceive.unlock();
        }
    }

    @Override
    public void close() throws IOException {
        socket.close();
    }
}
