import serializables.Serialize;

public class Frame {
    public final int tag;
    public final Serialize data;

    public Frame (int tag, Serialize data) {
        this.tag = tag;
        this.data = data;
    }
}