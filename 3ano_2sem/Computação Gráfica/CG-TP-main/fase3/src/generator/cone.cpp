#include "cone.hpp"
#include <math.h>

Cone::Cone(float _radius, float _height, int _slices, int _stacks) {
    radius = _radius;
    height = _height;
    slices = _slices;
    stacks = _stacks;
}

void Cone::generate() {
    float deltaAlpha = 2 * M_PI / slices;

	float deltaHeight = height / stacks;

	// base
	for (int i = 0; i < slices; i++) {
		float alpha = i * deltaAlpha;
		float nextAlpha = (i+1) * deltaAlpha;

		float xx = radius * cos(alpha);
		float zz = radius * sin(alpha);

		float nextxx = radius * cos(nextAlpha);
		float nextzz = radius * sin(nextAlpha);

        this->vertices.push_back(Vertex(0,0,0));
        this->vertices.push_back(Vertex(xx,0,zz));
        this->vertices.push_back(Vertex(nextxx,0,nextzz));

	}

    // lado
	for (int i = 0; i < stacks; i++) {
		float y1 = i * deltaHeight;
		float y2 = (i + 1) * deltaHeight;

		float r1 = radius * (1 - ((float)i / stacks));
		float r2 = radius * (1- ((float)(i+1) / stacks));

		for (int j = 0; j < slices; j++) {
			float alpha = j * deltaAlpha;
			float nextAlpha = (j + 1) * deltaAlpha;

			float x1 = r1 * cos(alpha);
			float z1 = r1 * sin(alpha);

			float x2 = r2 * cos(alpha);
			float z2 = r2 * sin(alpha);

			float x3 = r2 * cos(nextAlpha);
			float z3 = r2 * sin(nextAlpha);

			float x4 = r1 * cos(nextAlpha);
			float z4 = r1 * sin(nextAlpha);

			/*
			p1--p4 diferente x
			| \ |
			|  \|
			p2--p3	diferente x (z e y da anterior)

			p1-p4
		      \
			   \
			    \
				p2-p3
			vista lateral - plano yz, por exemplo
			*/

            this->vertices.push_back(Vertex(x1,y1,z1));
            this->vertices.push_back(Vertex(x2,y2,z2));
            this->vertices.push_back(Vertex(x3,y2,z3));

            this->vertices.push_back(Vertex(x1,y1,z1));
            this->vertices.push_back(Vertex(x3,y2,z3));
            this->vertices.push_back(Vertex(x4,y1,z4));

		}

	}

    
}
