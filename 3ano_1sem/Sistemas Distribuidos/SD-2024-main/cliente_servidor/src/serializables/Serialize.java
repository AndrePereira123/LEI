package serializables;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public interface Serialize {
    public void serialize(DataOutputStream dos) throws IOException;
    public Serialize deserialize(DataInputStream dis) throws IOException;
}
