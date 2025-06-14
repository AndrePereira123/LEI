#include "ring.hpp"
#include <math.h>


Ring::Ring(int s, float in_radius, float out_radius) {
    slices = s;
    inner_radius = in_radius;
    outter_radius = out_radius;
}

void Ring::generate() {
    float angle_step = 2 * M_PI / slices;
    float alpha = 0;

    for (int i = 0; i < slices; i++) {
        float x1_in = inner_radius * cos(alpha);
        float z1_in = inner_radius * sin(alpha);

        float x1_out = (inner_radius + outter_radius) * cos(alpha);
        float z1_out = (inner_radius + outter_radius) * sin(alpha);
        

        float x2_in = inner_radius * cos(alpha + angle_step);
        float z2_in = inner_radius * sin(alpha + angle_step);

        float x2_out = (inner_radius + outter_radius) * cos(alpha + angle_step);
        float z2_out = (inner_radius + outter_radius) * sin(alpha + angle_step);

        float s1 = alpha / (2 * M_PI);
        float s2 = (alpha + angle_step) / (2 * M_PI);
        float t_in = 0.0f;
        float t_out = 1.0f;

        this->vertices.push_back(Vertex(x1_in, 0, z1_in, 0, 1, 0, s1, t_in)); 
        this->vertices.push_back(Vertex(x2_in, 0, z2_in, 0, 1, 0, s2, t_in)); 
        this->vertices.push_back(Vertex(x2_out, 0, z2_out, 0, 1, 0, s2, t_out));

        this->vertices.push_back(Vertex(x1_in, 0, z1_in, 0, 1, 0, s1, t_in)); 
        this->vertices.push_back(Vertex(x2_out, 0, z2_out, 0, 1, 0, s2, t_out));
        this->vertices.push_back(Vertex(x1_out, 0, z1_out, 0, 1, 0, s1, t_out));


        // parte de baixo
        this->vertices.push_back(Vertex(x1_in, 0, z1_in, 0, -1, 0, s1, t_in)); 
        this->vertices.push_back(Vertex(x2_out, 0, z2_out, 0, -1, 0, s2, t_out));
        this->vertices.push_back(Vertex(x2_in, 0, z2_in, 0, -1, 0, s2, t_in)); 

        this->vertices.push_back(Vertex(x1_in, 0, z1_in, 0, -1, 0, s1, t_in)); 
        this->vertices.push_back(Vertex(x1_out, 0, z1_out, 0, -1, 0, s1, t_out));
        this->vertices.push_back(Vertex(x2_out, 0, z2_out, 0, -1, 0, s2, t_out));


        alpha += angle_step;
    }
}
