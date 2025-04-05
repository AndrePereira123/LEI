package serializables;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class Entrada implements Serialize {
    public final String chave;
    public final byte[] valor;

    public Entrada() {
        this.chave = null;
        this.valor = null;
    }

    public Entrada(String chave, byte[] valor) {
        this.chave = chave;
        this.valor = valor;
    }

    @Override
    public void serialize(DataOutputStream dos) throws IOException {
        dos.writeUTF(chave);
        dos.writeInt(valor.length);
        dos.write(valor);
        dos.flush();
    }

    @Override
    public Serialize deserialize(DataInputStream dis) throws IOException {
        String chave = dis.readUTF();
        int size = dis.readInt();
        byte[] val = new byte[size];
        dis.readFully(val);
        return new Entrada(chave, val);
    }

    public String toString() {
        return "Chave: " + chave + " - Valor: " + new String(valor, StandardCharsets.UTF_8);
    }
}
