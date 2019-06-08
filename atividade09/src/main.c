#include <stdlib.h>
#include "aibb.h"

int main(int argc, char *argv[]){

int linhas_m1, colunas_m1;
int linhas_m2, colunas_m2;
double *mat1, *mat2, *result;

char *file1 = argv[1];
char *file2 = argv[2];
char *file3 = argv[3];

mat1 = lerMatriz(file1, &linhas_m1, &colunas_m1);
mat2 = lerMatriz(file2, &linhas_m2, &colunas_m2);

result = (double*) malloc(linhas_m1 * colunas_m2 * sizeof(double));

//imprimirMatriz(mat1, linhas_m1, colunas_m1);
//imprimirMatriz(mat2, linhas_m2, colunas_m2);

multiplicarMatrizes(result, mat1, linhas_m1, colunas_m1, mat2, linhas_m2, colunas_m2);

//imprimirMatriz(result, linhas_m1, colunas_m2);

escreverMatriz(file3, result, linhas_m1, colunas_m2);

liberarMatriz(mat1);
liberarMatriz(mat2);

return 0;

}