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

		//centro
		float s1 = 0.5f;
		float t1 = 0.5f;

		// cos e sin pq depende do angulo alpha, pontos da borda
		float s2 = 0.5f + 0.5f * cos(alpha);
		float t2 = 0.5f + 0.5f * sin(alpha);

		// mesma logica do de cima
		float s3 = 0.5f + 0.5f * cos(nextAlpha);
		float t3 = 0.5f + 0.5f * sin(nextAlpha);


		float nextxx = radius * cos(nextAlpha);
		float nextzz = radius * sin(nextAlpha);

        this->vertices.push_back(Vertex(0,0,0, 0, -1, 0, s1, t1));
        this->vertices.push_back(Vertex(xx,0,zz, 0, -1, 0, s2, t2));
        this->vertices.push_back(Vertex(nextxx,0,nextzz, 0, -1, 0, s3, t3));

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

			// utilizando pontos do trianglo da para calcular a normal da superficie
			float nx1 = cos(alpha);
			float nz1 = sin(alpha);
			float ny1 = radius / height;
			
			float nx2 = cos(nextAlpha);
			float nz2 = sin(nextAlpha);
			float ny2 = radius / height;

			float len1 = sqrt(nx1*nx1 + ny1*ny1 + nz1*nz1);
			nx1 /= len1;
			ny1 /= len1;
			nz1 /= len1;
			
			float len2 = sqrt(nx2*nx2 + ny2*ny2 + nz2*nz2);
			nx2 /= len2;
			ny2 /= len2;
			nz2 /= len2;


			// o s, uma vez que corresponde ao "x", pode ser definido recorrendo às slices
			// de forma semelhante, o u que corresponde ao "y" pode ser definido às stacks

			// ponto p1
			float s1 = (float)j / slices;
			float t1 = 1.0f - (float)i / stacks; // o primeiro ponto é 1.0f

			// ponto p2
			float s2 = (float)j / slices;
			float t2 = 1.0f - (float)(i+1) / stacks;

			// ponto p3
			float s3 = (float)(j+1) / slices;
			float t3 = 1.0f - (float)(i+1) / stacks;

			// ponto p4
			float s4 = (float)(j+1) / slices;
			float t4 = 1.0f - (float)i / stacks;



			// First triangle
			this->vertices.push_back(Vertex(x1,y1,z1, nx1, ny1, nz1, s1, t1));
			this->vertices.push_back(Vertex(x2,y2,z2, nx1, ny1, nz1, s2, t2));
			this->vertices.push_back(Vertex(x3,y2,z3, nx2, ny2, nz2, s3, t3));

			// Second triangle
			this->vertices.push_back(Vertex(x1,y1,z1, nx1, ny1, nz1, s1, t1));
			this->vertices.push_back(Vertex(x3,y2,z3, nx2, ny2, nz2, s3, t3));
			this->vertices.push_back(Vertex(x4,y1,z4, nx2, ny2, nz2, s4, t4));

		}

	}

    
}
