#include "patch.hpp"
#include <math.h>
#include "vertex.hpp"

void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

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
            float dU[4] = { 3 * u * u, 2 * u, 1.0f, 0.0f };

            float U_next[4] = { u_next * u_next * u_next, u_next * u_next, u_next, 1.0f };
            float dU_next[4] = { 3 * u_next * u_next, 2 * u_next, 1.0f, 0.0f };
            
            float V[4] = { v * v * v, v * v, v, 1.0f };
            float dV[4] = { 3 * v * v, 2 * v, 1.0f, 0.0f };
            
            float V_next[4] = { v_next * v_next * v_next, v_next * v_next, v_next, 1.0f };
            float dV_next[4] = { 3 * v_next * v_next, 2 * v_next, 1.0f, 0.0f };

            // pontos da superfície
            float x1 = generateBezierSurface(U, V, P_x);
            float y1 = generateBezierSurface(U, V, P_y);
            float z1 = generateBezierSurface(U, V, P_z);

            float x1_du = generateBezierSurface(dU, V, P_x);
            float y1_du = generateBezierSurface(dU, V, P_y);
            float z1_du = generateBezierSurface(dU, V, P_z);

            float x1_dv = generateBezierSurface(U, dV, P_x);
            float y1_dv = generateBezierSurface(U, dV, P_y);
            float z1_dv = generateBezierSurface(U, dV, P_z);

            float v_p1[3] = { x1_dv, y1_dv, z1_dv };
            float u_p1[3] = { x1_du, y1_du, z1_du };
            normalize(v_p1);
            normalize(u_p1);
            float n_p1[3];
            cross(v_p1, u_p1, n_p1);





            float x2 = generateBezierSurface(U_next, V, P_x);
            float y2 = generateBezierSurface(U_next, V, P_y);
            float z2 = generateBezierSurface(U_next, V, P_z);

            float x2_du = generateBezierSurface(dU_next, V, P_x);
            float y2_du = generateBezierSurface(dU_next, V, P_y);
            float z2_du = generateBezierSurface(dU_next, V, P_z);

            float x2_dv = generateBezierSurface(U_next, dV, P_x);
            float y2_dv = generateBezierSurface(U_next, dV, P_y);
            float z2_dv = generateBezierSurface(U_next, dV, P_z);

            float v_p2[3] = { x2_dv, y2_dv, z2_dv };
            float u_p2[3] = { x2_du, y2_du, z2_du };
            normalize(v_p2);
            normalize(u_p2);
            float n_p2[3];
            cross(v_p2, u_p2, n_p2);


            float x3 = generateBezierSurface(U, V_next, P_x);
            float y3 = generateBezierSurface(U, V_next, P_y);
            float z3 = generateBezierSurface(U, V_next, P_z);

            float x3_du = generateBezierSurface(dU, V_next, P_x);
            float y3_du = generateBezierSurface(dU, V_next, P_y);
            float z3_du = generateBezierSurface(dU, V_next, P_z);

            float x3_dv = generateBezierSurface(U, dV_next, P_x);
            float y3_dv = generateBezierSurface(U, dV_next, P_y);
            float z3_dv = generateBezierSurface(U, dV_next, P_z);

            float v_p3[3] = { x3_dv, y3_dv, z3_dv };
            float u_p3[3] = { x3_du, y3_du, z3_du };
            normalize(v_p3);
            normalize(u_p3);
            float n_p3[3];
            cross(v_p3, u_p3, n_p3);




            float x4 = generateBezierSurface(U_next, V_next, P_x);
            float y4 = generateBezierSurface(U_next, V_next, P_y);
            float z4 = generateBezierSurface(U_next, V_next, P_z);

            float x4_du = generateBezierSurface(dU_next, V_next, P_x);
            float y4_du = generateBezierSurface(dU_next, V_next, P_y);
            float z4_du = generateBezierSurface(dU_next, V_next, P_z);

            float x4_dv = generateBezierSurface(U_next, dV_next, P_x);
            float y4_dv = generateBezierSurface(U_next, dV_next, P_y);
            float z4_dv = generateBezierSurface(U_next, dV_next, P_z);

            float v_p4[3] = { x4_dv, y4_dv, z4_dv };
            float u_p4[3] = { x4_du, y4_du, z4_du };
            normalize(v_p4);
            normalize(u_p4);
            float n_p4[3];
            cross(v_p4, u_p4, n_p4);




            /* (x1, y1, z1) ---- (x2, y2, z2)
                |          /      |
                |        /        |
                |      /          |
               (x3, y3, z3) ---- (x4, y4, z4) */
                         
            // Triângulo 1: (x1, y1, z1), (x3, y3, z3), (x2, y2, z2)
            this->vertices.push_back(Vertex(x1, y1, z1, n_p1[0], n_p1[1], n_p1[2], u, v));
            this->vertices.push_back(Vertex(x3, y3, z3, n_p3[0], n_p3[1], n_p3[2], u, v_next));
            this->vertices.push_back(Vertex(x2, y2, z2, n_p2[0], n_p2[1], n_p2[2], u_next, v));

            // Triângulo 2: (x2, y2, z2), (x3, y3, z3), (x4, y4, z4)
            this->vertices.push_back(Vertex(x2, y2, z2, n_p2[0], n_p2[1], n_p2[2], u_next, v));
            this->vertices.push_back(Vertex(x3, y3, z3, n_p3[0], n_p3[1], n_p3[2], u, v_next));
            this->vertices.push_back(Vertex(x4, y4, z4, n_p4[0], n_p4[1], n_p4[2], u_next, v_next));
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
        control_points.push_back(Vertex(x, y, z, 0, 0, 0, 0, 0));
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
