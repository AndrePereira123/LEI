#ifndef patch_HPP
#define patch_HPP

#include "shape.hpp"
#include "vertex.hpp"
#include <vector>
#include <string>

class Patch : public Shape {
    private:
        string patch_file;
        int tessellation;
        void processPatch(vector<vector<float>> P_x, vector<vector<float>> P_y, vector<vector<float>> P_z, int tessellation);

    public:
        Patch(string _patch_file, float _tessellation);
        void generate() override;
};

#endif
