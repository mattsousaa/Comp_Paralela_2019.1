#include <stdio.h>
#include "aibb.h"

#define INFINITY 99999999

/*
This implementation takes in a graph, represented as
lists of vertices and edges, and fills two arrays
(distance and predecessor) about the shortest path
from the source to each vertex
*/

void BellmanFord_serial(int vertices, float *mat1){

float distancia[vertices];
int predecessor[vertices];
int i, j;

/* Step 1: initialize graph */

for (i = 0; i < vertices; i++){
    distancia[i] = INFINITY;    /* Initialize the distance to all vertices to infinity */
    predecessor[i] = -1;        /* And having a null predecessor (-1) */
}

distancia[0] = 0;               /* The distance from the source to itself is, of course, zero */

/* Step 2: relax edges repeatedly */

for (i = 0; i < vertices; i++){
    for (j = 0; j < vertices; j++){
        if ((distancia[i] + *(mat1 + i*vertices + j)) < distancia[j]){
            distancia[j] = distancia[i] + *(mat1 + i*vertices + j);
            predecessor[j] = i;
        }
    }
}

/* Step 3: check for negative-weight cycles */

for (j = 0; j < vertices; j++){
    if ((distancia[i] + *(mat1 + i*vertices + j)) > distancia[j])
        printf("Graph contains a negative-weight cycle\n");
}

/* Print distancy and predecessor */

for(i = 0; i < vertices; i++)
    printf("distancia[%d]: %.2f, predecessor[%d]: %d\n", i, distancia[i], i, predecessor[i]);

}

int main(int argc, char *argv[]){

int linhas_m1, colunas_m1; /* Lines are vertices and columns are edges */
float *mat1;

char *file1 = argv[1];

mat1 = lerMatriz(file1, &linhas_m1, &colunas_m1);    
//imprimirMatriz(mat1, linhas_m1, colunas_m1);

BellmanFord_serial(linhas_m1, mat1);

return 0;

}