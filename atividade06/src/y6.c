#include "mpi.h"
#include "operacoesMatriz.h"

int main (int argc, char * argv[]){
    int lM,lV,cM,cV;
    float *mat, *vet, *mresult, *mresFinal;
    float *gather_mat, *gather_vet;
    int rank, numtask;
    int infoVetor[2], infoSubMatriz[2];
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtask);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
        // carregando a matriz e o vetor para a memória
        mat = lerMatriz(argv[1], &lM, &cM);
        vet = lerMatriz(argv[2], &lV, &cV);

        if(cM != lV){
            printf("Valores de tamanhos distintos. Isso vai dar errado!\n");
        }

        infoVetor[0] = lV;
        infoVetor[1] = cV;

        // broadcast
        MPI_Bcast(infoVetor, 2, MPI_INT, 0, MPI_COMM_WORLD);        
        MPI_Bcast(vet, lV*cV, MPI_FLOAT, 0, MPI_COMM_WORLD);
        
        infoSubMatriz[0] = lM / numtask;
        infoSubMatriz[1] = cM;
        
        // broadcast
        MPI_Bcast(infoSubMatriz, 2, MPI_INT, 0, MPI_COMM_WORLD);        
        MPI_Bcast(mat + rank*(infoSubMatriz[0]*cM), infoSubMatriz[0]*cM, MPI_FLOAT, 0, MPI_COMM_WORLD);

        mresFinal = (float*) malloc(lM*cV*sizeof(float));        
        mresult = (float*) malloc(infoSubMatriz[0]*infoVetor[1]*sizeof(float));
        // calculo local
        multiplicaMatrizes(mresult, mat, infoSubMatriz[0], cM, vet, lV, cV);

        // receber a contribuicao dos outros ranks
        MPI_Gather(mresult, infoSubMatriz[0]*cV, MPI_FLOAT, mresFinal, infoSubMatriz[0]*cV, MPI_FLOAT, 0, MPI_COMM_WORLD);
        
        escreverMatrizArquivo(argv[3] ,mresFinal, lM, cV);

        liberarMatriz(mat);
        liberarMatriz(vet);
        liberarMatriz(mresult);
    }
    else{
        MPI_Bcast(infoVetor, 2, MPI_INT, 0, MPI_COMM_WORLD);
        vet = (float*) malloc(infoVetor[0]*infoVetor[1]*sizeof(float));             
        MPI_Bcast(vet, infoVetor[0]*infoVetor[1], MPI_FLOAT, 0, MPI_COMM_WORLD);

        MPI_Bcast(infoSubMatriz, 2, MPI_INT, 0, MPI_COMM_WORLD);        
        mat = (float*) malloc(infoSubMatriz[0]*infoSubMatriz[1]*sizeof(float));
        MPI_Bcast(mat, infoSubMatriz[0]*infoSubMatriz[1], MPI_FLOAT, 0, MPI_COMM_WORLD);

        // calculo local
        mresult = (float*) malloc(infoSubMatriz[0]*infoVetor[1]*sizeof(float));
        multiplicaMatrizes(mresult, mat, infoSubMatriz[0], infoSubMatriz[1], vet, infoVetor[0], infoVetor[1]);

        MPI_Gather(mresult, infoSubMatriz[0]*infoVetor[1], MPI_FLOAT, mresFinal, infoSubMatriz[0]*infoVetor[1], MPI_FLOAT, 0, MPI_COMM_WORLD);

        liberarMatriz(mat);
        liberarMatriz(vet);
        liberarMatriz(mresult);
    }
    
    MPI_Finalize();
    return 0;
}