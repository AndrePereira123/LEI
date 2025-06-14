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

			// as normais sao simplesmente o vetor da origem da esfera ao ponto
			// fazer normalizacao
			float len1 = sqrt(x1*x1 + y1*y1 + z1*z1);
			float len2 = sqrt(x2*x2 + y2*y2 + z2*z2);
			float len3 = sqrt(x3*x3 + y3*y3 + z3*z3);
			float len4 = sqrt(x4*x4 + y4*y4 + z4*z4);


			// desta vez, o s, equivalente ao "x", será calculado utilizando as slices que, variam utilizando o alpha
			// o t é de forma parecida ao s, mas com o beta

			// o inicial tem que ser (0,1)
			float s1 = alpha / (2*M_PI); // limitar até 1
			float t1 = 1.0f - ((beta + M_PI/2) / M_PI); //linha de cima, apenas varia com as stacks

			float s2 = nextAlpha / (2*M_PI);
			float t2 = 1.0f - ((nextBeta + M_PI/2) / M_PI);


            this->vertices.push_back(Vertex(x1,y1,z1, x1/len1,y1/len1,z1/len1, s1, t1));
            this->vertices.push_back(Vertex(x2,y2,z2, x2/len2,y2/len2,z2/len2, s1, t2));
            this->vertices.push_back(Vertex(x3,y3,z3, x3/len3,y3/len3,z3/len3, s2, t2));

            this->vertices.push_back(Vertex(x1,y1,z1, x1/len1,y1/len1,z1/len1, s1, t1));
            this->vertices.push_back(Vertex(x3,y3,z3, x3/len3,y3/len3,z3/len3, s2, t2));
            this->vertices.push_back(Vertex(x4,y4,z4, x4/len4,y4/len4,z4/len4, s2, t1));

		}
	}
}
