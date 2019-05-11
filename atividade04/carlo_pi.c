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

int monteCarloPi(int n){

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
   //return (double) 4*acertos / n;
   return acertos; // A ideia agora é fazer uma operação de redução
}

int main(int argc, char* argv[]){

   int tasks_qtd, It_qtd, rank;
   int value = 0;
   int pi = 0;
   MPI_Init(&argc, &argv); 
   MPI_Comm_size(MPI_COMM_WORLD, &tasks_qtd);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
   srand(time(NULL) + (rank * rank) % RAND_MAX);

   It_qtd = (atoi(argv[1]) / tasks_qtd);
   value = monteCarloPi(It_qtd);
   MPI_Reduce(&value, &pi, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   
   // Processo raíz
   if(rank == 0)    printf("pi = %.5f\n", 4*((double)pi / (It_qtd * tasks_qtd)));

   MPI_Finalize();
   
   return 0;

}