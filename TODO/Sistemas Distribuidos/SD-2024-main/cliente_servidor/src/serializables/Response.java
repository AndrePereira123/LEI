package serializables;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Response implements Serialize {
    public final String message;
    public final Boolean status;

    public Response() {
        this.message = null;
        this.status = false;
    }

    public Response(String message, Boolean status) {
        this.message = message;
        this.status = status;
    }



    @Override
    public void serialize(DataOutputStream dos) throws IOException {
        dos.writeUTF(message);
        dos.writeBoolean(status);
    }

    @Override
    public Serialize deserialize(DataInputStream dis) throws IOException {
        String message = dis.readUTF();
        Boolean status = dis.readBoolean();
        return new Response(message, status);
    }
}
