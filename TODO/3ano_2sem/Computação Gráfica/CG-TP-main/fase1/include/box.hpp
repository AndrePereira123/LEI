#ifndef BOX_HPP
#define BOX_HPP

#include "shape.hpp"
#include "vertex.hpp"
#include <vector>
#include <string>

class Box : public Shape {
    private:
        float length;
        int divisions;

    public:
        Box(float l, int d);
        void generate() override;
};

#endif
