//
// Created by Antoine on 19/09/2023.
//

#include "main.h"
#include "hash_tables.h"

int main(int argc, char** argv){

    const char* mesh_file_name = argv[1];

    // load mesh data

    Mesh2D * m =;

    EdgeTable * edgeTable1 = build_adjacency_table1(m);
    EdgeTable * edgeTable2 = build_adjacency_table2(m);
    EdgeTable * edgeTable3 = build_adjacency_table3(m);

}
