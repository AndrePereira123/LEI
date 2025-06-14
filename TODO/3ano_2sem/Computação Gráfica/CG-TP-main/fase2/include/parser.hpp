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

struct Translate {
    float x = 0, y = 0, z = 0;
    int order = -1;
};

struct Rotate {
    float angle = 0, x = 0, y = 0, z = 0;
    int order = -1;
};

struct Scale {
    float x = 0, y = 0, z = 0;
    int order = -1;
};

struct Transform {
    Translate translate;
    Rotate rotate;
    Scale scale;
};

struct Group {
    vector<Model> models;
    Transform transform;
    vector<Group> subgroups;
};

struct World {
    Window window;
    Camera camera;
    Group group;
};

bool parseXML(const char* filename, World& w);

void parseShapes(Group main_group, vector<List*>& listas, vector<vector<Transform>>& transformations, int* total_vertices, vector<Transform> current_transformations);

#endif