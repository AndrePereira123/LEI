#ifndef CONE_HPP
#define CONE_HPP

#include "shape.hpp"
#include "vertex.hpp"
#include <vector>
#include <string>

class Cone : public Shape {
    private:
        float radius;
        float height;
        int slices;
        int stacks;

    public:
        Cone(float _radius, float _height, int _slices, int _stacks);
        void generate() override;
};

#endif
