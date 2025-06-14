#include "tinyxml2.h"
#include "parser.hpp"
#include "vertex.hpp"
#include "list.hpp"

#include <iostream>
#include <vector>
#include <cstdio>

using namespace tinyxml2;

void parseGroup(XMLElement* group_element, Group& group) {
    if (!group_element) return;

    int order = 1;

    XMLElement* transform_element = group_element->FirstChildElement("transform");
    if (transform_element) {
        XMLElement* child_element = transform_element->FirstChildElement();
        while (child_element) {
            if(string(child_element->Value()) == "translate") {
                
                const char* alignAttr = child_element->Attribute("align");
                if (alignAttr) {

                    child_element->QueryIntAttribute("time", &group.transform.translate.time);
                    group.transform.translate.align = (string(alignAttr) == "true");

                    XMLElement* point_element = child_element->FirstChildElement("point");
                    while (point_element) {
                        float x, y, z;
                        point_element->QueryFloatAttribute("x", &x);
                        point_element->QueryFloatAttribute("y", &y);
                        point_element->QueryFloatAttribute("z", &z);

                        group.transform.translate.points.push_back(Vertex(x,y,z, 0, 0, 0, 0, 0));

                        point_element = point_element->NextSiblingElement("point");
                    }

                } else {
                    child_element->QueryFloatAttribute("x", &group.transform.translate.x);
                    child_element->QueryFloatAttribute("y", &group.transform.translate.y);
                    child_element->QueryFloatAttribute("z", &group.transform.translate.z);
                }
                
                group.transform.translate.order = order++;
            } else if (string(child_element->Value()) == "rotate") {

                child_element->QueryFloatAttribute("time", &group.transform.rotate.time);
                child_element->QueryFloatAttribute("angle", &group.transform.rotate.angle);
                child_element->QueryFloatAttribute("x", &group.transform.rotate.x);
                child_element->QueryFloatAttribute("y", &group.transform.rotate.y);
                child_element->QueryFloatAttribute("z", &group.transform.rotate.z);
                group.transform.rotate.order = order++;
            } else if (string(child_element->Value()) == "scale") {
                child_element->QueryFloatAttribute("x", &group.transform.scale.x);
                child_element->QueryFloatAttribute("y", &group.transform.scale.y);
                child_element->QueryFloatAttribute("z", &group.transform.scale.z);
                group.transform.scale.order = order++;
            }
            child_element = child_element->NextSiblingElement();
        }
    }

    XMLElement* models_element = group_element->FirstChildElement("models");
    if (models_element) {
        XMLElement* model_element = models_element->FirstChildElement("model");
        while (model_element) {
            Model m;
            const char* file = model_element->Attribute("file");
            if (file) {
                m.file = file;
            }

            XMLElement* texture_element = model_element->FirstChildElement("texture");
            if (texture_element) {
                const char* texture_path = texture_element->Attribute("file");
                if (texture_path) {
                    m.texture_file = texture_path;
                }
            }

            XMLElement* color_element = model_element->FirstChildElement("color");
            if (color_element) {
                XMLElement* diffuse_element = color_element->FirstChildElement("diffuse");
                if (diffuse_element) {
                    const char * diffuse_element_r = diffuse_element->Attribute("R");
                    if (diffuse_element_r) {
                        m.color.diffuse.R = diffuse_element_r;
                    } else {
                        m.color.diffuse.R = "200";
                    }

                    const char * diffuse_element_g = diffuse_element->Attribute("G");
                    if (diffuse_element_g) {
                        m.color.diffuse.G = diffuse_element_g;
                    } else {
                        m.color.diffuse.G = "200";
                    }

                    const char * diffuse_element_b = diffuse_element->Attribute("B");
                    if (diffuse_element_b) {
                        m.color.diffuse.B = diffuse_element_b;
                    } else {
                        m.color.diffuse.B = "200";
                    }
                } else {
                    m.color.diffuse.R = "200";
                    m.color.diffuse.G = "200";
                    m.color.diffuse.B = "200";
                }
                
                XMLElement* ambient_element = color_element->FirstChildElement("ambient");
                if (ambient_element) {
                    const char * ambient_element_r = ambient_element->Attribute("R");
                    if (ambient_element_r) {
                        m.color.ambient.R = ambient_element_r;
                    } else {
                        m.color.ambient.R = "50";
                    }

                    const char * ambient_element_g = ambient_element->Attribute("G");
                    if (ambient_element_g) {
                        m.color.ambient.G = ambient_element_g;
                    } else {
                        m.color.ambient.G = "50";
                    }

                    const char * ambient_element_b = ambient_element->Attribute("B");
                    if (ambient_element_b) {
                        m.color.ambient.B = ambient_element_b;
                    } else {
                        m.color.ambient.B = "50";
                    }
                } else {
                    m.color.ambient.R = "50";
                    m.color.ambient.G = "50";
                    m.color.ambient.B = "50";
                }
                
                XMLElement* specular_element = color_element->FirstChildElement("specular");
                if (specular_element) {
                    const char * specular_element_r = specular_element->Attribute("R");
                    if (specular_element_r) {
                        m.color.specular.R = specular_element_r;
                    } else {
                        m.color.specular.R = "0";
                    }

                    const char * specular_element_g = specular_element->Attribute("G");
                    if (specular_element_g) {
                        m.color.specular.G = specular_element_g;
                    } else {
                        m.color.specular.G = "0";
                    }

                    const char * specular_element_b = specular_element->Attribute("B");
                    if (specular_element_b) {
                        m.color.specular.B = specular_element_b;
                    } else {
                        m.color.specular.B = "0";
                    }
                } else {
                    m.color.specular.R = "0";
                    m.color.specular.G = "0";
                    m.color.specular.B = "0";
                }
                
                XMLElement* emissive_element = color_element->FirstChildElement("emissive");
                if (emissive_element) {
                    const char * emissive_element_r = emissive_element->Attribute("R");
                    if (emissive_element_r) {
                        m.color.emissive.R = emissive_element_r;
                    } else {
                        m.color.emissive.R = "0";
                    }

                    const char * emissive_element_g = emissive_element->Attribute("G");
                    if (emissive_element_g) {
                        m.color.emissive.G = emissive_element_g;
                    } else {
                        m.color.emissive.G = "0";
                    }

                    const char * emissive_element_b = emissive_element->Attribute("B");
                    if (emissive_element_b) {
                        m.color.emissive.B = emissive_element_b;
                    } else {
                        m.color.emissive.B = "0";
                    }
                } else {
                    m.color.emissive.R = "0";
                    m.color.emissive.G = "0";
                    m.color.emissive.B = "0";
                }
                
                XMLElement* shininess_element = color_element->FirstChildElement("shininess");
                if (shininess_element) {
                    const char* value = shininess_element->Attribute("value");
                    if (value) {
                        m.color.shininess.value = value;
                    } else {
                        m.color.shininess.value = "0";
                    }
                } else {
                    m.color.shininess.value = "0";
                }
            } else {
                m.color.diffuse.R = "200";
                m.color.diffuse.G = "200";
                m.color.diffuse.B = "200";

                m.color.ambient.R = "50";
                m.color.ambient.G = "50";
                m.color.ambient.B = "50";

                m.color.specular.R = "0";
                m.color.specular.G = "0";
                m.color.specular.B = "0";

                m.color.emissive.R = "0";
                m.color.emissive.G = "0";
                m.color.emissive.B = "0";

                m.color.shininess.value = "0";
            }

            group.models.push_back(m);

            model_element = model_element->NextSiblingElement("model");
        }
    }

    XMLElement* subgroup_element = group_element->FirstChildElement("group");
    while (subgroup_element) {
        Group subGroup;
        parseGroup(subgroup_element, subGroup);
        group.subgroups.push_back(subGroup);
        subgroup_element = subgroup_element->NextSiblingElement("group");
    }
}


bool parseXML(const char* filename, World& w) {
    XMLDocument doc;
    if (doc.LoadFile(filename) != XML_SUCCESS) {
        cerr << "Error loading the following XML file: " << filename << endl;
        return false;
    }

    XMLElement* world_element = doc.FirstChildElement("world");
    if (!world_element) {
        cerr << "The parameter \"world\" of the following XML file is missing: " << filename << endl;
        return false;
    }

    XMLElement* window_element = world_element->FirstChildElement("window");
    if (window_element) {
        window_element->QueryAttribute("width", &w.window.width);
        window_element->QueryAttribute("height", &w.window.height);
    } else {
        cerr << "The parameter \"window\" of the following XML file is missing: " << filename << endl;
        return false;
    }

    XMLElement* camera_element = world_element->FirstChildElement("camera");
    if (camera_element) {
        XMLElement* position_element = camera_element->FirstChildElement("position");
        if (position_element) {
            position_element->QueryFloatAttribute("x", &w.camera.position.x);
            position_element->QueryFloatAttribute("y", &w.camera.position.y);
            position_element->QueryFloatAttribute("z", &w.camera.position.z);
        } else {
            cerr << "The parameter \"camera_position\" of the following XML file is missing: " << filename << endl;
            return false;
        }

        XMLElement* lookAt_element = camera_element->FirstChildElement("lookAt");
        if (lookAt_element) {
            lookAt_element->QueryFloatAttribute("x", &w.camera.lookAt.x);
            lookAt_element->QueryFloatAttribute("y", &w.camera.lookAt.y);
            lookAt_element->QueryFloatAttribute("z", &w.camera.lookAt.z);
        } else {
            cerr << "The parameter \"camera_lookAt\" of the following XML file is missing: " << filename << endl;
            return false;
        }

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
        cerr << "The parameter \"camera\" of the following XML file is missing: " << filename << endl;
        return false;
    }

    XMLElement* lights_element = world_element->FirstChildElement("lights");
    if (lights_element) {
        XMLElement* light_element = lights_element->FirstChildElement("light");
        int lightCount = 0;

        while(light_element && lightCount < 8) {
            Light light;

            const char* type = light_element->Attribute("type");
            if (type) {
                light.type = type;
            } else {
                cerr << "Light without type attribute" << endl;
                light_element = light_element->NextSiblingElement("light");
                continue;
            }

            

            if (light.type == "point" || light.type == "spotlight" || light.type == "spot") {
                light_element->QueryFloatAttribute("posX", &light.posX);
                light_element->QueryFloatAttribute("posY", &light.posY);
                light_element->QueryFloatAttribute("posZ", &light.posZ);

                light_element->QueryFloatAttribute("posx", &light.posX);
                light_element->QueryFloatAttribute("posy", &light.posY);
                light_element->QueryFloatAttribute("posz", &light.posZ);
            }
            
            if (light.type == "directional" || light.type == "spotlight" || light.type == "spot") {
                light_element->QueryFloatAttribute("dirX", &light.dirX);
                light_element->QueryFloatAttribute("dirY", &light.dirY);
                light_element->QueryFloatAttribute("dirZ", &light.dirZ);

                light_element->QueryFloatAttribute("dirx", &light.dirX);
                light_element->QueryFloatAttribute("diry", &light.dirY);
                light_element->QueryFloatAttribute("dirz", &light.dirZ);
            }
            
            if (light.type == "spotlight" || light.type == "spot") {
                light_element->QueryFloatAttribute("cutoff", &light.cutoff);
            }

            w.lights.lights.push_back(light);
            lightCount++;

            light_element = light_element->NextSiblingElement("light");
        }
    }

    int group_i = 0;
    XMLElement* group_element = world_element->FirstChildElement("group");
    if (!group_element) {
        cerr << "The parameter \"group\" of the following XML file is missing: " << filename << endl;
        return false;
    }

    parseGroup(group_element, w.group);
    // tem que se tirar isto porque só existe um grupo principal

    return true;
}



void parseShapes(Group main_group, vector<List*>& listas, vector<vector<Transform>>& transformations, int* total_vertices, vector<Transform> current_transformations,
        vector<int*>& vertices, vector<string>& modelTextureFiles, vector<Color>& modelMaterials) {
    

    bool valid = false;

    current_transformations.push_back(main_group.transform);

    for (Model& m: main_group.models) {
        List* lista = new List();
        valid = true;
        string ficheiro = "../objects/" + m.file;
        FILE* file = fopen(ficheiro.c_str(), "r");
        if (!file) {
            cerr << "Error opening the following model file: " << ficheiro << "\n";
            continue;
        } 

        char buffer[256];
    
        if (fgets(buffer, sizeof(buffer), file)) {
            int* n_vertices = new int(std::atoi(buffer));
            *total_vertices += *n_vertices;
            vertices.push_back(n_vertices);
        }

        int i = 0;

        while (fgets(buffer, sizeof(buffer), file)) {
            Vertex v;
            float x, y, z;
            float normal_x, normal_y, normal_z;
            float stext, ttext;
            if (sscanf(buffer, "%f,%f,%f; %f,%f,%f; %f,%f", &x, &y, &z, &normal_x, &normal_y, &normal_z, &stext, &ttext) == 8) {
                v.setX(x);
                v.setY(y);
                v.setZ(z);
                v.setnormalX(normal_x);
                v.setnormalY(normal_y);
                v.setnormalZ(normal_z);
                v.settextS(stext);
                v.settextT(ttext);
                lista->add(v);
            }
        }
        fclose(file);

        if (valid) { 
            listas.push_back(lista);
        }    

        transformations.push_back(current_transformations);
        modelTextureFiles.push_back(m.texture_file);
        modelMaterials.push_back(m.color);
    }

    

    for (Group subgroup: main_group.subgroups) {
        parseShapes(subgroup, listas, transformations, total_vertices, current_transformations, vertices, modelTextureFiles, modelMaterials);
    }
}