#ifndef VERTEX_HPP
#define VERTEX_HPP

class Vertex {
    private:
        float x, y, z;

    public:
        Vertex();
        Vertex(float x, float y, float z);
        
        float getX() const;
        float getY() const;
        float getZ() const;

        void setX(float x);
        void setY(float y);
        void setZ(float z);
};


#endif