#ifndef AIBB
#define AIBB

#define INFINITY 99999999

float *lerMatriz(char *str1, int *linhas, int *colunas);
void imprimirMatriz(float *data, int row, int column);
void escreverMatriz(char *out_mat, float *data, int row, int column);
void multiplicarMatrizes(float *result, float *mat1, int line_m1, int col_m1, float *mat2, int line_m2, int col_m2);
void liberarMatriz(float *data);
void BellmanFord_serial(int vertices, float *mat1);
void BellmanFord_paralelo(int vertices, float *g, int p);

#endif // AIBB