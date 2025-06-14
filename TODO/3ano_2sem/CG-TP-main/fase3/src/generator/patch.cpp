#include "patch.hpp"
#include <math.h>
#include "vertex.hpp"

float generateBezierSurface(float U[4], float V[4], vector<vector<float>> P) {
    
    
    // matriz M e Mt(M e a transposta sao iguais)
    float M[4][4] = {
        {-1.0f,  3.0f, -3.0f,  1.0f},
        { 3.0f, -6.0f,  3.0f,  0.0f},
        {-3.0f,  3.0f,  0.0f,  0.0f},
        { 1.0f,  0.0f,  0.0f,  0.0f}
    };
    
    
    
    // U * M
    float UM[4] = {0.0f};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            UM[i] += U[j] * M[j][i];
        }
    }

    // P * Mt
    float temp[4][4] = {0.0f};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                temp[i][j] += P[i][k] * M[k][j];
            }
        }
    }

    // UM * temp
    float temp2[4] = {0.0f};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp2[i] += UM[j] * temp[j][i];
        }
    }

    // temp2 * v

    float resultado = 0.0f;

    for (int i = 0; i < 4; i++) {
        resultado += temp2[i] * V[i];
    }

    return resultado;
}

void Patch::processPatch(vector<vector<float>> P_x, vector<vector<float>> P_y, vector<vector<float>> P_z, int tessellation) {
    float step = 1.0f / tessellation;

    for (int i = 0; i < tessellation; i++) {
        float u = i * step;
        float u_next = (i + 1) * step;

        for (int j = 0; j < tessellation; j++) {
            float v = j * step;
            float v_next = (j + 1) * step;

            
            float U[4] = { u * u * u, u * u, u, 1.0f };
            float U_next[4] = { u_next * u_next * u_next, u_next * u_next, u_next, 1.0f };
            float V[4] = { v * v * v, v * v, v, 1.0f };
            float V_next[4] = { v_next * v_next * v_next, v_next * v_next, v_next, 1.0f };

            // pontos da superfície
            float x1 = generateBezierSurface(U, V, P_x);
            float y1 = generateBezierSurface(U, V, P_y);
            float z1 = generateBezierSurface(U, V, P_z);

            float x2 = generateBezierSurface(U_next, V, P_x);
            float y2 = generateBezierSurface(U_next, V, P_y);
            float z2 = generateBezierSurface(U_next, V, P_z);

            float x3 = generateBezierSurface(U, V_next, P_x);
            float y3 = generateBezierSurface(U, V_next, P_y);
            float z3 = generateBezierSurface(U, V_next, P_z);

            float x4 = generateBezierSurface(U_next, V_next, P_x);
            float y4 = generateBezierSurface(U_next, V_next, P_y);
            float z4 = generateBezierSurface(U_next, V_next, P_z);

            /* (x1, y1, z1) ---- (x2, y2, z2)
                |          /      |
                |        /        |
                |      /          |
               (x3, y3, z3) ---- (x4, y4, z4) */
                         
            // Triângulo 1: (x1, y1, z1), (x3, y3, z3), (x2, y2, z2)
            this->vertices.push_back(Vertex(x1, y1, z1));
            this->vertices.push_back(Vertex(x3, y3, z3));
            this->vertices.push_back(Vertex(x2, y2, z2));

            // Triângulo 2: (x2, y2, z2), (x3, y3, z3), (x4, y4, z4)
            this->vertices.push_back(Vertex(x2, y2, z2));
            this->vertices.push_back(Vertex(x3, y3, z3));
            this->vertices.push_back(Vertex(x4, y4, z4));
        }
    }
}

Patch::Patch(string _patch_file, float _tessellation) {
    patch_file = "../patches/" + _patch_file;
    tessellation = _tessellation;
}

void Patch::generate() {
    FILE* file = fopen(patch_file.c_str(), "r");


    int num_patches;
    fscanf(file, "%d\n", &num_patches);
    
    vector<vector<int>> patch_indices;

    for (int i = 0; i < num_patches; i++) {
        vector<int> indices;
        for (int j = 0; j < 15; j++) {
            int buf = 0;
            fscanf(file, "%d, ", &buf);
            indices.push_back(buf);
        }
        int buf = 0;
        fscanf(file, "%d\n", &buf);
        indices.push_back(buf);
        patch_indices.push_back(indices);
    }

    int num_control_points;
    fscanf(file, "%d\n", &num_control_points);

    vector<Vertex> control_points;

    for (int i = 0; i < num_control_points; i++) {
        float x, y, z;
        fscanf(file, " %f, %f, %f\n", &x, &y, &z);
        control_points.push_back(Vertex(x, y, z));
    } 
    

    for (int i = 0; i < num_patches; i++) {
        vector<int> indices = patch_indices[i];

        vector<vector<float>> P_x(4, vector<float>(4));
        vector<vector<float>> P_y(4, vector<float>(4));
        vector<vector<float>> P_z(4, vector<float>(4));

        for (int u = 0; u < 4; u++) {
            for (int v = 0; v < 4; v++) {
                // preencher a matriz P com os pontos dos indices
                int indice = indices[u * 4 + v];
                P_x[u][v] = control_points[indice].getX();
                P_y[u][v] = control_points[indice].getY();
                P_z[u][v] = control_points[indice].getZ();
            }
        }

        processPatch(P_x, P_y, P_z, tessellation);

    }    
}
