#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "aibb.h"

int main(int argc, char *argv[]){

int linhas_m1, colunas_m1; /* Lines are vertices and columns are edges */
float *mat1;

char *file1 = argv[1];

/* ############################################################################### */
// Parte serial do código (Descomentar essa região) e digite make serial

/*
mat1 = lerMatriz(file1, &linhas_m1, &colunas_m1);    
//imprimirMatriz(mat1, linhas_m1, colunas_m1);

BellmanFord_serial(mat1, linhas_m1);
*/

/* ############################################################################### */


/* ############################################################################### */
// Parte paralela do código (Descomentar essa região) e digite make paralell

int qtd_threads =  atoi(argv[2]);

mat1 = lerMatriz(file1, &linhas_m1, &colunas_m1);    
//imprimirMatriz(mat1, linhas_m1, colunas_m1);
omp_set_num_threads(qtd_threads);
BellmanFord_paralelo(mat1, linhas_m1, qtd_threads);

/* ############################################################################### */

return 0;

}