#include "vertex.hpp"

Vertex::Vertex() {
    x = 0;
    y = 0;
    z = 0;
}

Vertex::Vertex(float xx, float yy, float zz) {
    x = xx;
    y = yy;
    z = zz;
}

float Vertex::getX() const {
    return x;
}

float Vertex::getY() const {
    return y;
}

float Vertex::getZ() const {
    return z;
}

void Vertex::setX(float xx) {
    x = xx;
}

void Vertex::setY(float yy) {
    y = yy;
}

void Vertex::setZ(float zz) {
    z = zz;
}