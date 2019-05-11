#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

/*
01. monteCarloPi(n) 
02. | acertos ← 0 
03. | para i ← 0 até n 
04. | | x ← sorteie um número real entre 0 e 1 
05. | | y ← sorteie um número real entre 0 e 1 
06. | | se(x * x + y * y < 1) 
07. | | | acertos ← acertos + 1 
08. | | fim_se 
09. | fim_para 
10. | retorne 4 * acertos / n 
11. fim_monteCarloPi

$ mpicc pi.c -o pi
$ mpirun -np 2 ./pi 1000
pi = 3,142342
*/

double monteCarloPi(int n){

double i = 0;
double x = 0;
double y = 0;
double z = 0;
int acertos = 0;

srand(time(NULL));

for(i = 0; i < n; i++){

   x = (double)rand() / RAND_MAX;
   y = (double)rand() / RAND_MAX;

   z = x * x + y * y;

   if(z <= 1) acertos++;
}
   return (double) 4*acertos / n;
}

int main(int argc, char* argv[]){

   int tasks_qtd, It_qtd, rank;
   float value = 0;
   int i = 0;
   float pi = 0;
   MPI_Init(&argc, &argv); 
   MPI_Comm_size(MPI_COMM_WORLD, &tasks_qtd);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
   srand(time(NULL) + (rank * rank) % RAND_MAX);

   It_qtd = (atoi(argv[1]) / tasks_qtd);

   if(rank == 0){
      pi = monteCarloPi(It_qtd);

      for(i = 1; i < tasks_qtd; i++){
         MPI_Recv(&value, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, NULL);
         pi += value;
      }

      printf("pi = %.5f\n", pi/tasks_qtd);
   } else {
      pi = monteCarloPi(It_qtd);
      MPI_Send(&pi, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
   }

   MPI_Finalize();
   
   return 0;
}