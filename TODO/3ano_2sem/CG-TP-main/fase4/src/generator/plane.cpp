#include "plane.hpp"

Plane::Plane(float l, int d) {
    length = l;
    divisions = d;
}

void Plane::generate() {

    float step = length / divisions;
    float start = -length / 2.0f;

    float textStep = 1.0f / divisions;

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {

            float x1 = start + j * step;
            float z1 = start + i * step;
            float x2 = x1 + step;
            float z2 = z1 + step;

            float s1 = (float)j / divisions;
            float t1 = 1.0f - (float)i / divisions;
            float s2 = (float)(j + 1) / divisions;
            float t2 = 1.0f - (float)(i + 1) / divisions;

            /*
            ------
            | \  |
            |  \ |
            ------
            */

            // triangulo 1
            this->vertices.push_back(Vertex(x1, 0, z1, 0, 1, 0, s1, t1));
            this->vertices.push_back(Vertex(x1, 0, z2, 0, 1, 0, s1, t2));
            this->vertices.push_back(Vertex(x2, 0, z2, 0, 1, 0, s2, t2));

            // triangulo 2
            this->vertices.push_back(Vertex(x2, 0, z1, 0, 1, 0, s2, t1));
            this->vertices.push_back(Vertex(x1, 0, z1, 0, 1, 0, s1, t1));
            this->vertices.push_back(Vertex(x2, 0, z2, 0, 1, 0, s2, t2));
        }
    }
}
