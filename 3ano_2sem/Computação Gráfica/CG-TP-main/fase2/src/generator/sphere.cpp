#include "sphere.hpp"
#include <math.h>

Sphere::Sphere(float _radius, int _slices, int _stacks) {
    radius = _radius;
    slices = _slices;
    stacks = _stacks;
}

void Sphere::generate() {
	float deltaBeta = M_PI / stacks; //latitude

	float deltaAlpha = 2 * M_PI / slices; //longitude

	for (int i = 0; i < stacks; i++) {
		float beta = (M_PI / 2) - i * deltaBeta;
		float nextBeta = (M_PI / 2) - (i+1) * deltaBeta;

		for (int j = 0; j < slices; j++) {

			float alpha = j * deltaAlpha;
			float nextAlpha = (j + 1) * deltaAlpha;

			float x1 = radius * cos(beta) * sin(alpha);
			float y1 = radius * sin(beta);
			float z1 = radius * cos(beta) * cos(alpha);

			float x2 = radius * cos(nextBeta) * sin(alpha);
			float y2 = radius * sin(nextBeta);
			float z2 = radius * cos(nextBeta) * cos(alpha);

			float x3 = radius * cos(nextBeta) * sin(nextAlpha);
			float y3 = radius * sin(nextBeta);
			float z3 = radius * cos(nextBeta) * cos(nextAlpha);

			float x4 = radius * cos(beta) * sin(nextAlpha);
			float y4 = radius * sin(beta);
			float z4 = radius * cos(beta) * cos(nextAlpha);

		/*
		 p1--p4
		 | \ |
		 |  \|
		 p2--p3

		*/

            this->vertices.push_back(Vertex(x1,y1,z1));
            this->vertices.push_back(Vertex(x2,y2,z2));
            this->vertices.push_back(Vertex(x3,y3,z3));

            this->vertices.push_back(Vertex(x1,y1,z1));
            this->vertices.push_back(Vertex(x3,y3,z3));
            this->vertices.push_back(Vertex(x4,y4,z4));

		}
	}
}
