#ifndef PLANE_HPP
#define PLANE_HPP

#include "shape.hpp"
#include "vertex.hpp"
#include <vector>
#include <string>

class Plane : public Shape {
    private:
        float length;
        int divisions;

    public:
        Plane(float l, int d);
        void generate() override;
};

#endif
