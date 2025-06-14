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

			// baixo
			this->vertices.push_back(Vertex(x1, -half_length, z1));
            this->vertices.push_back(Vertex(x2, -half_length, z1));
            this->vertices.push_back(Vertex(x1, -half_length, z2));

            this->vertices.push_back(Vertex(x2, -half_length, z2));
            this->vertices.push_back(Vertex(x1, -half_length, z2));
            this->vertices.push_back(Vertex(x2, -half_length, z1));

            // cima
            this->vertices.push_back(Vertex(x1, half_length, z1));
            this->vertices.push_back(Vertex(x1, half_length, z2));
            this->vertices.push_back(Vertex(x2, half_length, z2));

            this->vertices.push_back(Vertex(x2, half_length, z1));
            this->vertices.push_back(Vertex(x1, half_length, z1));
            this->vertices.push_back(Vertex(x2, half_length, z2));
        }
    }

    // plano xy
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float x1 = start + j * step;
            float y1 = start + i * step;
            float x2 = x1 + step;
            float y2 = y1 + step;

            // frente
            this->vertices.push_back(Vertex(x1, y1, half_length));
            this->vertices.push_back(Vertex(x2, y1, half_length));
            this->vertices.push_back(Vertex(x2, y2, half_length));

            this->vertices.push_back(Vertex(x2, y2, half_length));
            this->vertices.push_back(Vertex(x1, y2, half_length));
            this->vertices.push_back(Vertex(x1, y1, half_length));


            // trás
            this->vertices.push_back(Vertex(x1, y1, -half_length));
            this->vertices.push_back(Vertex(x1, y2, -half_length));
            this->vertices.push_back(Vertex(x2, y2, -half_length));

            this->vertices.push_back(Vertex(x2, y1, -half_length));
            this->vertices.push_back(Vertex(x1, y1, -half_length));
            this->vertices.push_back(Vertex(x2, y2, -half_length));
        }
    }

    // plano yz
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float y1 = start + j * step;
            float z1 = start + i * step;
            float y2 = y1 + step;
            float z2 = z1 + step;

            // esquerda
            this->vertices.push_back(Vertex(-half_length, y1, z1));
            this->vertices.push_back(Vertex(-half_length, y1, z2));
            this->vertices.push_back(Vertex(-half_length, y2, z2));

            this->vertices.push_back(Vertex(-half_length, y2, z2));
            this->vertices.push_back(Vertex(-half_length, y2, z1));
            this->vertices.push_back(Vertex(-half_length, y1, z1));

            // direita
            this->vertices.push_back(Vertex(half_length, y1, z2));
            this->vertices.push_back(Vertex(half_length, y1, z1));
            this->vertices.push_back(Vertex(half_length, y2, z2));

            this->vertices.push_back(Vertex(half_length, y1, z1));
            this->vertices.push_back(Vertex(half_length, y2, z1));
            this->vertices.push_back(Vertex(half_length, y2, z2));
        }
    }
}
