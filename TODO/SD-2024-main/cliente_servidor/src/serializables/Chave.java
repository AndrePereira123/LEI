package serializables;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Chave implements Serialize {
    public final String chave;

    public Chave() {
        this.chave = null;
    }

    public Chave(String chave) {
        this.chave = chave;
    }

    @Override
    public void serialize(DataOutputStream dos) throws IOException {
        dos.writeUTF(chave);
        dos.flush();
    }

    @Override
    public Serialize deserialize(DataInputStream dis) throws IOException {
        String chave = dis.readUTF();
        return new Chave(chave);
    }
}
