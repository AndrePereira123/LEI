#include "box.hpp"

Box::Box(float l, int d) {
    length = l;
    divisions = d;
}

void Box::generate() {

    float half_length = length * 0.5;
    float step = length / divisions;
    float start = -half_length;

    // plano xz
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

			// baixo
			this->vertices.push_back(Vertex(x1, -half_length, z1, 0, -1, 0, s1, t1));
            this->vertices.push_back(Vertex(x2, -half_length, z1, 0, -1, 0, s2, t1));
            this->vertices.push_back(Vertex(x1, -half_length, z2, 0, -1, 0, s1, t2));

            this->vertices.push_back(Vertex(x2, -half_length, z2, 0, -1, 0, s2, t2));
            this->vertices.push_back(Vertex(x1, -half_length, z2, 0, -1, 0, s1, t2));
            this->vertices.push_back(Vertex(x2, -half_length, z1, 0, -1, 0, s2, t1));

            // cima
            this->vertices.push_back(Vertex(x1, half_length, z1, 0, 1, 0, s1, t1));
            this->vertices.push_back(Vertex(x1, half_length, z2, 0, 1, 0, s1, t2));
            this->vertices.push_back(Vertex(x2, half_length, z2, 0, 1, 0, s2, t2));

            this->vertices.push_back(Vertex(x2, half_length, z1, 0, 1, 0, s2, t1));
            this->vertices.push_back(Vertex(x1, half_length, z1, 0, 1, 0, s1, t1));
            this->vertices.push_back(Vertex(x2, half_length, z2, 0, 1, 0, s2, t2));
        }
    }

    // plano xy
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float x1 = start + j * step;
            float y1 = start + i * step;
            float x2 = x1 + step;
            float y2 = y1 + step;

            float s1 = (float)j / divisions;
            float t1 = 1.0f - (float)i / divisions;
            float s2 = (float)(j + 1) / divisions;
            float t2 = 1.0f - (float)(i + 1) / divisions;

            // frente
            this->vertices.push_back(Vertex(x1, y1, half_length, 0, 0, 1, s1, t1));
            this->vertices.push_back(Vertex(x2, y1, half_length, 0, 0, 1, s2, t1));
            this->vertices.push_back(Vertex(x2, y2, half_length, 0, 0, 1, s2, t2));

            this->vertices.push_back(Vertex(x2, y2, half_length, 0, 0, 1, s2, t2));
            this->vertices.push_back(Vertex(x1, y2, half_length, 0, 0, 1, s1, t2));
            this->vertices.push_back(Vertex(x1, y1, half_length, 0, 0, 1, s1, t1));


            // trás
            this->vertices.push_back(Vertex(x1, y1, -half_length, 0, 0, -1, s1, t1));
            this->vertices.push_back(Vertex(x1, y2, -half_length, 0, 0, -1, s1, t2));
            this->vertices.push_back(Vertex(x2, y2, -half_length, 0, 0, -1, s2, t2));

            this->vertices.push_back(Vertex(x2, y1, -half_length, 0, 0, -1, s2, t1));
            this->vertices.push_back(Vertex(x1, y1, -half_length, 0, 0, -1, s1, t1));
            this->vertices.push_back(Vertex(x2, y2, -half_length, 0, 0, -1, s2, t2));
        }
    }

    // plano yz
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float y1 = start + j * step;
            float z1 = start + i * step;
            float y2 = y1 + step;
            float z2 = z1 + step;

            //trocar i e j e os vertices dos pontos centrais

            float s1 = (float)i / divisions;
            float t1 = 1.0f - (float)j / divisions;
            float s2 = (float)(i + 1) / divisions;
            float t2 = 1.0f - (float)(j + 1) / divisions;

            // esquerda
            this->vertices.push_back(Vertex(-half_length, y1, z1, -1, 0, 0, s1, t1));
            this->vertices.push_back(Vertex(-half_length, y1, z2, -1, 0, 0, s2, t1));
            this->vertices.push_back(Vertex(-half_length, y2, z2, -1, 0, 0, s2, t2));

            this->vertices.push_back(Vertex(-half_length, y2, z2, -1, 0, 0, s2, t2));
            this->vertices.push_back(Vertex(-half_length, y2, z1, -1, 0, 0, s1, t2));
            this->vertices.push_back(Vertex(-half_length, y1, z1, -1, 0, 0, s1, t1));

            // direita
            this->vertices.push_back(Vertex(half_length, y1, z2, 1, 0, 0, s2, t1));
            this->vertices.push_back(Vertex(half_length, y1, z1, 1, 0, 0, s1, t1));
            this->vertices.push_back(Vertex(half_length, y2, z2, 1, 0, 0, s2, t2));

            this->vertices.push_back(Vertex(half_length, y1, z1, 1, 0, 0, s1, t1));
            this->vertices.push_back(Vertex(half_length, y2, z1, 1, 0, 0, s1, t2));
            this->vertices.push_back(Vertex(half_length, y2, z2, 1, 0, 0, s2, t2));
        }
    }
}
