//
// Created by Antoine on 17/09/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "hash_tables.h"


int edge_pos_in_tri(int v1, int v2, Triangle t){
    // Check if v1 and v2 are valid vertex indices
    if (v1 < 0 || v1 > 2 || v2 < 0 || v2 > 2) {
        return -1;
    }

    // Check if the edge v1 -> v2 is part of the triangle
    if ((t.index[0] == v1 && t.index[1] == v2) || (t.index[1] == v1 && t.index[2] == v2) || (t.index[2] == v1 && t.index[0] == v2)) {
        // Determine the position of the edge in the triangle
        if (t.index[0] == v1 && t.index[1] == v2) {
            return 0;
        } else if (t.index[1] == v1 && t.index[2] == v2) {
            return 1;
        } else {
            return 2;
        }
    } else {
        // Edge v1 -> v2 is not part of the triangle
        return -1;
    }
}

#include <stdbool.h>

int tris_are_neighbors(int tri1, int tri2, const Mesh2D* m) {
    // Get the triangles from the mesh using their indices
    Triangle t1 = m->tri[tri1];
    Triangle t2 = m->tri[tri2];

    // Iterate over the edges of t1 to check if any of them matches an edge of t2
    for (int i = 0; i < 3; i++) {
        int v1 = t1.index[i];
        int v2 = t1.index[(i + 1) % 3];  // Next vertex in t1

        // Check if edge v1 -> v2 in t1 is also an edge in t2
        int pos_in_t2 = edge_pos_in_tri(v1, v2, t2);
        if (pos_in_t2 != -1) {
            // Edge v1 -> v2 in t1 matches an edge in t2
            return i + 1;  // Return position (1-based index)
        }
    }

    // If no matching edges were found, the triangles are not neighbors
    return 0;
}

int * build_agency_table1(const Mesh2D* m) {

    int ntri = m->nt;

    // Initialization of the agency table
    int * adj = (int*)malloc(3*ntri * sizeof(int));
    if (adj == NULL) return NULL;
    for (int i=0; i<3*ntri; i++){
        adj[i] = -1;
    }

    for(int tri1=0; tri1<ntri; tri1++){
        for(int tri2=0; tri2<ntri; tri2++) {
            if(tri1 == tri2) continue;
            int pos = tris_are_neighbors(tri1, tri2, m);
            if(pos > 0){
                adj[3*tri1+pos-1]= tri2;
            }
        }
    }
    return adj;
}

HashTable * build_edge_table1(const Mesh2D *m){

    int ntri = m->nt;

    // Initialization of the hashtable
    HashTable * htable = createHashTable(3*ntri);

    // Loop over all triangles in the mesh
    for (int i = 0; i< ntri; i++) {
        Triangle t = m->tri[i];

        // Iterate over the edges of the triangle
        for (int j = 0; j < 3; j++) {
            int v1 = t.index[j];
            int v2 = t.index[(j + 1) % 3];

            // Create an Edge
            Edge edge;
            edge.v1 = v1;
            edge.v2 = v2;

            // Create the unique key for each edge
            char key[20];
            sprintf(key, "Edge-%d-%d", v1, v2);

            // Insert the edge into the hash table with the triangle index as the value
            insert(htable, key, &i);
        }
    }
    return htable;
}

int * build_adjacency_table2(const Mesh2D *m){

    HashTable * edgetable = build_edge_table1(m);


    int ntri = m->nt;
    int* adjacencyTable = (int*)malloc(3 * ntri * sizeof(int));
    if (adjacencyTable == NULL) {
        // Handle memory allocation error
        return NULL;
    }

    // Initialize the adjacency table with -1
    for (int i = 0; i < 3 * ntri; i++) {
        adjacencyTable[i] = -1;
    }

    // Loop over all triangles in the mesh
    for (int i = 0; i < ntri; i++) {
        Triangle T = m->tri[i];

        // Iterate over the edges of the triangle
        for (int j = 0; j < 3; j++) {
            int v1 = T.index[j];
            int v2 = T.index[(j + 1) % 3];

            // Create an Edge struct for the edge
            Edge edge;
            edge.v1 = v1;
            edge.v2 = v2;

            // Look up the opposing edge with reversed vertices in the edge table
            Edge reversedEdge;
            reversedEdge.v1 = v2;
            reversedEdge.v2 = v1;

            char key[20];
            sprintf(key,"Edge-%d-%d",v1,v2);

            int* oppositeTri = (int*)get(edgetable, key);

            // If an opposing edge is found, update the adjacency table
            if (oppositeTri != NULL) {
                adjacencyTable[3 * i + j] = *oppositeTri;
            }
        }
    }

    // Free the edge table since it's no longer needed
    destroyHashTable(edgetable);

    return adjacencyTable;
}

void edge_table_initialize(EdgeTable *et, int nvert, int ntri){
    et->head = (int*)malloc(nvert*sizeof(int));
    if(et->head == NULL){
        printf("malloc error\n");
        exit(EXIT_FAILURE);
    }
    et->next = (int*)malloc(3*ntri*sizeof(int));
    if(et->next == NULL){
        printf("malloc error\n");
        exit(EXIT_FAILURE);
    }
}

void edge_table_dispose(EdgeTable *et){
    free(et->head);
    free(et->next);
    free(et);
}

void edge_table_insert(int v1, int edge_code, EdgeTable *et){
    if(et->head[v1] == -1){
        et->head[v1] = edge_code;
    } else{
        int tmp = et->head[v1];
        et->head[v1] = edge_code;
        et->next[edge_code] = tmp;
    }
}

int edge_table_find(int v1, int v2, const EdgeTable *et, const Mesh2D *m) {
    // Check if v1 is a valid vertex index within the mesh
    if (v1 < 0 || v1 >= m->nv) {
        return -1; // Invalid vertex index
    }

    // Check if v2 is a valid vertex index within the mesh
    if (v2 < 0 || v2 >= m->nv) {
        return -1; // Invalid vertex index
    }

    // Iterate through the linked list of edges for vertex v1
    int edge_code = et->head[v1];
    while (edge_code != -1) {
        // Determine which triangle and edge within that triangle it corresponds to
        int triangle_index = edge_code / 3;
        int edge_index = edge_code % 3;

        // Get the vertex indices of the triangle
        int v3 = m->tri[triangle_index].index[edge_index];
        int v4 = m->tri[triangle_index].index[(edge_index + 1) % 3];

        // Check if the edge connects v1 and v2
        if ((v3 == v1 && v4 == v2) || (v3 == v2 && v4 == v1)) {
            return edge_code; // Found the edge
        }

        // Move to the next edge in the linked list
        edge_code = et->next[edge_code];
    }

    // Edge not found
    return -1;
}

EdgeTable * build_edge_table3(const Mesh2D *m){
    EdgeTable * edgeTable = (EdgeTable *)malloc(sizeof(EdgeTable));
    edge_table_initialize(edgeTable,m->nv,m->nt);

}


int main(){

    return 0;
}