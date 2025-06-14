#ifndef PARSER_HPP
#define PARSER_HPP

#include "list.hpp"

#include <iostream>
#include <vector>

using namespace std;

struct Window {
    int width, height;
};

struct Position {
    float x, y, z;
};

struct LookAt {
    float x, y, z;
};

struct Up {
    float x, y, z;
};

struct Projection {
    float fov, near, far;
};

struct Camera {
    Position position;
    LookAt lookAt;
    Up up;
    Projection projection;
};

struct Model {
    string file;
};

struct Group {
    vector<Model> models;
};

struct World {
    Window window;
    Camera camera;
    Group group;
};

bool parseXML(const char* filename, World& w);

List* parseShapes(vector<Model>& models);


#endif