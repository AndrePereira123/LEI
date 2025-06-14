#ifndef LIST_HPP
#define LIST_HPP

#include "vertex.hpp"
#include <cstdlib>
#include <iostream>

class List {
private:
    Vertex* vertices;
    int numvertices;
    int capacidade;

    void expand() {
        int novaCapacidade = capacidade + 1000;

        Vertex* temp = (Vertex*) realloc(vertices, novaCapacidade * sizeof(Vertex));
        if (!temp) {
            std::cerr << "Erro ao alocar memoria\n";
            exit(1);
        }
        vertices = temp;
        capacidade = novaCapacidade;
    }

public:
    List() {
        capacidade = 1000;
        vertices = (Vertex*) malloc(capacidade * sizeof(Vertex));
        if (!vertices) {
            std::cerr << "Erro ao alocar memoria\n";
            exit(1);
        }
        numvertices = 0;
    }

    ~List() {
        if (vertices) {
            free(vertices);
            vertices = nullptr;
        }
    }

    void add(Vertex v) {
        if (numvertices >= capacidade) {
            expand();
        }
        vertices[numvertices++] = v;
    }

    Vertex get(int index) {
        if (index < 0 || index >= numvertices) {
            std::cerr << "Indice fora do intervalo: " << index << "\n";
            return Vertex(0, 0, 0, 0, 0, 0, 0, 0);
        }
        return vertices[index];
    }

    int size() {
        return numvertices;
    }
};

#endif
