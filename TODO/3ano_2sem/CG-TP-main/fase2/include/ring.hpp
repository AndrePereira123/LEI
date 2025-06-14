#ifndef RING_HPP
#define RING_HPP

#include "shape.hpp"
#include "vertex.hpp"
#include <vector>
#include <string>

class Ring : public Shape {
    private:
        int slices;
        float inner_radius;
        float outter_radius;

    public:
        Ring(int s, float in_radius, float out_radius);
        void generate() override;
};

#endif
