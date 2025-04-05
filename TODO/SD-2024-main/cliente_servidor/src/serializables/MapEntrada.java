package serializables;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

public class MapEntrada implements Serialize {
    public final Map<String, byte[]> mapa;


    public MapEntrada() {
        this.mapa = null;
    }

    public MapEntrada(Map<String, byte[]> mapa) {
        this.mapa = mapa;
    }

    @Override
    public void serialize(DataOutputStream dos) throws IOException {
        int tamanho = mapa.size();
        dos.writeInt(tamanho);
        for (String chave: mapa.keySet()) {
            dos.writeUTF(chave);
            byte[] valor = mapa.get(chave);
            dos.writeInt(valor.length);
            dos.write(valor);
        }
        dos.flush();
    }

    @Override
    public Serialize deserialize(DataInputStream dis) throws IOException {
        Map<String, byte[]> map = new HashMap<>();
        int tamanho = dis.readInt();
        for (int i = 0; i < tamanho; i++) {
            String key = dis.readUTF();
            byte[] toInsert = new byte[dis.readInt()];
            dis.readFully(toInsert);
            map.put(key, toInsert);
        }
        return new MapEntrada(map);
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (String chave: mapa.keySet()) {
            sb.append("Chave: ").append(chave).append(" - Valor: ").append(new String(mapa.get(chave), StandardCharsets.UTF_8)).append("\n");
        }
        return sb.toString();
    }

}
