#ifndef PARSER_HPP
#define PARSER_HPP

#include "list.hpp"
#include "vertex.hpp"

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

struct Diffuse {
    string R, G, B;
};

struct Ambient {
    string R, G, B;
};

struct Specular {
    string R, G, B;
};

struct Emissive {
    string R, G, B;
};

struct Shininess {
    string value;
};

struct Color {
    Diffuse diffuse;
    Ambient ambient;
    Specular specular;
    Emissive emissive;
    Shininess shininess;
};

struct Model {
    string file;
    string texture_file;
    Color color;
};

struct Translate {
    float x = 0, y = 0, z = 0;
    int order = -1, time = 0;
    vector<Vertex> points;
    bool align = false;
};

struct Rotate {
    float angle = 0, x = 0, y = 0, z = 0, time = 0;
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

struct Light {
    string type;
    float posX, posY, posZ;
    float dirX, dirY, dirZ;
    float cutoff;
};

struct Lights {
    vector<Light> lights;
};

struct World {
    Window window;
    Camera camera;
    Group group;
    Lights lights;
};

bool parseXML(const char* filename, World& w);

void parseShapes(Group main_group, vector<List*>& listas, vector<vector<Transform>>& transformations, int* total_vertices, vector<Transform> current_transformations, vector<int*>& vertices, vector<string>& modelTextureFiles, vector<Color>& modelMaterials);

#endif