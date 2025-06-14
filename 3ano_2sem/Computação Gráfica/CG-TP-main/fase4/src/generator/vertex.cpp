#include "vertex.hpp"

Vertex::Vertex() {
    x = 0;
    y = 0;
    z = 0;
    x_normal = 0;
    y_normal = 0;
    z_normal = 0;
    s_text = 0;
    t_text = 0;
}

Vertex::Vertex(float xx, float yy, float zz,
               float xnormal, float ynormal, float znormal,
               float stext, float ttext) {
    x = xx;
    y = yy;
    z = zz;
    x_normal = xnormal;
    y_normal = ynormal;
    z_normal = znormal;
    s_text = stext;
    t_text = ttext;
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

float Vertex::getnormalX() const {
    return x_normal;
}

float Vertex::getnormalY() const {
    return y_normal;
}

float Vertex::getnormalZ() const {
    return z_normal;
}

float Vertex::gettextS() const {
    return s_text;
}

float Vertex::gettextT() const {
    return t_text;
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

void Vertex::setnormalX(float xx) {
    x_normal = xx;
}

void Vertex::setnormalY(float yy) {
    y_normal = yy;
}

void Vertex::setnormalZ(float zz) {
    z_normal = zz;
}

void Vertex::settextS(float s) {
    s_text = s;
}

void Vertex::settextT(float t) {
    t_text = t;
}