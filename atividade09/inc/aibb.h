#ifndef AIBB
#define AIBB

double *lerMatriz(char *str1, int *linhas, int *colunas);
void imprimirMatriz(double *data, int row, int column);
void escreverMatriz(char *out_mat, double *data, int row, int column);
void multiplicarMatrizes(double *result, double *mat1, int line_m1, int col_m1, double *mat2, int line_m2, int col_m2);
void liberarMatriz(double *data);

#endif // AIBB