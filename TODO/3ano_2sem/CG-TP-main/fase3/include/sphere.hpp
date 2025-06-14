#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"
#include "vertex.hpp"
#include <vector>
#include <string>

class Sphere : public Shape {
    private:
        float radius;
        int slices;
        int stacks;

    public:
        Sphere(float _radius, int _slices, int _stacks);
        void generate() override;
};

#endif
