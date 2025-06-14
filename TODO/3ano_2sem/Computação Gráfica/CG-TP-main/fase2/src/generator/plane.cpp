#include "plane.hpp"

Plane::Plane(float l, int d) {
    length = l;
    divisions = d;
}

void Plane::generate() {

    float step = length / divisions;
    float start = -length / 2.0f;

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {

            float x1 = start + j * step;
            float z1 = start + i * step;
            float x2 = x1 + step;
            float z2 = z1 + step;

            // triangulo 1
            this->vertices.push_back(Vertex(x1, 0, z1));
            this->vertices.push_back(Vertex(x1, 0, z2));
            this->vertices.push_back(Vertex(x2, 0, z2));

            // triangulo 2
            this->vertices.push_back(Vertex(x2, 0, z1));
            this->vertices.push_back(Vertex(x1, 0, z1));
            this->vertices.push_back(Vertex(x2, 0, z2));
        }
    }
}
