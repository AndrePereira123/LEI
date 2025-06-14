#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include "vertex.hpp"

using namespace std;

class Shape {
    protected:
        vector<Vertex> vertices;

    public:
        virtual void generate() = 0; // obriga as classes a "implementarem" este método

        void writeToFile(const char* filename) {
            string fullPath = "../objects/" + string(filename);
            
            FILE* file = fopen(fullPath.c_str(), "w");
            if (!file) {
                cerr << "Erro ao abrir o ficheiro " << filename << endl;
                return;
            }

            fprintf(file, "%lu\n", vertices.size());
            for (Vertex v : vertices) {
                fprintf(file, "%f,%f,%f\n", v.getX(), v.getY(), v.getZ());
            }

            fclose(file);
        }
};

#endif
