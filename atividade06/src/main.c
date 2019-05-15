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

    /* Variável que armazenará as dimensões do vetor */
    dimMat2[0] = vec_vetor[1]; 
    dimMat2[1] = vec_vetor[0];

     /* Vetor de auxílio da submatriz particionada */
    dimSubMat1[0] = (vec_matriz[0] / tasks_qtd);
    dimSubMat1[1] = vec_matriz[1];

    MPI_Bcast(dimMat2, 2, MPI_INT, 0, MPI_COMM_WORLD);        
    MPI_Bcast(mat2, vec_vetor[0]*vec_vetor[1], MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Bcast(dimSubMat1, 2, MPI_INT, 0, MPI_COMM_WORLD);        
    MPI_Bcast(mat1 + rank*(dimSubMat1[0]*vec_matriz[1]), dimSubMat1[0]*vec_matriz[1], MPI_FLOAT, 0, MPI_COMM_WORLD);
    
} else{

    MPI_Bcast(dimMat2, 2, MPI_INT, 0, MPI_COMM_WORLD);
    b_mat2 = (float*) malloc(dimMat2[0] * dimMat2[1] * sizeof(float));           
    MPI_Bcast(b_mat2, vec_vetor[0]*vec_vetor[1], MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Bcast(dimSubMat1, 2, MPI_INT, 0, MPI_COMM_WORLD);        
    b_mat1 = (float*) malloc(dimSubMat1[0] * dimSubMat1[1] * sizeof(float));
    MPI_Bcast(b_mat1, dimSubMat1[0]*dimSubMat1[1], MPI_FLOAT, 0, MPI_COMM_WORLD);
    
}

/* Consolidação de resultados */
if(rank == 0){
    /* Alocação do resultado final em um array dinâmico com dimensões obtidas */
    gather_mat = (float*) malloc(vec_matriz[0] * vec_vetor[0] * sizeof(float));
    result = (float*) malloc(dimSubMat1[0] * dimMat2[1] * sizeof(float));
    /* Chama função para multiplicar as matrizes - Realiza Cálculo local */
    multiplicarMatrizes(result, mat1, dimSubMat1[0], vec_matriz[1], mat2, dimMat2[0], dimMat2[1]);
   
    MPI_Gather(result, dimSubMat1[0]*vec_vetor[1], MPI_FLOAT, gather_mat, dimSubMat1[0]*vec_vetor[0], MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* Escreve matriz resultante em um arquivo */
    escreverMatriz(file3 ,gather_mat, vec_matriz[0], vec_vetor[0]);

    /* Libera memórias para arrays alocados dinamicamente */
    liberarMatriz(mat1);
    liberarMatriz(mat2);
    liberarMatriz(result);
    liberarMatriz(gather_mat);

} else{
    /* Aloca memória para o resultado da multiplicação */
    b_result = (float*) malloc(dimSubMat1[0]*dimMat2[1] * sizeof(float));

    /* Chama função de multiplicação para cálculo local */
    multiplicarMatrizes(b_result, b_mat1, dimSubMat1[0], dimSubMat1[1], b_mat2, dimMat2[0], dimMat2[1]);
    
    MPI_Gather(b_result, dimSubMat1[0]*dimMat2[1], MPI_FLOAT, gather_mat, dimSubMat1[0]*dimMat2[1], MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* Libera espaço da memória para os arrays alocados */
    liberarMatriz(b_mat1);
    liberarMatriz(b_mat2);
    liberarMatriz(b_result);

}

/* Encerra MPI */
MPI_Finalize();

return 0;

}