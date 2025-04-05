package serializables;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

public class SetChave implements Serialize {
    public final Set<String> chaves;

    public SetChave() {
        this.chaves = null;
    }

    public SetChave(Set<String> chaves) {
        this.chaves = chaves;
    }


    @Override
    public void serialize(DataOutputStream dos) throws IOException {
        dos.writeInt(chaves.size());
        for(String chave: chaves) {
            dos.writeUTF(chave);
        }
        dos.flush();
    }

    @Override
    public Serialize deserialize(DataInputStream dis) throws IOException {
        Set<String> set = new HashSet<>();
        int size = dis.readInt();
        for (int i = 0; i < size; i++) {
            set.add(dis.readUTF());
        }
        return new SetChave(set);
    }
}
