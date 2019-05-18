#include <stdlib.h>
#include <mpi.h>
#include "aibb.h"

int main(int argc, char *argv[]){

/* Variáveis da quantidade de tasks e ranks */
int tasks_qtd, rank;

/* Variáveis de dimensões dos arrays */
int rows_m1, cols_m1, rows_m2, cols_m2;
int vec_matriz[] = {rows_m1, cols_m1};
int vec_vetor[] = {rows_m2, cols_m2};
int calc_local = 0;

/* Ponteiros para recebimento de matrizes */
float *mat1, *mat2, *result;
float *b_mat1, *b_mat2, *b_result;
float *gather_mat, *gather_vet;

/* Arquivos: matriz.txt, vetor.txt, resultado.txt */
char *file1 = argv[1];
char *file2 = argv[2];
char *file3 = argv[3];

/* Vetores para matriz e sub-matriz */
int dimMat2[2];
int dimSubMat1[2];

/* Inicialização e configuração inicial MPI */
MPI_Status status;
MPI_Init(&argc, &argv); 
MPI_Comm_size(MPI_COMM_WORLD, &tasks_qtd);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

/* O rank = 0 irá fazer a distribuição da matriz e do vetor */
if(rank == 0){
    /* Leitura dos arrays:
    mat1 é a matriz e mat2 é o vetor em formato de uma matriz qualquer */
    mat1 = lerMatriz(file1, &vec_matriz[0], &vec_matriz[1]);
    mat2 = lerMatriz(file2, &vec_vetor[0], &vec_vetor[1]);

}

/* Envia as dimensões do vetor para todos os processos */
MPI_Bcast(vec_vetor, 2, MPI_INT, 0, MPI_COMM_WORLD);

if(rank != 0)
    mat2 = initMatrixWithZero(vec_vetor[0], vec_vetor[1]);

MPI_Bcast(mat2, (vec_vetor[0] * vec_vetor[1]), MPI_FLOAT, 0, MPI_COMM_WORLD);
MPI_Bcast(vec_matriz, 2, MPI_INT, 0, MPI_COMM_WORLD);


calc_local = (vec_matriz[1] / tasks_qtd);

b_mat1 = (float*) malloc(vec_matriz[1] * calc_local * sizeof(float));

MPI_Scatter(mat1, calc_local * vec_matriz[1], MPI_FLOAT, b_mat1, vec_matriz[1] * calc_local, \
            MPI_FLOAT, 0, MPI_COMM_WORLD);

multiplicarMatrizes(result, b_mat1, vec_matriz[0], vec_matriz[1], mat2, vec_vetor[0], vec_vetor[1]);

MPI_Gather(result, calc_local * vec_vetor[1], MPI_FLOAT, b_mat2, calc_local * vec_vetor[1], MPI_FLOAT, 0, MPI_COMM_WORLD);

liberarMatriz(b_mat1);

if(rank == 0){
    escreverMatriz(file3, b_mat2, vec_matriz[0], vec_vetor[1]);
}

/* Encerra MPI */
MPI_Finalize();

return 0;

}