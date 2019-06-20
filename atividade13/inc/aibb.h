#ifndef AIBB
#define AIBB

#define INFINITY 99999999
#define INIT_SYNCRONIZATION(calc_thread_total, calc_thread, who_am_i) calc_thread_total |= calc_thread[who_am_i]

float *lerMatriz(char *str1, int *linhas, int *colunas);
void imprimirMatriz(float *data, int row, int column);
void escreverMatriz(char *out_mat, float *data, int row, int column);
void multiplicarMatrizes(float *result, float *mat1, int line_m1, int col_m1, float *mat2, int line_m2, int col_m2);
void liberarMatriz(float *data);
void BellmanFord_serial(float *mat1, int v_grafo);
void BellmanFord_paralelo(float *mat1, int v_grafo, int qtd_threads);

#endif // AIBB