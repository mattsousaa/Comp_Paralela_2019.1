#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

/* Variáveis Globais */
MPI_Status status;
int rank, tasks_qtd;
int right, left;

int printArrayForRank(int rank, int *array,  int arraySize) {
   int i;
   char *output = (char *) malloc((arraySize + 100) * 4 * sizeof(char));
   sprintf(output, "Rank %d: ", rank);
   for (int i = 0; i < arraySize; i++)
      sprintf(output, "%s %d", output, array[i]);
   sprintf(output,"%s\n", output);
   printf("%s", output);
   fflush(stdout);
   free(output);
   return i;
}

int MPI_Ring_broadcast(int *array, int arraySize, MPI_Comm comm) {
   /* Distribuição em anel para os ranks - Envio de dados para esquerda e direita*/
   if(rank == 0){
      MPI_Send(array, arraySize, MPI_INT, right, 0, comm);
      MPI_Send(array, arraySize, MPI_INT, left, 0, comm);
   } else if(rank >= (tasks_qtd / 2)){
      MPI_Recv(array, arraySize, MPI_INT, right, 0, comm, &status);
      MPI_Send(array, arraySize, MPI_INT, left, 0, comm);
   } else{
      MPI_Recv(array, arraySize, MPI_INT, left, 0, comm, &status);
      MPI_Send(array, arraySize, MPI_INT, right, 0, comm);
   }
}

int MPI_Mesh_broadcast(int *array, int arraySize, MPI_Comm comm) {
   
   /* Definição do Mesh */
   int grid[] = {4,4};
   int aux[] = {0,0};
   int local_mesh[2];   /* Variável para coordenada do mesh */
   int *neighbors;      /* Variável para os vizinhos */
   MPI_Comm mesh_broadcast;
   /* Cria comunicação para uma topologia cartesiana */
   MPI_Cart_create(comm, 2, grid, aux, 0, &mesh_broadcast);

   /* Aloca vizinhos e desloca com a coordenada */
   neighbors = malloc(4 * sizeof(int));
   MPI_Cart_shift(mesh_broadcast, 0, 1, &neighbors[0], &neighbors[1]);
   MPI_Cart_shift(mesh_broadcast, 1, 1, &neighbors[2], &neighbors[3]);

   /* Pega a coordenada do mesh */
   MPI_Cart_coords(mesh_broadcast, rank, 2, local_mesh);

   /* Se a localização do mesh for 0... */
   if(local_mesh[1] == 0){
      /* A posição recebe o dado do vizinho de cima caso o rank seja diferente de 0 */
      if(rank != 0)  MPI_Recv(array, arraySize, MPI_INT, neighbors[0], 0, MPI_COMM_WORLD, &status);
      
      /* Envia os dados para todos os vizinhos de baixo e da direita */
      MPI_Send(array, arraySize, MPI_INT, neighbors[3], 0, MPI_COMM_WORLD);
      MPI_Send(array, arraySize, MPI_INT, neighbors[1], 0, MPI_COMM_WORLD);
   } else{
      /* Envia os dados para todos os vizinhos da direita */
      MPI_Recv(array, arraySize, MPI_INT, neighbors[2], 0, MPI_COMM_WORLD, &status);
      MPI_Send(array, arraySize, MPI_INT, neighbors[3], 0, MPI_COMM_WORLD);
   }
}

int main(int argc, char *argv[]) {
   //int size, rank;
   int *array, arraySize;

   /* Inicialização MPI */
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &tasks_qtd);

   /* Definição retirada do exemplo encontrado no repositório da disciplina */
   left = (((rank - 1) + tasks_qtd) % tasks_qtd);
   right = (rank + 1) % tasks_qtd;

   arraySize = atoi(argv[1]);
   array = (int *) malloc(arraySize * sizeof(int));
 
   // Inicializa o array com 0.
   if (rank == 0) {
     for (int i = 0; i < arraySize; i++)
         array[i] = 0;
   }

   // Imprime o array inicial de todos os processos.
   printArrayForRank(rank, array, arraySize);

   // Faz broadcast usando uma topologia anel.
   MPI_Ring_broadcast(array, arraySize, MPI_COMM_WORLD);

   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);

   // Imprime o array após o broadcast na topologia anel.
   printArrayForRank(rank, array, arraySize);
  
   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);
   
   // Reconfigura o array com todos os elementos iguais a 1000.
   if (rank == 0) {
      for (int i = 0; i < arraySize; i++)
         array[i] =  1000;
   }

   // Faz broadcast usando uma topologia mesh.
   MPI_Mesh_broadcast(array, arraySize, MPI_COMM_WORLD);

   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);

   // Imprime o array após o broadcast na topologia mesh.
   printArrayForRank(rank, array, arraySize);
     
   free(array);
   MPI_Finalize();

   return 0;
}