#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include "aibb.h"

int main(int argc, char *argv[]){

/* Variáveis da quantidade de tasks e ranks */
int tasks_qtd, rank;

/* Variáveis de dimensões dos arrays */
int rows_m1, cols_m1, rows_m2, cols_m2;
int vec_matriz[] = {rows_m1, cols_m1};
int vec_vetor[] = {rows_m2, cols_m2};

/* Ponteiros para recebimento de matrizes */
double *mat1, *mat2, *result;
double *b_mat1, *b_mat2, *b_result;

/* Arquivos: matriz.txt, vetor.txt, resultado.txt */
char *file1 = argv[1];
char *file2 = argv[2];
char *file3 = argv[3];

omp_set_num_threads(atoi(argv[4]));

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

    /* Variável que armazenará as dimensões do vetor */
    dimMat2[0] = vec_vetor[0]; 
    dimMat2[1] = vec_vetor[1];

     /* Vetor de auxílio da submatriz particionada */
    dimSubMat1[0] = (vec_matriz[0] / tasks_qtd);
    dimSubMat1[1] = vec_matriz[1];

    /* Este Rank irá distribuir o vetor para todos os processos */
    for(int i = 1; i < tasks_qtd; i++){
        /* Envia tamanho das dimensões do vetor */
        MPI_Send(dimMat2, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
        /* Envia o vetor propriamente dito */
        MPI_Send(mat2, dimMat2[0]*dimMat2[1], MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
    }

    /* Distribuição da matriz particionada para todos os processos */
    for(int i = 1; i < tasks_qtd; i++){
        /* Envia o tamanho das dimensões da matriz particionada*/
        MPI_Send(dimSubMat1, 2, MPI_INT, i, 2, MPI_COMM_WORLD);
        /* Envia a submatriz propriamente dita */
        MPI_Send(mat1 + i*(dimSubMat1[0]*dimSubMat1[1]), dimSubMat1[0]*dimSubMat1[1], MPI_DOUBLE, i, 3, MPI_COMM_WORLD);
    }
    
} else{
    
    /* Recebe as dimensões do vetor */
    MPI_Recv(dimMat2, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    /* Alocação do vetor na variável mat2 */
    b_mat2 = (double*) malloc(dimMat2[0] * dimMat2[1] * sizeof(double));
    /* Recebe o vetor do processo 0 - rank 0 */
    MPI_Recv(b_mat2, dimMat2[0]*dimMat2[1], MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);

    /* Recebe as dimensões da matriz particionada */
    MPI_Recv(dimSubMat1, 2, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    /* Aloca a matriz particionada na memória */
    b_mat1 = (double*) malloc(dimSubMat1[0] * dimSubMat1[1] * sizeof(double));
    /* Recebe a matriz particionada */
    MPI_Recv(b_mat1, dimSubMat1[0]*dimSubMat1[1], MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, &status);

    
}

/* Consolidação de resultados */
if(rank == 0){
    /* Alocação do resultado final em um array dinâmico com dimensões obtidas */
    result = (double*) malloc(vec_matriz[0] * vec_vetor[1] * sizeof(double));

    /* Chama função para multiplicar as matrizes - Realiza Cálculo local */
    multiplicarMatrizes(result, mat1, dimSubMat1[0], vec_matriz[1], mat2, dimMat2[0], dimMat2[1]);

    /* Recebe o cálculo de contribuição dos outros processos */
    for(int i = 1; i < tasks_qtd; i++)
        MPI_Recv(result + i*(dimSubMat1[0]*vec_vetor[1]), dimSubMat1[0]*vec_vetor[1], MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);

    /* Escreve matriz resultante em um arquivo */
    escreverMatriz(file3 ,result, vec_matriz[0], vec_vetor[1]);

    /* Libera memórias para arrays alocados dinamicamente */
    liberarMatriz(mat1);
    liberarMatriz(mat2);
    liberarMatriz(result);

} else{
    /* Aloca memória para o resultado da multiplicação */
    b_result = (double*) malloc(dimSubMat1[0]*dimMat2[1] * sizeof(double));

    /* Chama função de multiplicação para cálculo local */
    multiplicarMatrizes(b_result, b_mat1, dimSubMat1[0], dimSubMat1[1], b_mat2, dimMat2[0], dimMat2[1]);
    
    /* O resultado do cálculo local é enviado para o rank 0 */
    MPI_Send(b_result, dimSubMat1[0]*dimMat2[1], MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    /* Libera espaço da memória para os arrays alocados */
    liberarMatriz(b_mat1);
    liberarMatriz(b_mat2);
    liberarMatriz(b_result);
}

/* Encerra MPI */
MPI_Finalize();

return 0;

}