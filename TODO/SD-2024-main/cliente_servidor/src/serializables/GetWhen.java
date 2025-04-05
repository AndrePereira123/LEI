package serializables;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class GetWhen implements Serialize {
    public String chave_real;
    public String chave_condicional;
    public byte[] valor_condicional;

    public GetWhen() {
        this.chave_real = null;
        this.chave_condicional = null;
        this.valor_condicional = null;
    }

    public GetWhen(String chave_real, String chave_condicional, byte[] valor_condicional) {
        this.chave_real = chave_real;
        this.chave_condicional = chave_condicional;
        this.valor_condicional = valor_condicional;
    }

    @Override
    public void serialize(DataOutputStream dos) throws IOException {
        dos.writeUTF(chave_real);
        dos.writeUTF(chave_condicional);
        dos.writeInt(valor_condicional.length);
        dos.write(valor_condicional);
        dos.flush();
    }

    @Override
    public Serialize deserialize(DataInputStream dis) throws IOException {
        String chave_real = dis.readUTF();
        String chave_condicional = dis.readUTF();
        byte[] valor_condicional = new byte[dis.readInt()];
        dis.readFully(valor_condicional);
        return new GetWhen(chave_real, chave_condicional, valor_condicional);
    }
}
