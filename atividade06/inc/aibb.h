#ifndef AIBB
#define AIBB

float *lerMatriz(char *str1, int *linhas, int *colunas);
void imprimirMatriz(float *data, int row, int column);
void escreverMatriz(char *out_mat, float *data, int row, int column);
void multiplicarMatrizes(float *result, float *mat1, int line_m1, int col_m1, float *mat2, int line_m2, int col_m2);
float *initMatrixWithZero(int rows, int columns);
void liberarMatriz(float *data);

#endif // AIBB