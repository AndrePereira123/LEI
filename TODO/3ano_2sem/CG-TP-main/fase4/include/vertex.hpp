#ifndef VERTEX_HPP
#define VERTEX_HPP

class Vertex {
    private:
        float x, y, z;
        float x_normal, y_normal, z_normal;
        float s_text, t_text;


    public:
        Vertex();
        Vertex(float xx, float yy, float zz,
                float xnormal, float ynormal, float znormal,
                float stext, float ttext);
        
        float getX() const;
        float getY() const;
        float getZ() const;

        float getnormalX() const;
        float getnormalY() const;
        float getnormalZ() const;

        float gettextS() const;
        float gettextT() const;

        void setX(float x);
        void setY(float y);
        void setZ(float z);

        void setnormalX(float x);
        void setnormalY(float y);
        void setnormalZ(float z);

        void settextS(float s);
        void settextT(float t);
};


#endif