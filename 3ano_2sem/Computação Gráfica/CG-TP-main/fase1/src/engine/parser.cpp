#include "tinyxml2.h"
#include "parser.hpp"
#include "vertex.hpp"
#include "list.hpp"

#include <iostream>
#include <vector>
#include <cstdio>

using namespace tinyxml2;

bool parseXML(const char* filename, World& w) {
    XMLDocument doc;
    if (doc.LoadFile(filename) != XML_SUCCESS) {
        cerr << "Error loading the following XML file: " << filename << endl;
        return false;
    }

    // world
    XMLElement* world_element = doc.FirstChildElement("world");
    if (!world_element) {
        cerr << "The parameter \"world\" of the following XML file is missing: " << filename << endl;
        return false;
    }

    // window
    XMLElement* window_element = world_element->FirstChildElement("window");
    if (window_element) {
        window_element->QueryAttribute("width", &w.window.width);
        window_element->QueryAttribute("height", &w.window.height);
    } else {
        cerr << "The parameter \"window\" of the following XML file is missing: " << filename << endl;
        return false;
    }

    // camera settings
    XMLElement* camera_element = world_element->FirstChildElement("camera");
    if (camera_element) {
        // position
        XMLElement* position_element = camera_element->FirstChildElement("position");
        if (position_element) {
            position_element->QueryFloatAttribute("x", &w.camera.position.x);
            position_element->QueryFloatAttribute("y", &w.camera.position.y);
            position_element->QueryFloatAttribute("z", &w.camera.position.z);
        } else {
            cerr << "The parameter \"camera_position\" of the following XML file is missing: " << filename << endl;
            return false;
        }

        // lookAt
        XMLElement* lookAt_element = camera_element->FirstChildElement("lookAt");
        if (lookAt_element) {
            lookAt_element->QueryFloatAttribute("x", &w.camera.lookAt.x);
            lookAt_element->QueryFloatAttribute("y", &w.camera.lookAt.y);
            lookAt_element->QueryFloatAttribute("z", &w.camera.lookAt.z);
        } else {
            cerr << "The parameter \"camera_lookAt\" of the following XML file is missing: " << filename << endl;
            return false;
        }

        // up, opcional
        XMLElement* up_element = camera_element->FirstChildElement("up");
        if (up_element) {
            up_element->QueryFloatAttribute("x", &w.camera.up.x);
            up_element->QueryFloatAttribute("y", &w.camera.up.y);
            up_element->QueryFloatAttribute("z", &w.camera.up.z);
        } else {
            w.camera.up.x = 0;
            w.camera.up.y = 1;
            w.camera.up.z = 0;
        }

        // projection
        XMLElement* projection_element = camera_element->FirstChildElement("projection");
        if (projection_element) {
            projection_element->QueryFloatAttribute("fov", &w.camera.projection.fov);
            projection_element->QueryFloatAttribute("near", &w.camera.projection.near);
            projection_element->QueryFloatAttribute("far", &w.camera.projection.far);
        } else {
            w.camera.projection.fov = 60;
            w.camera.projection.near = 1;
            w.camera.projection.far = 1000;
        }
    } else {
        cout << "The parameter \"camera\" of the following XML file is missing: " << filename << endl;
        return false;
    }

    // group
    XMLElement* group_element = world_element->FirstChildElement("group");
    if (group_element) {
        XMLElement* models_element = group_element->FirstChildElement("models");
        if (models_element) {
            XMLElement* model_element = models_element->FirstChildElement("model");
            if (model_element) {
                while (model_element) {
                    Model m;
                    const char* file = model_element->Attribute("file");
                    if (file) {
                        m.file = file;
                        w.group.models.push_back(m);
                    }
                    model_element = model_element->NextSiblingElement("model");
                }
            } else {
                cout << "The parameter \"group_models_model\" of the following XML file is missing: " << filename << endl;
                return false;
            }

        } else {
            cout << "The parameter \"group_models\" of the following XML file is missing: " << filename << endl;
            return false;
        }
    } else {
        cout << "The parameter \"group\" of the following XML file is missing: " << filename << endl;
        return false;
    }

    return true;
}


List* parseShapes(vector<Model>& models) {
    List* lista = new List();

    int total_vertices = 0;

    for (Model& m: models) {
        string ficheiro = "../objects/" + m.file;
        FILE* file = fopen(ficheiro.c_str(), "r");
        if (!file) {
            cerr << "Error opening the following model file: " << ficheiro << "\n";
            continue;
        }
        
        char buffer[256];
    
        if(fgets(buffer, sizeof(buffer), file)) {
            total_vertices += std::atoi(buffer);
        } 

        int i = 0;

        while (fgets(buffer, sizeof(buffer), file)) {
            Vertex v;
            float x, y, z;
            if (sscanf(buffer, "%f,%f,%f", &x, &y, &z) == 3) {
                v.setX(x);
                v.setY(y);
                v.setZ(z);
                lista->add(v);
            }
        }
        fclose(file);
    }

    
    if (total_vertices == lista->size() && total_vertices != 0) {
        return lista;
    } else {
        return NULL;
    }
}