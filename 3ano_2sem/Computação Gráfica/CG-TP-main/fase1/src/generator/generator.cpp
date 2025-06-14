#include <string>
#include "plane.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "cone.hpp"
#include <regex>
using namespace std;


// falta fazer verificação de erros
int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Invalid input.\nUsage: ./generator <plane|box|sphere|cone> <arg_0> ... <arg_n>" << endl;
        return 1;
    }

    string primitive_type = argv[1];


    try {
        if (primitive_type == "plane") {
            if (argc != 5) {
                cerr << "Invalid input.\nUsage: ./generator plane <length> <divisions> <output.3d>" << endl;
                return 1;
            }
            float length = stof(argv[2]);
            int divisions = stoi(argv[3]);
            if (length <= 0 || divisions <= 0) {
                cerr << "The values \"length\" and \"divions\" must be positive." << endl;
                return 1;
            }
            string name_file = argv[4];
            if (!regex_match(name_file, regex("^.*\\.3d$"))) {
                cerr << "The output file must end with .3d" << endl;
                return 1;
            }
            Plane plano = Plane(length, divisions);
            plano.generate();
            plano.writeToFile(name_file.c_str());
        } 
        else if (primitive_type == "box") {
            if (argc != 5) {
                cerr << "Invalid input.\nUsage: ./generator box <length> <divisions> <output.3d>" << endl;
                return 1;
            }
            float length = stof(argv[2]);
            int divisions = stoi(argv[3]);
            if (length <= 0 || divisions <= 0) {
                cerr << "The values \"length\" and \"divions\" must be positive." << endl;
                return 1;
            }
            string name_file = argv[4];
            if (!regex_match(name_file, regex("^.*\\.3d$"))) {
                cerr << "The output file must end with .3d" << endl;
                return 1;
            }
            Box box = Box(length, divisions);
            box.generate();
            box.writeToFile(name_file.c_str());
        }
        else if (primitive_type == "sphere") {
            if (argc != 6) {
                cerr << "Invalid input.\nUsage: ./generator sphere <radius> <slices> <stacks> <output.3d>" << endl;
                return 1;
            }
            float radius = stof(argv[2]);
            int slices = stoi(argv[3]);
            int stacks = stoi(argv[4]);
            if (radius <= 0 || slices <= 0 || stacks <= 0) {
                cerr << "The values \"radius\", \"slices\" and \"stacks\" must be positive." << endl;
                return 1;
            }
            string name_file = argv[5];
            if (!regex_match(name_file, regex("^.*\\.3d$"))) {
                cerr << "The output file must end with .3d" << endl;
                return 1;
            }
            Sphere esfera = Sphere(radius, slices, stacks);
            esfera.generate();
            esfera.writeToFile(name_file.c_str());
        }
        else if (primitive_type == "cone") {
            if (argc != 7) {
                cerr << "Invalid input.\nUsage: ./generator cone <radius> <height> <slices> <stacks> <output.3d>" << endl;
                return 1;
            }
            float radius = stof(argv[2]);
            float height = stof(argv[3]);
            int slices = stoi(argv[4]);
            int stacks = stoi(argv[5]);
            if (radius <= 0 || height <= 0 || slices <= 0 || stacks <= 0) {
                cerr << "The values \"radius\", \"height\", \"slices\" and \"stacks\" must be positive." << endl;
                return 1;
            }
            string name_file = argv[6];
            if (!regex_match(name_file, regex("^.*\\.3d$"))) {
                cerr << "The output file must end with .3d" << endl;
                return 1;
            }
            Cone cone = Cone(radius, height, slices, stacks);
            cone.generate();
            cone.writeToFile(name_file.c_str());
        } else {
            cerr << "Invalid input.\nUsage: ./generator <plane|box|sphere|cone> <arg_0> ... <arg_n>" << endl;
            return 1;
        }
    } catch (const invalid_argument & e) {
        cerr << "Error in the following function: " << e.what() << "\nThe following values must a number: length, divisions, radius, slices, stacks, height." << endl;
        return 1;
    }
    catch (const out_of_range & e) {
        cerr << "Error in the following function: " << e.what() << "\nThe following values must a number: length, divisions, radius, slices, stacks, height." << endl;
        return 1;
    }

    cout << "The object was created successfuly." << endl;

    return 0;
}