package serializables;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Account implements Serialize {
    public final String user;
    public final String password;

    public Account(String user, String password) {
        this.user = user;
        this.password = password;
    }

    public Account() {
        this.user = null;
        this.password = null;
    }


    @Override
    public void serialize(DataOutputStream dos) throws IOException {
        dos.writeUTF(user);
        dos.writeUTF(password);
    }

    @Override
    public Serialize deserialize(DataInputStream dis) throws IOException {
        String user = dis.readUTF();
        String password = dis.readUTF();
        return new Account(user, password);
    }

}
