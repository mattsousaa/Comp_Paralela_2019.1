#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

/*
####################################################
=> Execução serial
# gcc integral_serial.c -o serial -lm
# time ./serial 1.0 10000.0 80000000
####################################################
=> Execução paralela
# mpicc integral_paralelo.c -o paralelo -lm
# time mpirun -np 8 ./paralelo 1.0 10000.0 80000000
####################################################
*/

double f(double x) {
   double return_val = 0.0;
   // return_val = x*x*x*x - x*x*x + x*x - x + 1;
   return_val = cos(x * log(1.0/x));
   return return_val;
}

int main(int argc, char *argv[]) {

    // Valor da integral global
    double integral_global = 0; 
    // Limites do intervalo
    double a, b;
    // Número de trapézios
    int n;
    // Variável local para armazenar o resultado de uma parte da integral em cada processo
    double calc_task = 0;
    // Base do trapézio
    double h;
    double x;

    /* Recebe os argumentos */
    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoi(argv[3]);

    /* Variáveis do MPI */
    int tasks_qtd, It_qtd, rank;

    /* Inicialização do MPI */
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &tasks_qtd);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Cálculo do tamanho do trapézio */ 
    h = (b - a) / n;

    /* Variável que guarda a diferença dos intervalos do trapézio */
    double num_intervalo = b - a;
    
    /* Todos os processos calculam um x local */
    x = ((num_intervalo / tasks_qtd) * rank) + a;

    /* Todos os processos calculam um pedaço da integral, ou seja "n/tasks_qtd" */
    for(int i = 0; i < n/tasks_qtd; i++){
        x += h;
        calc_task += f(x);
    }

    /* A ideia é somar a contribuição de cada processo com uma operação de redução */
    MPI_Reduce(&calc_task, &integral_global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    /* processo com rank = 0 consolida o cálculo total e soma a integral global */ 
    if(rank == 0){
        integral_global += (f(a) + f(b))/2.0;
        integral_global *= h;
        printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral_global);
    }

    MPI_Finalize();

    return 0;
}
