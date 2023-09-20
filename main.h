//
// Created by Antoine on 17/09/2023.
//

#ifndef TP3_MAIN_H
#define TP3_MAIN_H



// Define the Vertex structure for 2D
typedef struct{
    double x;
    double y;
} Vertex ;

// Define the Triangle structure
typedef struct {
    int index[3];
} Triangle;

// Define the Mesh2D structure
typedef struct{
    int nv; // Number of vertices
    Vertex * vert;
    int nt; // Number of triangles
    Triangle * tri;
} Mesh2D ;

typedef struct{
    int v1;
    int v2;
} Edge;

typedef struct{
    int * head;
    int * next;
} EdgeTable;


int edge_pos_in_tri(int v1, int v2, Triangle t);

int tris_are_neighbors(int tri1, int tri2, const Mesh2D* m);

int * build_adjacency_table1(const Mesh2D* m);

int * build_adjacency_table2(const Mesh2D* m);

void edge_table_initialize(EdgeTable *et, int nvert, int ntri);

void edge_table_dispose(EdgeTable *et);

void edge_table_insert(int v1, int edge_code, EdgeTable *et);

int edge_table_find(int v1, int v2, const EdgeTable *et, const Mesh2D *m);

EdgeTable * build_edge_table3(const Mesh2D *m);

int *build_adjacency_table3(const Mesh2D *m);

#endif //TP3_MAIN_H
